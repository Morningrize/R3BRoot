##############################################################################
#   Copyright (C) 2019 GSI Helmholtzzentrum für Schwerionenforschung GmbH    #
#   Copyright (C) 2019 Members of R3B Collaboration                          #
#                                                                            #
#             This software is distributed under the terms of the            #
#                 GNU General Public Licence (GPL) version 3,                #
#                    copied verbatim in the file "LICENSE".                  #
#                                                                            #
# In applying this license GSI does not waive the privileges and immunities  #
# granted to it by virtue of its status as an Intergovernmental Organization #
# or submit itself to any jurisdiction.                                      #
##############################################################################

project(TestNeulandDigitizing)

file(GLOB TestFiles testNeulandDigitizingTamex.cxx testNeulandDigitizingPaddle.cxx)

set(TEST_SRC_FILES
    ${TEST_SRC_FILES} ${TestFiles}
    PARENT_SCOPE)
