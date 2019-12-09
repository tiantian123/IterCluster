#!/bin/bash

#SCRIPT_PATH=`dirname $0`

echo "check profile ..."
date
source ./profile
lane1_num= `ls -all $r1 | wc -l`
lane2_num= `ls -all $r2 | wc -l`
if [[$lane1_num != $lane2_num ]]; then
    echo "ERROR: Profile error. Please make sure all file is correct !!!!"
    exit 1
fi
echo "generate $R1 and $R2 ...."
date
if [[$lane1_num -lt 1]]; then
    echo "ERROR: File not exist: $r1. Exit ..."
    exit 1
elif [[ $lane1_num -eq 1]]; then
    echo "Only 1 lane detect in $r1. "
    ln -s $r1 $R1;
    ln -s $r2 $R2;
else
    echo "$lane1_num lanes detect in $r1 ."
    echo "Start cat all files into tm_r1.fq.gz ..."
    cat $r1 > $R1;
    cat $r2 > $R2;
    echo "Cat end ... "
fi

echo "split barcode ..."
date
tag=`data +_%m_%d_%H_%M_%S`
# split barcode and sort.
echo "maybe need a long time."
$PERL $SCRIPT_PATH/Script/00.split_barcode_PEXXX_42_reads_stlfr.pl $SCRIPT_PATH/data/barcode.list _$SCRIPT_PATH/data/barcode_RC.txt  $R1 $R2 $ReadLen $SPLIT
echo "split barcode have been done, start to analysis the QC..."
$FASTP -i $SPLIT.1.fq.gz -o reads1.fp -I $SPLIT.2.fq.gz -O reads2.fp -w 60 -A -h stLFR.fp.html 2>stLFR.qc.log
echo "QC analysis have been done."
date
