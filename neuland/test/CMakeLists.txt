##############################################################################
#   Copyright (C) 2019 GSI Helmholtzzentrum für Schwerionenforschung GmbH    #
#   Copyright (C) 2019-2024 Members of R3B Collaboration                     #
#                                                                            #
#             This software is distributed under the terms of the            #
#                 GNU General Public Licence (GPL) version 3,                #
#                    copied verbatim in the file "LICENSE".                  #
#                                                                            #
# In applying this license GSI does not waive the privileges and immunities  #
# granted to it by virtue of its status as an Intergovernmental Organization #
# or submit itself to any jurisdiction.                                      #
##############################################################################

cmake_minimum_required(VERSION 3.23)
set(PROJECT_TEST_NAME NeulandUnitTests)

if(GTEST_FOUND)
    file(GLOB TEST_SRC_FILES ${PROJECT_SOURCE_DIR}/neuland/test/*.cxx)
    add_subdirectory(digitizing)

    include_directories(
        ${SYSTEM_INCLUDE_DIRECTORIES}
        ${BASE_INCLUDE_DIRECTORIES}
        ${R3BROOT_SOURCE_DIR}/r3bbase
        ${R3BROOT_SOURCE_DIR}/r3bdata/neulandData
        ${R3BROOT_SOURCE_DIR}/neuland/shared
        ${R3BROOT_SOURCE_DIR}/neuland/reconstruction
        ${R3BROOT_SOURCE_DIR}/neuland/digitizing
        ${R3BROOT_SOURCE_DIR}/neuland/reconstruction/multiplicity)

    link_directories(${ROOT_LIBRARY_DIR} ${FAIRROOT_LIBRARY_DIR}
        ${Boost_LIBRARY_DIRS})

    set(TEST_DEPENDENCIES
        GTest::gtest_main
        GTest::gmock_main
        ${ROOT_LIBRARIES}
        FairTools
        R3BData
        ParBase
        GeoBase
        R3BNeulandShared
        R3BNeulandDigitizing
        R3BNeulandReconstruction
        Alignment)

    add_executable(${PROJECT_TEST_NAME} ${TEST_SRC_FILES})
    target_link_libraries(${PROJECT_TEST_NAME} ${TEST_DEPENDENCIES})
    gtest_discover_tests(${PROJECT_TEST_NAME} DISCOVERY_TIMEOUT 600)
endif(GTEST_FOUND)

# generate_root_test_script(${R3BROOT_SOURCE_DIR}/neuland/test/testNeulandSimulation.C)
# add_test(NeulandSimulation ${R3BROOT_BINARY_DIR}/neuland/test/testNeulandSimulation.sh)
set(simuPars
    --simuFile test.simu.root
    --paraFile test.para.root
    --eventNum 100
    --eventPrint 10
    --energy 1.0
    --multiplicity 4
    --logLevel error
    )
add_test(NeulandSimulation ${R3BROOT_BINARY_DIR}/bin/neulandSim ${simuPars})
set_tests_properties(NeulandSimulation PROPERTIES TIMEOUT "2000")
set_tests_properties(NeulandSimulation PROPERTIES PASS_REGULAR_EXPRESSION
    "Macro finished successfully.")

set(digiPars
    --simuFile test.simu.root
    --paraFile test.para.root
    --paddle neuland
    --channel tamex
    --digiFile test.digi.root
    -v info
    )
add_test(NeulandDigitizer ${R3BROOT_BINARY_DIR}/bin/neulandAna ${digiPars})
set_tests_properties(NeulandDigitizer PROPERTIES DEPENDS NeulandSimulation)
set_tests_properties(NeulandDigitizer PROPERTIES TIMEOUT "1000")
set_tests_properties(NeulandDigitizer PROPERTIES PASS_REGULAR_EXPRESSION
    "Macro finished successfully.")

generate_root_test_script(${R3BROOT_SOURCE_DIR}/neuland/test/testNeulandDigitizer.C)
add_test(NeulandDigitizerMacro ${R3BROOT_BINARY_DIR}/neuland/test/testNeulandDigitizer.sh)
set_tests_properties(NeulandDigitizerMacro PROPERTIES DEPENDS NeulandSimulation)
set_tests_properties(NeulandDigitizerMacro PROPERTIES TIMEOUT "1000")
set_tests_properties(NeulandDigitizerMacro PROPERTIES PASS_REGULAR_EXPRESSION
    "Macro finished successfully.")


# generate_root_test_script(${R3BROOT_SOURCE_DIR}/neuland/test/testNeulandTrain.C)
# add_test(NeulandTrain ${R3BROOT_BINARY_DIR}/neuland/test/testNeulandTrain.sh)
# set_tests_properties(NeulandTrain PROPERTIES DEPENDS NeulandDigitizer)
# set_tests_properties(NeulandTrain PROPERTIES TIMEOUT "1000")
# set_tests_properties(NeulandTrain PROPERTIES PASS_REGULAR_EXPRESSION "Macro finished successfully.")

# generate_root_test_script(${R3BROOT_SOURCE_DIR}/neuland/test/testNeulandReconstruction.C)
# add_test(NeulandReconstruction ${R3BROOT_BINARY_DIR}/neuland/test/testNeulandReconstruction.sh)
# set_tests_properties(NeulandReconstruction PROPERTIES DEPENDS NeulandTrain)
# set_tests_properties(NeulandReconstruction PROPERTIES TIMEOUT "1000")
# set_tests_properties(NeulandReconstruction PROPERTIES PASS_REGULAR_EXPRESSION
#                                                       "Macro finished successfully.")

add_subdirectory(calibration)
