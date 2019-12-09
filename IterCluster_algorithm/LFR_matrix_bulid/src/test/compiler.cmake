set(testDir ${PROJECT_SOURCE_DIR}/test)
include_directories(${testDir})

set(test_cpp 
    ${testDir}/UniqueKmerArray_test.cpp
    ${testDir}/MultiThread_test.cpp
    ${testDir}/ArrayHash_test.cpp
    ${testDir}/KmerHash_test.cpp
    ${testDir}/SeqReader_test.cpp
    ${testDir}/LFRReader_test.cpp
    ${testDir}/BQ2KmerWR_test.cpp
    ${testDir}/KmerBuffer_test.cpp
    )
