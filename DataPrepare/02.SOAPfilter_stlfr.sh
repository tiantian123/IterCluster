date

SOAPfilter_v2.2 -t 30 -q 33 -y -F CTGTCTCTTATACACATCTTAGGAAGACAAGCACTGACGACATGATCACCAAGGATCGCCATAGTCCATGCTAAAGGACGTCAGGAAGGGCGATCTCAGG -R TCTGCTGAGTCGAGAACGTCTCTGTGAGCCAAGGAGTTGCTCTGGCGACGGCCACGAAGCTAACAGCCAATCTGCGTAACAGCCAAACCTGAGATCGCCC -p -M 2 -f -1 -Q 10 lane.lst stat.txt 1>log 2>err

perl -e '@A;$n=-1; while(<>){$n++;chomp;@t=split; for($i=0;$i<@t;$i++){$A[$n][$i]=$t[$i]; }} for($i=0;$i<@t;$i++){print "$A[0][$i]\t$A[1][$i]\n";}' stat.txt >stat.csv

date
#perl -e '$n=0;open IN,"gzip -dc Read1.fq.gz.clean.gz |"; open OUT,"| gzip > Zebra_Read1.fq.gz"; while(<IN>){$n++; print OUT "$_"; if($n == 48854740*4){last;}} close(IN);close(OUT);' &
#perl -e '$n=0;open IN,"gzip -dc Read2.fq.gz.clean.gz |"; open OUT,"| gzip > Zebra_Read2.fq.gz"; while(<IN>){$n++; print OUT "$_"; if($n == 48854740*4){last;}} close(IN);close(OUT);' &

wait
date
echo Filter reads done!
