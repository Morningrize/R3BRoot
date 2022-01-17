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

#include "R3BRpcMappedData.h"

R3BRpcMappedData::R3BRpcMappedData()
    : fChannelId(0)
    , fTime(0)
    , fWrts(0)
    , fSide(0)
{
}

R3BRpcMappedData::R3BRpcMappedData(UShort_t channelId, uint64_t time, uint64_t wrts, Int_t side)
    : fChannelId(channelId)
    , fTime(time)
    , fWrts(wrts)
    , fSide(side)
{
}

ClassImp(R3BRpcMappedData);
