#!/bin/bash

source ./profile

if [[! -f $SPLIT_FILTER.1.filter.fq.gz || $SPLIT_FILTER.2.filter.fq.gz]]; then
    echo "ERROR : split.*.filter.fq.gz dosen't exists, Exit..."
    exit 1
fi

mkdir Evaluate

$BWA mem -t $THREADS $REF $SPLIT_FILTER.1.filter.fq.gz $SPLIT_FILTER.2.filter.fq.gz >./Evaluate/$PROJECT_NAME.aln.sam 

echo "aligment have done"
date

$PERL -e 'while(<>){chomp; @t=split;next if((/^@/) || $t[3]=~/\*/ || $t[3] == $t[7] || $t[1]>2048 || $t[8]==0);print "$_\n";} close(IN);' ./Evaluate/$PROJECT_NAME.aln.sam >./Evaluate/$PROJECT_NAME.aln.filter.sam
echo "filter bad aligment have done"
date

# Find barcode true position, the region contain true barcode set. Assume the distance between is 2e7bp.
$PERL $SCRIPT_PATH/Script/06.get_barcode_position.pl ./Evaluate/$PROJECT_NAME.aln.filter.sam split_reads.log $PROJECT_NAME 2e7

mv $PROJECT_NAME.barcode_info ./Evaluate/

echo "True barcode set have been built."
date
