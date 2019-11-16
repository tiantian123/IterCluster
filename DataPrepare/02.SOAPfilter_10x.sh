date
SOAPfilter_v2.2 -t 30 -q 33 -p -M 2 -f -1 -Q 10 lane.lst stat.txt 1>log 2>err

perl -e '@A;$n=-1; while(<>){$n++;chomp;@t=split; for($i=0;$i<@t;$i++){$A[$n][$i]=$t[$i]; }} for($i=0;$i<@t;$i++){print "$A[0][$i]\t$A[1][$i]\n";}' stat.txt >stat.csv
date
echo Filter reads done! 
