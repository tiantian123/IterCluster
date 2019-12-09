#!/bin/bash

source ./profile

# start to build matrix
$SCRIPT_PATH/IterCluster/LFR-matrix-build/bin/LFR-matrix-build  All  -o tmp -f  -i 20 -a 60 -t $THREADS -k 17 -p $SPLIT_FILTER.1.filter.fq.gz  -e $SPLIT_FILTER.2.filter.fq.gz  2>cerrfile 1>coutfile

echo "matrix have been built."

pgiz -d -p $THREADS  tmp.barcode_matrix.gz >tmp.barcode_matrix
echo "gunzip have done!"
date

# start to Cluster
$SCRIPT_PATH/IterCluster-MCL/IterCluster-MCL -m tmp.barcode_matrix  -i tmp.barcode_info -b tmp.barcode2kmer -c 60 -k 2 -n 20000 -r 0.3 -g 100 -f 3 -t $THREADS -a 2 -h 12 -s 1 -d 1  1>cout.log 2>cerr.log

mv seed_tarbarcode.txt ./Evaluate
rm -f optput*

echo "IterCluster have done."
date
