#!/bin/bash

source ./profile

for var in 0 5e4 8e4 1e5 1.5e5 2e5 2.5e5 3e5 3.5e5 4e5
do
$PERL $SCRIPT_PATH/Script/07.Single_MclIterCuster_evaluate.pl ./Evaluate/seed_tarbarcode.txt ./Evaluate/$PROJECT_NAME.barcode_info ./tmp.barcode_info  500 30 5 $var 0 
done

mv single_evaluate_*.txt ./Evaluate/
echo "have evaluated the Cluster."
date
