date
Reads=HLCJTCCXX

longranger basic  --id=Split_barcode --fastqs=$Reads --lanes=5,6 --indices=CCTGGAGA,TTCACGCG --localcores=20 --localmem=55

date
perl 01.split_10X_reads_new.pl barcoded.fastq.gz  151 10X

perl -e 'while(<>){chomp;@t=split; $base=$t[2]*(128+151)/1000000; print "$base\n"; }' 10X.list >BaseSize.stat.plot

date
echo done!
