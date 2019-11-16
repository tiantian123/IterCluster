#!/usr/bin/perl
use strict;
my $usage=<<USAGE;
version: v2, 2018/5/16, chentian\@genomics.cn
example: perl $0 <seed_capture file> <barcode->read_pair_count>
USAGE
die $usage if(@ARGV != 2);

my $file1 = shift;
my $file2 = shift;

open(IN1,"$file1" ) or die ("can't open the $file1!\n");
open(IN2, "$file2") or die ("can't open the $file2!\n");
open(OUT, ">loss_barcode") or die ("can't crate the file!\n");

my %barcode_hash;
my $total_reads = 0;
my $capture_reads = 0;

while(<IN2>){
  chomp;
  my @line = split;
  $barcode_hash{$line[0]} = $line[1]; 
  $total_reads += $line[1];
}
my $bar_num = keys %barcode_hash;
print "repeat barcode in Cluster set:\n";
while(<IN1>){
   chomp;
   my @line = split;
   foreach(@line){
     if(exists($barcode_hash{$_})){
        $capture_reads += $barcode_hash{$_};
        delete $barcode_hash{$_};
     }else{
       print "$_  ";
     }
   }
}
print "\n";
my $lost_reads = $total_reads - $capture_reads;
print "lost_reads:$lost_reads\n";

my $loss_reads = 0;
my $loss_bar = keys %barcode_hash;

while(my ($key,$value) = each %barcode_hash){
     print  OUT "$key ";
     $loss_reads += $value;  
}

print OUT "\n";
print  OUT "total barcode number: $bar_num\n";
print  OUT "Cluster loss barcode number: $loss_bar\n";
print OUT "Total reads number: $total_reads\n";
print OUT "Cluster lost reads number:$loss_reads\n";

my $loss_rate = $loss_bar/$bar_num;
my $loss_reads_rate = $loss_reads/$total_reads;
print OUT  "Cluster barcode loss rate: $loss_rate\n";
print OUT "Cluster reads loss rate: $loss_reads_rate\n";

close IN1;
close IN2;
close OUT;
