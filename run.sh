#!/bin/bash

source ./profile

if [[ ! -f _step_0_end.txt ]] ; then
    echo "---------------------Step 0 start ---"
    # run step 0
    echo `date` >>_step_0_start.txt
    $SCRIPT_PATH/step_0_split_barcode.sh || exit 1
    echo `date` >>_step_0_end.txt
    echo "---------------------Step 0 end    ---"
fi

if [[ $USE_FILTER == "yes" ]] ; then
    if [[ ! -f _step_1_end.txt ]] ; then
    echo "---------------------Step 1 start ---"
        # run step 1
        echo `date` >>_step_1_start.txt
        $SCRIPT_PATH/step_1_filter.sh || exit 1
        echo `date` >>_step_1_end.txt
    echo "---------------------Step 1 end    ---"
    fi
else
    echo "--------------------- No filter detect"
if [[ ! -f _step_2_end.txt ]] ; then
    # run step 2
    echo "---------------------Step 2 start ---"
    echo `date` >>_step_2_start.txt
    $SCRIPT_PATH/step_2_Aln_bar_pos.sh || exit 1
    echo `date` >>_step_2_end.txt
    echo "---------------------Step 2 end    ---"
fi

if [[ ! -f _step_3_end.txt ]] ; then
    # run step 2
    echo "---------------------Step 3 start ---"
    echo `date` >>_step_3_start.txt
    $SCRIPT_PATH/step_3_build_matrix_IterCluster.sh || exit 1
    echo `date` >>_step_3_end.txt
    echo "---------------------Step 3 end    ---"
fi

if [[ ! -f _step_4_end.txt ]] ; then
    # run step 2
    echo "---------------------Step 3 start ---"
    echo `date` >>_step_4_start.txt
    $SCRIPT_PATH/step_4_evaluate.sh || exit 1
    echo `date` >>_step_4_end.txt
    echo "---------------------Step 3 end    ---"
fi
