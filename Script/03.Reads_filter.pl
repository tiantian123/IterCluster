#!/usr/bin/perl
use strict;
use Getopt::Long;
my %opts;

my $usage =<<USAGE;
Function: (1)filter the reads which cotain N;
          (2)filter the barcode which have less than threshold pair reads;

Contact: Chen Tian,<chentian\@genomics.cn>

Version: V.0.1,release 2018-06-14

msage: perl $0 [options] <reads1 file> <reads2 file> <output prefix>
          -L <int> the reads length,default <100>;
          -cutoff <int> the barcode contain min reads pair,default <10>;
          -N <int> the switch of filter N ,0:off,1:on <1>;
          -Re <int>  the switch of reverse compliment the reads2,0:off,1:on <0>;
          -help    output help information to screen
Example: perl $0 reads1.fq reads2.fq hg19 -N 1 -cutoff 10 -L 100 
USAGE
GetOptions(\%opts,"N:i","L:i","cutoff:i","help!");
die $usage if (@ARGV != 3 || defined($opts{"help"}));

my $reads1 = shift;
my $reads2 = shift;
my $prefix = shift;
my $threshold = (exists $opts{cutoff}) ? $opts{cutoff} : 10;
my $len = (exists $opts{L}) ? $opts{L} : 100;
my $N = (exists $opts{N} ? $opts{N} : 1);
my $reverse = (exists $opts{Re}) ? $opts{Re} : 0;

if($reads1 =~ /gz/){
   open (IN1,"gzip -dc $reads1|") or die ("can't open the $reads1!\n"); 
}else{
   open (IN1,"$reads1") or die ("can't open the $reads1!\n");
}
if($reads2 =~ /gz/){
   open (IN2,"gzip -dc $reads2|") or die ("can't open the $reads2!\n");
}else{
    open (IN2,"$reads2") or die ("can't open the $reads2!\n");
}
open (OUT1, "|gzip >$prefix.1.filter.fq.gz") or die ("can't create the file!\n");
open (OUT2, "|gzip >$prefix.2.filter.fq.gz") or die ("can't create the reads2!\n");
open (OUT3,">bar_reads_cnt.txt") or die ("can't create the file!\n");
open (OUT4,">del_bar_ID.txt") or die ("can't create the file!\n");

my $reads_cnt = 0;
my $quality = 'b' x $len;
my @reads1_info;
my @reads2_info;
my @bar_reads_cnt;
my $bar_ID = 0;

while(<IN1>){
   my $r1_seq1 = $_;
   my $r1_seq2 = <IN1>;
   my $r1_seq3 = <IN1>;
   my $r1_seq4 = <IN1>;
   my $r2_seq1 = <IN2>;
   my $r2_seq2 = <IN2>;
   my $r2_seq3 = <IN2>;
   my $r2_seq4 = <IN2>;
   if ($N){
     if ($r1_seq2 =~ /N/ or $r2_seq2 =~ /N/){
         next;
      }
   }
   chomp $r1_seq1;
   chomp $r1_seq2;
   chomp $r2_seq2;
   chomp $r1_seq4;
   chomp $r2_seq4;

   my $seq1 = substr($r1_seq2,0,$len);
   my $Q1 = substr($r1_seq4,0,$len);
   my $seq2 = substr($r2_seq2,0,$len);
   my $Q2 = substr($r2_seq4,0,$len);

   if($reverse){
      $seq2 = reverse($seq2);
      $Q2 = $quality;
      $Q1 = $quality;
      $seq2 =~ tr/ACGTacgt/TGCAtgca/;
   }
   my @line = split(/\t|\//,$r1_seq1);
   next if ($line[2] == 0);
   if($bar_ID == $line[2]){
      $reads_cnt ++;
      push(@reads1_info,[$line[0],$seq1]);
      push(@reads2_info,[$line[0],$seq2]);
   }else{
     print OUT3 "$bar_ID\t$reads_cnt\n";
     if ($reads_cnt > $threshold){
         foreach my $index(0..$#reads1_info){
             print OUT1 "$reads1_info[$index]->[0]/1\t$bar_ID\t1\n$reads1_info[$index]->[1]\n+\n$Q1\n";
             print OUT2 "$reads2_info[$index]->[0]/2\t$bar_ID\t1\n$reads2_info[$index]->[1]\n+\n$Q2\n";
         }
     }else{
         print OUT4 "$bar_ID\t$reads_cnt\n";
     }
     @reads1_info = ();
     @reads2_info = ();
     push(@reads1_info,[$line[0],$seq1]);
     push(@reads2_info,[$line[0],$seq2]);
     $bar_ID = $line[2];
     $reads_cnt = 1; 
   }
}


close IN1;
close IN2;
close OUT1;
close OUT2;
close OUT3;
close OUT4;
