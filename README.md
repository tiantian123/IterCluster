# IterCluster

## <a name=intro>Introduction</a>

- The IterCluster is a barcode clustering algorithm fragment read analysis. There is pipeline for stLFR raw reads using the IterCluster.

*Written by Perl, Shell and C++ languages.*
*Need to campile and install the IterCluster in addition.*

### <a name=files>Structure of the files</a>
```
├── README.md
├── Script
│   ├── 00.split_barcode_PEXXX_42_reads_stlfr.pl # splits raw reads, get barcode sequences.
│   ├── 01.Fastq_QC.sh                           # use Fastp for QC with the raw reads after split, detect the adapter,duplicate etc.
│   ├── 02.SOAPfilter_10x.sh                     # filters duplication & adaptors(for 10x data).
│   ├── 02.SOAPfilter_stlfr.sh                   # filters duplication & adaptors(for stLFR data).
│   ├── 03.Reads_filter.pl                       # filters the small barcode which contian less than 10 pair reads and reads which contains N.
│   ├── 04.kmer.sh                               # kmer analysis.
│   ├── 05.aln_run.sh                            # alignment the clean reads to the refrence.
│   ├── 06.get_barcode_position.pl               # according the aligment results to find the barcode true region.
│   └── 07.Single_MclIterCuster_evaluate.pl      # compare the Cluster result with the true barcode region, evaluate the effect.
├── data
│   ├── barcode.list                             # barcode list of stLFR.
│   ├── barcode_RC.list                          # reverse complementary barcode list of stLFR.
│   ├── lane.lst                                 # config for SOAPfilter.
│   └── lib.list                                 # config for KmerFreq.
├── IterCluster                                
│   ├── README                                   # the Install approach.
│   ├── LFR_matrix_bulid                         # build the relation matrix.
│   ├── IterCluster-MCL                          # cluster
│   └──  FalseRemove                             # remove the false positive reads.
├── profile                                      # default profile.
├── run.sh                                       # 1-step executable script, which calls all four substeps 0 ~ 3.
├── step_0_split_barcode.sh                      # calls 00.split_barcode...pl,Fastp and generate split_reads.[1,2].fq.gz & split_reads.log & stLFR.qc.log
├── step_1_filter.sh                             # calls 02.SOAPfilter..pl,03.Reads_filter.pl,04.kmer.sh and generate split.*.filter.fq.gz
├── step_2_Aln_bar_pos.sh                        # calls 05.aln_run.sh,06.get_barcode...pl and generate the *.filter.sam, *.barcode_Info.
├── step_3_build_matrix_IterCluster.sh           # calls IterCluster.
└── step_4_evaluate.sh                           # calls 07.Single_...evaluate.pl to evaluate the IterCluster.
```
### <a name=usage>General usage</a>

- 1st, create a new work folder

```
> makdir YourProjectRoot
```

- 2nd, copy the default profile into your work folder

```
> cd YourProjectRoot
> cp YOUR-INSTALL-DIR/profile ./your_own_profile
```
   **Do not change the filename. Always use "profile".**

- 3rd, edit your_own_profile

```
> vi ./your_own_profile
```
- 4th, run the 1-step executable script with your profile or run it step by step

    - 4.1 run the 1-step executable script
```
> YOUR-INSTALL-DIR/run.sh  # make sure your run this command in YourProjectRoot
```
-   - 4.2 run the pipeline step by step or just run what you want

```
> YOUR-INSTALL-DIR/step_x_xxxx.sh # make sure your run this command in YourProjectRoot
```

### <a name=profile>The profile file</a>

**Do not change the filename. Always use "profile".**

**Make sure the profile file is in YourProjectRoot directory.**

## <a name=contact>Contact</a>

- please contact chentian@genomics.cn/wengjiancng@hotmail.com
- or please creat an issue in github.

## <a name=appendix>Appendix</a>

- Split barcode

```
>perl 00.split_barcode_PEXXX_42_reads_stlfr.pl
Example: perl Split_SingleTube_reads.1.pl barcode.list barcode_RC.list read_1.fq.gz read_2.fq.gz 100 split_read

output: split_read.*.fq.gz

```

- Filter reads

```
>perl 03.Reads_filter.pl
Function: (1)filter the reads which cotain N;
          (2)filter the barcode which have less than threshold pair reads;

Contact: Chen Tian,<chentian@genomics.cn>

Version: V.0.1,release 2018-06-14

msage: perl 03.Reads_filter.pl [options] <reads1 file> <reads2 file> <output prefix>
          -L <int> the reads length,default <100>;
          -cutoff <int> the barcode contain min reads pair,default <10>;
          -N <int> the switch of filter N ,0:off,1:on <1>;
          -Re <int>  the switch of reverse compliment the reads2,0:off,1:on <0>;
          -help    output help information to screen

Example: perl 03.Reads_filter.pl reads1.fq reads2.fq hg19 -N 1 -cutoff 10 -L 100

Output: split.*.filter.fq.gz
```

- Build True barcode postion and Cluster set

```
>perl 06.get_barcode_position.pl
version: v3,2018-05-24
example: perl 06.get_barcode_position.pl <aln_file file> <split_log file> <prefix, string> <distance between LFR, int>

output: prefix.barcode_info, such as

bar_ID  bar_strand      start_Pos       end_Pos lenth   center
1       chr6.0  107046807       107086865       40058   107066836       34
1       chr10.0 83106037        83108293        2256    83107165        6
1       chr9.0  115014407       120179898       5165491 117597152.5     108
1       chr14.0 93730062        93779746        49684   93754904        62
2       chr7.0  62776725        62872452        95727   62824588.5      18
2       chr7.1  158253815       158253912       97      158253863.5     2
2       chr14.0 20693692        20780483        86791   20737087.5      150
2       chrY.0  13466866        13466877        11      13466871.5      2
2       chrY.1  58843761        58900672        56911   58872216.5      4
...

```

- Evaluate the Cluster result

```
>perl 07.Single_MclIterCuster_evaluate.pl
version: v6, 2018-07-03
example: perl 07.Single_MclIterCuster_evaluate.pl  <seed_capture_result file> <bar_pos file> <seed-info file>  <LFR low len> <reads_low_cnt> <target read cnt> <expand_len> <real_bar_thr>

output: single_evaluate_*.txt,such as

seed_id LFR  unique_kme kmer_cnt   rate   correct_cnt  real_cnt   cap_cnt   ppv  fdr  fnr
15554810        chr17_2.0       3097    8702    0.36    1226    1684    1255    0.98    0.02    0.27
15656801        chr5_5.0        3082    6432    0.48    1077    1510    1103    0.98    0.02    0.29
15808007        chr19_3.0       3206    8207    0.39    1078    1536    1107    0.97    0.03    0.30
15902418        chr4_11.0       3004    7151    0.42    1039    1473    1069    0.97    0.03    0.30
15627951        chr10_1.0       3018    7041    0.43    1185    1592    1215    0.98    0.02    0.26
15639511        chr15_9.0       3229    7848    0.41    1185    1540    1217    0.97    0.03    0.23
16111127        chr1_11.0       3070    7426    0.41    1215    1572    1231    0.99    0.01    0.23
16311767        chr5_4.0        3150    6742    0.47    1088    1323    1131    0.96    0.04    0.18
...

```
