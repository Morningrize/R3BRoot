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

# Create a library called "libR3Bsource" which includes the source files given
# in the array. The extension is already found. Any number of sources could be
# listed here.

set(SYSTEM_INCLUDE_DIRECTORIES
${SYSTEM_INCLUDE_DIRECTORIES}
${BASE_INCLUDE_DIRECTORIES}
${ucesb_INCLUDE_DIR}
)

set(INCLUDE_DIRECTORIES
#put here all directories where header files are located
${R3BROOT_SOURCE_DIR}/r3bsource
${R3BROOT_SOURCE_DIR}/r3bsource/base
${R3BROOT_SOURCE_DIR}/r3bsource/base/utils
${R3BROOT_SOURCE_DIR}/r3bsource/wr
${R3BROOT_SOURCE_DIR}/r3bsource/trloii
${R3BROOT_SOURCE_DIR}/r3bsource/beammonitor
${R3BROOT_SOURCE_DIR}/r3bsource/frssci
${R3BROOT_SOURCE_DIR}/r3bsource/sci2
${R3BROOT_SOURCE_DIR}/r3bsource/sci8
${R3BROOT_SOURCE_DIR}/r3bsource/rolu
${R3BROOT_SOURCE_DIR}/r3bsource/pspx
${R3BROOT_SOURCE_DIR}/r3bsource/pdc
${R3BROOT_SOURCE_DIR}/r3bsource/stubes
${R3BROOT_SOURCE_DIR}/r3bsource/strack
${R3BROOT_SOURCE_DIR}/r3bsource/califa
${R3BROOT_SOURCE_DIR}/r3bsource/neuland
${R3BROOT_SOURCE_DIR}/r3bsource/ams
${R3BROOT_SOURCE_DIR}/r3bsource/foot
${R3BROOT_SOURCE_DIR}/r3bsource/alpide
${R3BROOT_SOURCE_DIR}/r3bsource/music
${R3BROOT_SOURCE_DIR}/r3bsource/mwpc
${R3BROOT_SOURCE_DIR}/r3bsource/twim
${R3BROOT_SOURCE_DIR}/r3bsource/musli
${R3BROOT_SOURCE_DIR}/r3bsource/fibers
${R3BROOT_SOURCE_DIR}/r3bsource/tofd
${R3BROOT_SOURCE_DIR}/r3bsource/tofi
${R3BROOT_SOURCE_DIR}/r3bsource/rpc
${R3BROOT_SOURCE_DIR}/r3bsource/sync_check
${R3BROOT_SOURCE_DIR}/r3bbase
${R3BROOT_SOURCE_DIR}/r3bdata/beammonitorData
${R3BROOT_SOURCE_DIR}/r3bdata/califaData
${R3BROOT_SOURCE_DIR}/r3bdata/neulandData
${R3BROOT_SOURCE_DIR}/r3bdata/pspData
${R3BROOT_SOURCE_DIR}/r3bdata/fibData
${R3BROOT_SOURCE_DIR}/r3bdata/sfibData
${R3BROOT_SOURCE_DIR}/r3bdata/losData
${R3BROOT_SOURCE_DIR}/r3bdata/roluData
${R3BROOT_SOURCE_DIR}/r3bdata/frssciData
${R3BROOT_SOURCE_DIR}/r3bdata/sci2Data
${R3BROOT_SOURCE_DIR}/r3bdata/sci8Data
${R3BROOT_SOURCE_DIR}/r3bdata/tofData
${R3BROOT_SOURCE_DIR}/r3bdata/tofiData
${R3BROOT_SOURCE_DIR}/r3bdata/pdcData
${R3BROOT_SOURCE_DIR}/r3bdata/strawtubesData
${R3BROOT_SOURCE_DIR}/psp/par
${R3BROOT_SOURCE_DIR}/r3bdata/amsData
${R3BROOT_SOURCE_DIR}/r3bdata/footData
${R3BROOT_SOURCE_DIR}/r3bdata/alpideData
${R3BROOT_SOURCE_DIR}/r3bdata/wrData
${R3BROOT_SOURCE_DIR}/r3bdata/rpcData
${R3BROOT_SOURCE_DIR}/r3bdata/trloiiData
${R3BROOT_SOURCE_DIR}/r3bdata/musicData
${R3BROOT_SOURCE_DIR}/r3bdata/twimData
${R3BROOT_SOURCE_DIR}/r3bdata/tttxData
${R3BROOT_SOURCE_DIR}/r3bdata/musliData
${R3BROOT_SOURCE_DIR}/r3bdata/mwpcData
${R3BROOT_SOURCE_DIR}/r3bdata/sampData
${R3BROOT_SOURCE_DIR}/r3bdata/synccheckData
)

set(LINK_DIRECTORIES
${ROOT_LIBRARY_DIR}
${FAIRROOT_LIBRARY_DIR}
${ucesb_LIBRARY_DIR}
)

include_directories( ${INCLUDE_DIRECTORIES})
include_directories(SYSTEM ${SYSTEM_INCLUDE_DIRECTORIES})
link_directories( ${LINK_DIRECTORIES})

set(SRCS
./base/utils/R3BUcesbLauncher.cxx
./base/utils/R3BUcesbStructInfo.cxx
./base/R3BUcesbSource.cxx
./base/R3BUcesbSource2.cxx
./base/R3BReader.cxx
./base/R3BUnpackReader.cxx
./wr/R3BWhiterabbitMasterReader.cxx
./califa/R3BWhiterabbitCalifaReader.cxx
./neuland/R3BWhiterabbitNeulandReader.cxx
./ams/R3BWhiterabbitAmsReader.cxx
./foot/R3BWhiterabbitFootReader.cxx
./wr/R3BWhiterabbitS2Reader.cxx
./wr/R3BWhiterabbitS8Reader.cxx
./los/R3BWhiterabbitLosReader.cxx
./wr/R3BWhiterabbitPspReader.cxx
./music/R3BWhiterabbitMusicReader.cxx
./trloii/R3BTrloiiTpatReader.cxx
./trloii/R3BTrloiiSampReader.cxx
./trloii/R3BTrloiiScalerReader.cxx
./trloii/R3BTrloiiScalerReader_s467.cxx
./wr/R3BTimestampMasterReader.cxx
./pspx/R3BPspxReader.cxx
./pspx/R3BS515PspxReader.cxx
./fibers/R3BBunchedFiberReader.cxx
./fibers/R3BBunchedFiberSPMTTrigReader.cxx
./beammonitor/R3BBeamMonitorReader.cxx
./fibers/R3BFi0Reader.cxx
./fibers/R3BFi1aReader.cxx
./fibers/R3BFi1bReader.cxx
./fibers/R3BFi3aReader.cxx
./fibers/R3BFi3bReader.cxx
./fibers/R3BFi4Reader.cxx
./fibers/R3BFi5Reader.cxx
./fibers/R3BFi6Reader.cxx
./fibers/R3BFi7Reader.cxx
./fibers/R3BFi8Reader.cxx
./fibers/R3BFiberReader.cxx
./los/R3BLosReader.cxx
./rolu/R3BRoluReader.cxx
./rolu/R3BRoluReaderOnline.cxx
./frssci/R3BFrsSciReader.cxx
./sci2/R3BSci2Reader.cxx
./sci8/R3BSci8Reader.cxx
./fibers/R3BSfibReader.cxx
./tofd/R3BTofdReader.cxx
./tofi/R3BTofiReader.cxx
./pdc/R3BPdcReader.cxx
./califa/R3BCalifaFebexReader.cxx
./califa/R3BCalifaJulichReader.cxx
./tofd/R3BPtofReader.cxx
./neuland/R3BNeulandTamexReader.cxx
./neuland/R3BNeulandTacquilaReader.cxx
./stubes/R3BStrawtubesReader.cxx
./ams/R3BAmsReader.cxx
./foot/R3BFootReader.cxx
./foot/R3BFootSiReader.cxx
./alpide/R3BAlpideReader.cxx
./alpide/R3BMosaicReader.cxx
./music/R3BMusicReader.cxx
./rpc/R3BRpcReader.cxx
./twim/R3BTwimReader.cxx
./tttx/R3BTttxReader.cxx
./musli/R3BMusliReader.cxx
./mwpc/R3BMwpcReader.cxx
./sync_check/R3BSyncCheckReader.cxx
)

Set(STRUCT_HEADERS
./base/ext_h101_unpack.h
./wr/ext_h101_whiterabbit.h
./wr/ext_h101_wrmaster.h
./califa/ext_h101_wrcalifa.h
./neuland/ext_h101_wrneuland.h
./ams/ext_h101_wrams.h
./foot/ext_h101_wrfoot.h
./music/ext_h101_wrmusic.h
./wr/ext_h101_timestamp_pspx.h
./wr/ext_h101_wrs2.h
./wr/ext_h101_wrs8.h
./los/ext_h101_wrlos.h
./trloii/ext_h101_tpat.h
./trloii/ext_h101_trlo.h
./trloii/ext_h101_trlo_s467.h
./wr/ext_h101_timestamp_master.h
./beammonitor/ext_h101_bmon.h
./fibers/ext_h101_fib.h
./fibers/ext_h101_fibzero.h
./fibers/ext_h101_fibonea.h
./fibers/ext_h101_fiboneb.h
./fibers/ext_h101_fibthreea.h
./fibers/ext_h101_fibthreeb.h
./fibers/ext_h101_fibfour.h
./fibers/ext_h101_fibfive.h
./fibers/ext_h101_fibsix.h
./fibers/ext_h101_fibseven.h
./fibers/ext_h101_fibeight.h
./fibers/ext_h101_fibten.h
./fibers/ext_h101_fibeleven.h
./fibers/ext_h101_fibtwelve.h
./fibers/ext_h101_fibthirteen.h
./fibers/ext_h101_fib23a.h
./fibers/ext_h101_fib23b.h
./fibers/ext_h101_fib30.h
./fibers/ext_h101_fib31.h
./fibers/ext_h101_fib32.h
./fibers/ext_h101_fib33.h
./los/ext_h101_los_dez19.h
./rolu/ext_h101_rolu.h
./rolu/ext_h101_rolu_online.h
./frssci/ext_h101_frssci.h
./sci2/ext_h101_sci2.h
./sci8/ext_h101_sci8.h
./fibers/ext_h101_sfib.h
./tofd/ext_h101_tofd.h
./tofi/ext_h101_tofi.h
./tofd/ext_h101_ptof.h
./pspx/ext_h101_psp.h
./califa/ext_h101_califa.h
./neuland/ext_h101_raw_nnp_tacquila.h
./neuland/ext_h101_raw_nnp_tamex.h
./stubes/ext_h101_strawtubes.h
./ams/ext_h101_ams.h
./foot/ext_h101_foot.h
./alpide/ext_h101_alpide.h
./alpide/ext_h101_mosaic.h
./music/ext_h101_music.h
./twim/ext_h101_twim.h
./tttx/ext_h101_ttt10.h
./musli/ext_h101_musli.h
./mwpc/ext_h101_mwpc.h
./trloii/ext_h101_samp.h
./trloii/ext_h101_samplos.h
./trloii/ext_h101_sampms.h
./trloii/ext_h101_samplosms.h
./pdc/ext_h101_pdc.h
./rpc/ext_h101_rpc.h
./sync_check/ext_h101_sync_check.h
)

# fill list of header files from list of source files
# by exchanging the file extension
CHANGE_FILE_EXTENSION(*.cxx *.h HEADERS "${SRCS}")
Set(HEADERS ${STRUCT_HEADERS} ${HEADERS})

set(LINKDEF SourceLinkDef.h)
set(DEPENDENCIES
    GeoBase ParBase MbsAPI Base FairTools
    R3BBase R3BData Core Geom GenVector
    Physics Matrix MathCore R3BPsp
    ${ucesb_LIBRARY_SHARED})
set(LIBRARY_NAME R3Bsource)

GENERATE_LIBRARY()
target_include_directories(R3Bsource PUBLIC neuland base base/utils trloii wr ${SYSTEM_INCLUDE_DIRECTORIES})
