#!/bin/bash

#SCRIPT_PATH=`dirname $0`

source ./profile
echo "check files ..."

date 
if [[! -f $SPLIT.1.fq.gz || ! -f $SPLIT.1.fq.gz ]]; then
    echo "ERROR: FILE $SPLIT.1.fq.gz or $SPLIT.2.fq.gz is not exists, exit..."
    exit 1;
fi
cp $SCRIPT_PATH/data/lane.lst ./

echo "run SOAPFILTER ... maybe long time ..."
date
tag=`date +_%m_%d_%H_%M_%S`
$SOAP_FILTER -t $THREADS -y -F  CTGTCTCTTATACACATCTTAGGAAGACAAGCACTGACGACATGA -R TCTGCTGAGTCGAGAACGTCTCTGTGAGCCAAGGAGTTGCTCTGG -p -M 2 -f -1 -Q 10 lane.lst stat.txt >SOAPfilter_"$tag".log 2>SOAPfilter_"$tag".err || exit 1

echo "we can choose to filter the barcode which ID is 0 or contain reads less 10 pair and the reads which contain N."

$PERL $SCRIPT_PATH/Script/03.Reads_filter.pl $SPLIT.1.fq.gz.clean.gz $SPLIT.2.fq.gz.clean.gz $SPLIT_FILTER -N 1 -cutoff 10 -L 100 >filter_barcode_"$tag"

echo "filter have done ..."

cp $SCRIPT_PATH/data/lib.list ./

$KmerFreq -k 17 -l lib.list -t $THREADS >17mer.freq >17mer.log &
date

