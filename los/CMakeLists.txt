# Create a library called "libR3BCal" which includes the source files given in
# the array .
# The extension is already found.  Any number of sources could be listed here.

set(INCLUDE_DIRECTORIES
${ROOT_INCLUDE_DIR} 
#put here all directories where header files are located
${BASE_INCLUDE_DIRECTORIES}
${R3BROOT_SOURCE_DIR}/r3bbase 
${R3BROOT_SOURCE_DIR}/passive
${R3BROOT_SOURCE_DIR}/mcstack 
${R3BROOT_SOURCE_DIR}/los
${R3BROOT_SOURCE_DIR}/los/unpack
${R3BROOT_SOURCE_DIR}/r3bdata/
${R3BROOT_SOURCE_DIR}/r3bdata/calData
${R3BROOT_SOURCE_DIR}/r3bdata/dchData
${R3BROOT_SOURCE_DIR}/r3bdata/gfiData
${R3BROOT_SOURCE_DIR}/r3bdata/mtofData
${R3BROOT_SOURCE_DIR}/r3bdata/tofData
${R3BROOT_SOURCE_DIR}/r3bdata/landData
${R3BROOT_SOURCE_DIR}/r3bdata/trackerData  )

include_directories( ${INCLUDE_DIRECTORIES})

set(LINK_DIRECTORIES
${ROOT_LIBRARY_DIR}
${FAIRROOT_LIBRARY_DIR}
) 

link_directories( ${LINK_DIRECTORIES})

set(SRCS
unpack/R3BLosUnpack.cxx
unpack/R3BLosRawHit.cxx)

# fill list of header files from list of source files
# by exchanging the file extension
CHANGE_FILE_EXTENSION(*.cxx *.h HEADERS "${SRCS}")

Set(LINKDEF LosLinkDef.h)
Set(LIBRARY_NAME R3BLos)
Set(DEPENDENCIES
    R3Bbase R3BData R3BPassive MCStack)

GENERATE_LIBRARY()
