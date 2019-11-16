#!/usr/bin/perl
use strict;
my $usage =<<USAGE;
version: v3, 2018-07-12
examp: perl $0  <seed_capture_result file> <bar_pos file> <seed-info file> <LFR low len> <reads_low_cnt> <target reads cnt> <expand_len> <real_bar_thr,int>
USAGE
die $usage if(@ARGV != 8);

my $seed_file = shift;
my $bar_file = shift;
my $bar_info_file = shift;
my $LFR_low_len = shift;
my $reads_low_cnt = shift;
my $target_reads_low = shift;
my $expand_len = shift;
my $real_bar_thr = shift;

open (IN,$seed_file) or die ("can't open the $seed_file!\n");
open (IN1,$bar_file) or die ("can't open the $bar_file!\n");
open (IN2,$bar_info_file) or die ("can't open the $bar_info_file!\n");
open (OUT,">evaluate_$expand_len.txt") or die("can't create the file!\n");

my $seed_cnt = 0;
my %bar_info;
my %bar_pos;

while(<IN1>){
  chomp;
  next if(!/\d+/);
  my @line = split;
  $bar_info{$line[0]}{$line[1]} = $_;
  my @chr = split(/\./,$line[1]);
#  print "$chr[0]\t$chr[1]\t$line[0]\n";
  next if ($line[4] <= $LFR_low_len || $line[6] <= $target_reads_low);
  $bar_pos{$chr[0]}{$chr[1]}{$line[0]}{'max'} = $line[3];
  $bar_pos{$chr[0]}{$chr[1]}{$line[0]}{'min'} = $line[2];
}
my %seed_info;

while(<IN2>){
  chomp;
  my @line = split;
  $seed_info{$line[0]}{'unique_cnt'} = $line[1];
  $seed_info{$line[0]}{'kmer_cnt'} = $line[2];
  my $rate = $line[1] / $line[2];
  $rate = sprintf "%.2f", $rate;
  $seed_info{$line[0]}{'rate'} = $rate;
}
print OUT "seed_id  unique_kme kmer_cnt   rate   correct_cnt  real_cnt   cap_cnt   ppv  fdr  fnr\n";

while(<IN>){
  chomp;
 # last if($seed_cnt > $iter);
  my @line = split;
  if (exists($bar_info{$line[0]})){         #search seed barcode info
  #   print "$line[0]:\n";
     my $bar_strand = $bar_info{$line[0]};
     my @True_well = ();
     my @True_well_sub = ();
     while (my ($key,$value) = each %$bar_strand){
         my @bar = split(/\t/,$value);
         if ($bar[4] > $LFR_low_len && $bar[6] >= $reads_low_cnt){
           @True_well_sub = evaluate($bar[0],$bar[1],$bar[2],$bar[3],$expand_len);
           my $set_len = @True_well_sub;
 #          print "$set_len\n";
           if ($set_len > $real_bar_thr ){
               push (@True_well,@True_well_sub);
           }
         }
     }
     my @unique_True = removeRepeat(\@True_well);
    # my @unique_cap = removeRepeat(\@line);
     my %hash_True = map{$_=>1} @unique_True;
     my %hash_cap = map{$_=>1} @line;
     my @common = grep {$hash_True{$_}} @line;
=pod
     my @union = ();
     my @insect = ();
     my %union = {};
     my %insect = {};
     foreach my $e (@True_well,@line){
        $union{$e}++ && $insect{$e}++;
     }
     @union = keys %union;
     @insect = keys %insect;
=cut
     my $real_cnt = @True_well;
     my $cap_cnt = @line;
     my $correct_cnt = @common;
     my $ppv = $correct_cnt / $cap_cnt;
     $ppv = sprintf "%.2f",$ppv;
     my $fnr = 1 - $correct_cnt / ($real_cnt + 1);
     $fnr = sprintf "%.2f",$fnr;
     my $fdr = 1 - $ppv;
     $fdr = sprintf "%.2f",$fdr;
     print OUT "$line[0]\t$seed_info{$line[0]}{'unique_cnt'}\t$seed_info{$line[0]}{'kmer_cnt'}\t$seed_info{$line[0]}{'rate'}\t$correct_cnt\t$real_cnt\t$cap_cnt\t$ppv\t$fdr\t$fnr\n";
  }
 $seed_cnt ++;   
}

sub evaluate{
 my ($id,$ctg,$left,$right,$len) = @_;
 #print "$id\t$ctg\t$center\t$len\n";
 my $start = $left - $len;
 my $end = $right + $len;
 if($start < 0){
   $start = 0;
 }
 my @real_wellId;
 my @chr = split(/\./,$ctg);
 print "$chr[0]\t";
 #find real wellId between in target seed barcode.
 my $new_hash = $bar_pos{$chr[0]};
 while (my ($key,$value) = each %$new_hash){
    #print "$key\t$value\n";
    while(my($k1,$v1) = each %$value){
#          print "$key\t$k1\n";
         if((($bar_pos{$chr[0]}{$key}{$k1}{'min'} > $start) && ($bar_pos{$chr[0]}{$key}{$k1}{'min'} < $end)) || (($bar_pos{$chr[0]}{$key}{$k1}{'max'} > $start) && ($bar_pos{$chr[0]}{$key}{$k1}{'max'} < $end))){ 
             # print "$key\t";
             push (@real_wellId,$k1);
         }
     }
  }
 # print "\n";
  return @real_wellId;
=pod
 #calculate the TP,FP,Acc.
 my @union = ();
 my @insect = ();
 my %union;
 my %insect;
 foreach my $e(@real_wellId,@cap_result){
    $union{$e}++ && $insect{$e}++;   
 }
 @union = keys %union;
 @insect = keys %insect;
 my $insect_cnt = @insect;
 my $ppv = @insect / @cap_result;
 $ppv = sprintf "%.2f",$ppv;
 my $fnr = 1 - @insect / (@real_wellId + 1);
 $fnr = sprintf "%.2f",$fnr;
 my $fdr = 1 - $ppv;
 $fdr = sprintf "%.2f",$fdr;
 my $real_cnt = @real_wellId;
 my $cap_cnt = @cap_result;
 my $correct_cnt = @insect;
 print OUT "$id\t$seed_info{$id}{'unique_cnt'}\t$seed_info{$id}{'kmer_cnt'}\t$seed_info{$id}{'rate'}\t$ctg\t$start\t$end\t$LFR_len\t$correct_cnt\t$real_cnt\t$cap_cnt\t$ppv\t$fdr\t$fnr\n";
=cut
}

sub removeRepeat{
  my $arrRef = shift;
  my %count = ();
  my @uniArr = grep {++$count{$_} == 1} @$arrRef;
  return @uniArr;
}

close IN;
close IN1;

