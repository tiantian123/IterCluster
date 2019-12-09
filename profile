#
# basic settings below
# MODIFY HERE FOR YOU PROJECT !!!
#
r1="L1.1.fq.gz L2.1.fq.gz"  # stLFR raw read1. use " " to seperate differnt lanes.
r2="L1.2.fq.gz L2.2.fq.gz"  # stLFR raw read2. use " " to seperate differnt lanes.
USE_FILTER="yes"            # yes or no. # use SOAPFilter or not
BARCODE_FREQ_THRESHOLD=2    # if the number of read pairs sharing the same barcode is smaller then BARCODE_FREQ_THRESHOLD, then discard the barcode.

# below are basic parameters for IterCLuster.
PROJECT_NAME="stLFR"
THREADS=60
ReadLen=100
REF= '/ldfssz1/MGI_ALGORITHM/assembly/chentian/data/hg19.fasta' # the reference for alignment to build true set.

# "SCRIPT_PATH" is YOUR-INSTALL-DIR directory
SCRIPT_PATH='/ldfssz1/MGI_ALGORITHM/assembly/chentian/IterCluster_code/'
#"SOAP_FILTER is the executable path of SOAPFilter
SOAP_FILTER="/hwfssz1/ST_OCEAN/USER/guolidong/stLFR/data_pipeline/SOAPfilter_v2.2.1/SOAPfilter_v2.2"
# "BWA" is the excutable path of bwa.
BWA='/home/tangjb/BIN/bwa'
# "PERL" is the excutable path of perl.
PERL='/bin/perl'
# "KmerFreq" is the excutable path of KmerFreq.
KmerFreq='/ldfssz1/MGI_ALGORITHM/assembly/chentian/Software/mysoft/kmerfreq_v5.0/kmerfreq'
# "FASTP" is the excutable path of Fastp for QC.
FASTP='~/bin/fastp'

#
#   intermediate files which will be generated/needed below.
#         DO NOT MODIFY BELOW
#   UNLESS YOU KNOW WHAT YOU ARE DOING.
#

R1="tmp_r1.fq.gz"                   # the symbol-link concatenating different lanes.
R2="tmp_r2.fq.gz"
SPLIT="split_reads"                 # the prefix of splitted reads. split_reads.1.fq.gz & split_reads.2.fq.gz
SPLIT_FILTER="split"                # the prefix of splitted and reads. split.1.filter.fq.gz & split.2.filter.fq.gz
