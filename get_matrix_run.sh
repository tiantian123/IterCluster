/ldfssz1/MGI_ALGORITHM/assembly/chentian/04.LFR_cluster_survey/LFR-final/bin/LFR-matrix-build  All  -o tmp -f  -i 20 -a 80 -t 16 -k 17 -p hg19.reads1.filter.fq  -e hg19.reads2.filter.fq  2>cerrfile 1>coutfile
#/ldfssz1/MGI_ALGORITHM/assembly/chentian/04.LFR_cluster_survey/LFR-final/bin/LFR-matrix-build BarcodeMatrix -t 8 -o tmp -u 983483366 -i 20 -a 80 2>M_cerrfile 1>M_coutfile
echo "have get matrix"
date
gunzip -c tmp.barcode_matrix.gz >tmp_barcode_matrix 
echo "gunzip have done!"
date
