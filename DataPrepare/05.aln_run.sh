date
bwa mem -t 60  hg19.fasta 10X.1.filter.fq.gz 10X.2.filter.fq.gz >/zfssz3/MGI_ALGORITHM/assembly/chentian/01.LFR_Cluster/10x/03.aln_file/10x.hg19.aln.sam
echo "aligment have done"
date
perl -e 'while(<>){chomp; @t=split;next if((/^@/) || $t[3]=~/\*/ || $t[3] == $t[7] || $t[1]>2048 || $t[8]==0);print "$_\n";} close(IN);' /zfssz3/MGI_ALGORITHM/assembly/chentian/01.LFR_Cluster/10x/03.aln_file/10x.hg19.aln.sam >/zfssz3/MGI_ALGORITHM/assembly/chentian/01.LFR_Cluster/10x/03.aln_file/10x.hg19.aln.filter.sam
echo "filter have done"
date
