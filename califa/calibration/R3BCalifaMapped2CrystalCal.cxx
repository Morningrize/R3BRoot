/******************************************************************************
 *   Copyright (C) 2019 GSI Helmholtzzentrum für Schwerionenforschung GmbH    *
 *   Copyright (C) 2019 Members of R3B Collaboration                          *
 *                                                                            *
 *             This software is distributed under the terms of the            *
 *                 GNU General Public Licence (GPL) version 3,                *
 *                    copied verbatim in the file "LICENSE".                  *
 *                                                                            *
 * In applying this license GSI does not waive the privileges and immunities  *
 * granted to it by virtue of its status as an Intergovernmental Organization *
 * or submit itself to any jurisdiction.                                      *
 ******************************************************************************/

#include "TClonesArray.h"
#include "TMath.h"
#include "TRandom.h"

#include "FairLogger.h"
#include "FairRootManager.h"
#include "FairRunAna.h"
#include "FairRuntimeDb.h"

#include <iomanip>

#include "R3BCalifa.h"
#include "R3BCalifaCrystalCalData.h"
#include "R3BCalifaCrystalCalPar.h"
#include "R3BCalifaMapped2CrystalCal.h"
#include "R3BCalifaMappedData.h"
#include "R3BCalifaTotCalPar.h"

#ifndef PLOG
#define PLOG(LVL) LOG(LVL) << __PRETTY_FUNCTION__ << ": "
#endif

// R3BCalifaMapped2CrystalCal: Constructor
R3BCalifaMapped2CrystalCal::R3BCalifaMapped2CrystalCal()
    : FairTask("R3B CALIFA Calibrator")
    , NumCrystals(0)
    , NumParams(0)
    , fCalParams(NULL)
    , fCal_Par(NULL)
    , fOnline(kFALSE)
    , fCalifaMappedDataCA(NULL)
    , fCalifaCryCalDataCA(NULL)
{
}

R3BCalifaMapped2CrystalCal::~R3BCalifaMapped2CrystalCal()
{
    LOG(INFO) << "R3BCalifaMapped2CrystalCal: Delete instance";

    /*
     * Note to whomever felt in neccessary to add: "delete fCalifaMappedDataCA;"
     * This will cause a double free because  R3BCalifaFebexReader (reasonably)
     * considers itself the owner of that object.
     * Short version: if your class did not allocate the object with new,
     * it also (typically) may not delete it. -- pklenze
     */
    if (fCalifaCryCalDataCA)
        delete fCalifaCryCalDataCA;
}

void R3BCalifaMapped2CrystalCal::SetParContainers()
{
    // Parameter Container
    // Reading califaCrystalCalPar from FairRuntimeDb
    FairRuntimeDb* rtdb = FairRuntimeDb::instance();
    if (!rtdb)
    {
        LOG(ERROR) << "R3BCalifaMapped2CrystalCal:: FairRuntimeDb not opened";
    }

    fCal_Par = (R3BCalifaCrystalCalPar*)rtdb->getContainer("califaCrystalCalPar");
    if (!fCal_Par)
    {
        LOG(ERROR) << "R3BCalifaMapped2CrystalCal::Init() Couldn't get handle on califaCrystalCalPar container";
    }
    else
    {
        LOG(INFO) << "R3BCalifaMapped2CrystalCal:: califaCrystalCalPar container open";
    }

    fTotCal_Par = (R3BCalifaTotCalPar*)rtdb->getContainer("CalifaTotCalPar");
    if (!fTotCal_Par)
    {
        LOG(WARNING) << "R3BCalifaCrystalCal2TotCalPar::Init() Couldn't get handle on CalifaTotCalPar container";
    }
    if (fTotCal_Par)
    {
        LOG(INFO) << "R3BCalifaCrystalCal2TotCalPar:: CalifaTotCalPar container open";
    }
}

void R3BCalifaMapped2CrystalCal::SetParameter()
{
    // NB: the handling of calibration parameters should be redone from the scratch.
    // The par-file format  is not really human-readable (the root file even is worse).
    // Luckily, this does not matter because the choice to store the values in the
    // class using two one-dimensional array precludes any possibility of humans groking
    // it.
    // A sensible reimplementation might store the calibration in csv lines (with the crID
    // coming first) or go down the json rabbit hole.
    // For using the calibration here,  an std::vector of some struct {m, c, totAmp, totTime}
    // would be convenient. Nobody wants higher order polynomials for calibration.
    // Also, if you insist on crIDs starting from one (that ship has long sailed), then
    // please just leave the first array entry undefined and use params(id) == a[id], instead
    // of bothering with params(id)==a[id-1].
    // See also: https://github.com/R3BRootGroup/R3BRoot/issues/473
    // Just my 2 cents. -- pklenze

    //--- Parameter Container ---
    NumCrystals = fCal_Par->GetNumCrystals();    // Number of Crystals
    NumParams = fCal_Par->GetNumParametersFit(); // Number of Parameters

    fCalParams = fCal_Par->GetCryCalParams(); // Array with the Cal parameters
    assert(fCalParams->GetSize() >= NumCrystals * NumParams);

    LOG(INFO) << "R3BCalifaMapped2CrystalCal:: Max Crystal ID " << NumCrystals;
    LOG(INFO) << "R3BCalifaMapped2CrystalCal:: Nb of parameters used in the fits " << NumParams;

    //--- Parameter Container --- Tot
    NumTotParams = fTotCal_Par->GetNumParametersFit(); // Number of Parameters

    fCalTotParams = fTotCal_Par->GetCryCalParams(); // Array with the Tot Cal parameters
    assert(fCalTotParams->GetSize() >= NumCrystals * NumTotParams);

    // handle old calibrations which mapped to barrel protons to crId+2432:
    // If you cal[id] is zero or nan (you wish),
    // And cal[id+2432] is nonzero,
    // Then make cal[id]=cal[id+2432] in case you are using the new unpacker
    // (where barrel is always in [1, 1952]) with an old calibration

    constexpr int offset = 2432;
    auto& cal = *fCalParams; // because (*ptr)[i] is ugly and error-prone
    auto& tot = *fCalTotParams;
    if (NumParams != 2 || NumCrystals < 2 * offset)
    {
        PLOG(WARNING) << "Not checking calibration in former proton range.";
        return;
    }
    auto invalid = [&cal](int id) {
        auto a = cal.GetAt(2 * (id - 1) + 0);
        auto b = cal.GetAt(2 * (id - 1) + 1);
        return (std::isnan(a) || a == 0.0) && (std::isnan(a) || a == 0.0);
    };

    int replaced{};
    for (int id = 1; id <= 1952; id++) // barrel range, formerly barrel gamma range
        if (invalid(id) && !invalid(id + offset))
        {
            ++replaced;
            // Note:  (*a)[n]=...
            for (int p = 0; p < NumParams; p++)
                cal[NumParams * (id - 1) + p] = cal[NumParams * (id - 1 + offset) + p];
            for (int p = 0; p < NumTotParams; p++)
                tot[NumTotParams * (id - 1) + p] = tot[NumTotParams * (id - 1 + offset) + p];
        }
    if (replaced)
        PLOG(WARNING) << replaced
                      << " missing calibrations for crIDs in [1, 1952] have been copied over from the legacy proton "
                         "barrel range.";
}

InitStatus R3BCalifaMapped2CrystalCal::Init()
{
    LOG(INFO) << "R3BCalifaMapped2CrystalCal::Init()";

    // INPUT DATA
    FairRootManager* rootManager = FairRootManager::Instance();
    if (!rootManager)
    {
        LOG(FATAL) << "R3BCalifaMapped2CrystalCal::FairRootManager not found";
        return kFATAL;
    }

    fCalifaMappedDataCA = (TClonesArray*)rootManager->GetObject("CalifaMappedData");
    if (!fCalifaMappedDataCA)
    {
        LOG(FATAL) << "R3BCalifaMapped2CrystalCal::CalifaMappedData not found";
        return kFATAL;
    }

    // OUTPUT DATA
    // Calibrated data
    fCalifaCryCalDataCA = new TClonesArray("R3BCalifaCrystalCalData", 50);

    rootManager->Register("CalifaCrystalCalData", "CALIFA Crystal Cal", fCalifaCryCalDataCA, !fOnline);

    SetParameter();
    return kSUCCESS;
}

InitStatus R3BCalifaMapped2CrystalCal::ReInit()
{
    SetParContainers();
    SetParameter();
    return kSUCCESS;
}

void R3BCalifaMapped2CrystalCal::Exec(Option_t* option)
{
    // Reset entries in output arrays, local arrays
    Reset();

    if (!fCal_Par)
    {
        LOG(WARNING) << "R3BCalifaMapped2CrystalCal::Parameter container not found";
    }

    // Reading the Input -- Mapped Data --
    Int_t nHits = fCalifaMappedDataCA->GetEntries();
    if (!nHits)
        return;

    R3BCalifaMappedData** mappedData;
    mappedData = new R3BCalifaMappedData*[nHits];

    // Overflow (R3BROOT-speech "Errors") handling:
    // If an error bit indicates that the data is invalid,
    // the correct approach is to set the invalid fields to NaN, imho

    //  source: mbs f_user dir, struct_event_115a.h
    // bit      name        invalidates

    //   0      CFD         nothing, trigger branch overflow
    //   1      Baseline    everything
    //   2      MAU         everything
    //   3      MWD         everything

    //   4      PeakSensing everything????
    //   5      E->EvntBuf  Energy
    //   6      Trace->EBuf nothing, traces are not handled by R3BROOT
    //   7      Nf->EvntBuf Nf

    //   8      Ns->EvntBuf Ns
    //   9      ADC         everything
    //   a      ADC underfl everything
    //   b      QPID Nf     Nf

    //   c      QPID Ns     Ns
    const uint32_t ANY_ERRORS = 0x061e;
    const uint32_t QPID_ERRORS = 0x1980 | ANY_ERRORS;
    const uint32_t EN_ERRORS = 0x0020 | ANY_ERRORS;

    for (Int_t i = 0; i < nHits; i++)
    {
        mappedData[i] = (R3BCalifaMappedData*)(fCalifaMappedDataCA->At(i));
        auto crystalId = mappedData[i]->GetCrystalId();
        auto wrts = mappedData[i]->GetWrts();
        auto ov = mappedData[i]->GetOverFlow();
        auto Tot = mappedData[i]->GetTot();
        auto validate_smear = [](uint16_t err_cond, double raw) {
            return err_cond ? NAN : raw + gRandom->Rndm() - 0.5;
        };
        enum id
        {
            en = 0,
            Nf = 1,
            Ns = 2
        };
        double raw[3];
        raw[en] = validate_smear(ov & EN_ERRORS, mappedData[i]->GetEnergy());
        raw[Nf] = validate_smear(ov & QPID_ERRORS, mappedData[i]->GetNf());
        raw[Ns] = validate_smear(ov & QPID_ERRORS, mappedData[i]->GetNs());
        double cal[3] = { 0, 0, 0 };

        if (0 < crystalId && crystalId <= NumCrystals)
            for (int idx = 0; idx < 3; idx++)
                for (int p = 0; p < NumParams; p++)
                {
                    cal[idx] +=
                        pow(raw[idx], (NumParams == 1) ? 1 : p) * fCalParams->GetAt(NumParams * (crystalId - 1) + p);
                }
        else
            for (int idx; idx < 3; idx++)
                cal[idx] = NAN;

        double TotCal = Tot;
        if (fCalTotParams)
        {
            double a0 = fCalTotParams->GetAt(NumTotParams * (crystalId - 1));
            double a1 = fCalTotParams->GetAt(NumTotParams * (crystalId - 1) + 1);
            TotCal = a0 * TMath::Exp(Tot / a1);
        }
        AddCalData(crystalId, cal[en], cal[Nf], cal[Ns], wrts, TotCal);
    }

    if (mappedData)
        delete[] mappedData; // FTFY
    return;
}

void R3BCalifaMapped2CrystalCal::Finish() {}

void R3BCalifaMapped2CrystalCal::Reset()
{
    LOG(DEBUG) << "Clearing CrystalCalData Structure";
    if (fCalifaCryCalDataCA)
        fCalifaCryCalDataCA->Clear();
}

R3BCalifaCrystalCalData* R3BCalifaMapped2CrystalCal::AddCalData(Int_t id,
                                                                Double_t energy,
                                                                Double_t Nf,
                                                                Double_t Ns,
                                                                uint64_t wrts,
                                                                Double_t tot_energy = 0)
{
    // It fills the R3BCalifaCrystalCalData
    TClonesArray& clref = *fCalifaCryCalDataCA;
    Int_t size = clref.GetEntriesFast();
    return new (clref[size]) R3BCalifaCrystalCalData(id, energy, Nf, Ns, wrts, tot_energy);
}

ClassImp(R3BCalifaMapped2CrystalCal)