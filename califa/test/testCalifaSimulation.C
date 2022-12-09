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

void testCalifaSimulation(const int nbevents = 100)
{
    // Timer
    TStopwatch timer;
    timer.Start();

    // Logging
    FairLogger::GetLogger()->SetLogVerbosityLevel("low");
    FairLogger::GetLogger()->SetLogScreenLevel("warn");
    FairLogger::GetLogger()->SetColoredLog(true);

    // System paths
    const TString workDirectory = getenv("VMCWORKDIR");
    gSystem->Setenv("GEOMPATH", workDirectory + "/geometry");
    gSystem->Setenv("CONFIG_DIR", workDirectory + "/gconfig");

    // Output files
    // const TString simufile = "test.simu.root";
    // const TString parafile = "test.para.root";

    // Basic simulation setup
    auto run = new FairRunSim();
    run->SetName("TGeant4");
    run->SetStoreTraj(false);
    run->SetMaterials("media_r3b.geo");
    // run->SetSink(new FairRootFileSink(simufile));

    // Primary particle generator
    auto boxGen = new FairBoxGenerator(2212, 8);
    boxGen->SetXYZ(0, 0, 0.);
    boxGen->SetThetaRange(7., 145.);
    boxGen->SetPhiRange(0., 360.);
    boxGen->SetEkinRange(0.6, 0.6);
    auto primGen = new FairPrimaryGenerator();
    primGen->AddGenerator(boxGen);
    run->SetGenerator(primGen);

    // Geometry: Cave
    auto cave = new R3BCave("CAVE");
    cave->SetGeometryFileName("r3b_cave.geo");
    run->AddModule(cave);

    // Geometry: Califa
    run->AddModule(new R3BCalifa("califa_2020.geo.root", { 0., 0., 0. }));

    // Digitizer: Califa
    auto califaDig = new R3BCalifaDigitizer();
    run->AddTask(califaDig);

    // Init
    run->Init();

    // Simulate
    run->Run(nbevents);

    // Report
    timer.Stop();
    std::cout << "Macro finished successfully." << std::endl;
    std::cout << "Real time: " << timer.RealTime() << "s, CPU time: " << timer.CpuTime() << "s" << std::endl;
    gApplication->Terminate();
}