AUX_SOURCE_DIRECTORY(. DIR_TEST1_SRCS)
include_directories( . ./eigen-eigen-b9cd8366d4e8/)
ADD_LIBRARY ( Test ${DIR_TEST1_SRCS})
#add_definitions(-std=c++11)
