data: 2018-10-08

author: Jiancong Weng

IterCluster is a barcode cluster algorithm for long fragment data.

IterCluster contain 3 module: IterCluster, matrix-bulid and FalseRemove. Three module can be compile and run sepatately.

Requirement:
1. GCC version 5.2.0
2. cmake version 3.9
Please make sure gcc、g++ and cmake are under /usr/bin/. . 

Here is a introduction to install cmake:
1. wget https://github.com/Kitware/CMake/releases/download/v3.14.3/cmake-3.14.3-Linux-x86_64.tar.gz
2. tar zxvf cmake-3.14.3-Linux-x86_64.tar.gz
3. cd cmake-3.14.3-Linux-x86_64/bin
4. ln -s cmake /usr/bin/cmake

1. LFR_matrix_build

Install:
cd matrix-bulid/src
sh make.sh

Usage of parameter:
    -p first fastq file.
    -e second fastq file.
    -o output filename prefix.
    -i min frequency used for select unique kmer.
    -a max frequency used for select unique kmer.
    -k [option] k value. default is 17.
    -t [option] cpu number. default is 8.
    -f [option] set means use Array instead of Hash.
    -z [option] kmer number ,used for init hash table, default is 10000000

Example:
./LFR-matrix-build -f -r 0 -o tmp -i 20 -a 80 -t 8 -k 17 -p Read1.fq -e Read2.fq -z 1000000

output:
tmp.barcode_matrix, tmp.barcode_info, tmp.barcode2kmer
The main relation matrix is tmp.barcode_matrix

2. IterCluster-MCL

Install:
cd IterCluster
sh build.sh

Usage of parameter:
-m      the input of relation matrix[path]
-i      the input of barcode information index[path]
-b      the input of barcode to kmer index[path]
-c      the minimum co-unique kmer in the relation matrix[int]
-k      the iteration time for each seed when cluster[int]
-n      the minimum unique kmer number of the seed[int]
-r      the minimum unique kmer rate of the seed[double]
-g      the minimum co-unique kmer between seed and target barcode[int]
-f      the minimum frequency of target unique kmer[int]
-t      the thread number[int]
-a      the unique kmer frequency increment for each[int]
-h      the iteration time of MCL[int]
-s      whether to use frequency select model
-d      whether to debug

Example:
IterCluster-MCL -m tmp.barcode_matrix -i tmp.barcode_info -b tmp.barcode2kmer -h 12 -c 10 -k 4 -n 1500 -r 0.3 -g 100 -f 3 -a 2 -t 50 -s 1 -d 1 1>cout.log 2>cerr.log

Output:
The main cluster result is seed_tarbarcode.txt

3. FalseReove

Install:
cd FalseRemove
sh build.sh

Usage of parameter:
Usage:
-f      input of read1.fq[path]
-q      input of read2.fq[path]
-s      input of cluster set[path]
-k      the k size of k-mer[int]
-c      min average k-mer depth of each read. cut-off.[int]
-d      average sequence depth[int]
-t      thread number[int]

Example:
FalseRemove -f Read1.fq.gz -q Read2.fq.gz -s seed_tarbarcode.txt -k 17 -c 8 1>cout.log 2>cerr.log

Output:
output.read1, output.read2
The main result is two fasta formate file, the quality of reads have been lost after FalseRemove.

4. Datesets used in paper:
Datesets1(BGI's stLFR):https://db.cngb.org/cnsa/sample/CNS0007594/public/
Datesets2(10X Genomics data):https://support.10xgenomics.com/de-novo-assembly/datasets/2.1.0/hgp
