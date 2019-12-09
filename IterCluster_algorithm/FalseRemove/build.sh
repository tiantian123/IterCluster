#!/bin/bash

CC=/usr/bin/gcc CXX=/usr/bin/g++ /usr/bin/cmake .
#-DBUILD_SHARED_LIBS=ON -DCGATOOLS_IS_PIPELINE=ON -DBOOST_ROOT=/ldfssz1/MGI_ALGORITHM/assembly/huangweihua/bin/boost_1_46_1 -DCMAKE_BUILD_TYPE=Debug  ..

if [ -f ./Makefile ]; then
    make -j8
else
    echo "Errors executing CMake. Fix errors and run again."
fi

mv main FalseRemove
