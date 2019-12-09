set(ThirdPartyDir ${PROJECT_SOURCE_DIR}/ThirdParty)
include_directories(${ThirdPartyDir})

set(source_cpp ${source_cpp} 
    ${ThirdPartyDir}/gzstream.cpp
    ${ThirdPartyDir}/SeqReader.cpp
    ${ThirdPartyDir}/Util.cpp
    )
