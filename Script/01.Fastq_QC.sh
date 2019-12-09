date
r1=/zfssz3/MGI_ALGORITHM/assembly/chentian/01.LFR_Cluster/ZY20171116Lib9-G50MHLEESUG11C/01.reads_file/Read1.fq.gz.clean.gz 
r2=/zfssz3/MGI_ALGORITHM/assembly/chentian/01.LFR_Cluster/ZY20171116Lib9-G50MHLEESUG11C/01.reads_file/Read2.fq.gz.clean.gz
time fastp -i $r1 -o reads1.fp -I $r2 -O reads2.fp -w 60 -A -h stLFR.fp.html 2>stLFR.qc.log &
date
echo 'have done'
