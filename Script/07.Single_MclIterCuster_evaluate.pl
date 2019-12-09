#!/usr/bin/perl
use strict;
my $usage =<<USAGE;
version: v6, 2018-07-03
example: perl $0  <seed_capture_result file> <bar_pos file> <seed-info file>  <LFR low len> <reads_low_cnt> <target read cnt> <expand_len> <real_bar_thr>
USAGE
die $usage if(@ARGV != 8);

my $seed_file = shift;
my $bar_file = shift;
my $bar_info_file = shift;
my $LFR_len_thr = shift;
my $reads_low_cnt = shift;
my $target_reads_low = shift;
my $expand_len = shift;
my $real_bar_thr = shift;

open (IN,$seed_file) or die ("can't open the $seed_file!\n");
open (IN1,$bar_file) or die ("can't open the $bar_file!\n");
open (IN2,$bar_info_file) or die ("can't open the $bar_info_file!\n");
open (OUT,">single_evaluate_$expand_len.txt") or die("can't create the file!\n");

my $seed_cnt = 1;
my %bar_info;
my %bar_pos;

while(<IN1>){
  chomp;
  next if(!/\d+/);
  my @line = split;
  $bar_info{$line[0]}{$line[1]} = $_;
  my @chr = split(/\./, $line[1]);
  #print "$line[5]\t$line[6]\n";
  next if ($line[4] <= $LFR_len_thr || $line[6] <= $target_reads_low);
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
print OUT "seed_id LFR  unique_kme kmer_cnt   rate   correct_cnt  real_cnt   cap_cnt   ppv  fdr  fnr\n";

while(<IN>){
  chomp;
  #last if($seed_cnt > $iter);
  my @line = split;
  my  @seed = @line[0,1];
  my @seed1 = grep(/^$line[0]$/,@line);
  my @seed2 = grep(/^$line[1]$/,@line);
  splice(@line,0,2);
  if(@seed1>@seed2){
       Match_seed(@line,$seed[0]);
  }else{
       Match_seed(@line,$seed[1]);
  }
  $seed_cnt ++;   
}


sub Match_seed {
  my ($seed_ID,@Cluster_set) = (pop@_,@_);
 # splice(@Cluster_set,0,2); #delete the seed tag
  my $bar_strand = $bar_info{$seed_ID};
#  print "$bar_strand\n";
  my @True_well = ();
#  my @True_well_sub = ();
  my @max_p = (0,0,0,0,0,0,0);
  while (my ($key,$value) = each %$bar_strand){
         my @bar = split(/\t/,$value);
         if ($bar[4] >= $LFR_len_thr && $bar[6] >= $reads_low_cnt){ #add the condition: the initial seed's count should be larger than reads_low_cnt
          # print "$bar[5]\t$bar[6]\n";
           @True_well = evaluate($bar[0],$bar[1],$bar[2],$bar[3],$expand_len);#use seed ranges instead of center point
           my $set_len = @True_well;
           my @unique_True = removeRepeat(\@True_well);
           my @unique_cap = removeRepeat(\@Cluster_set);
           my %hash_True = map{$_=>1} @unique_True;
           my @common = grep {$hash_True{$_}} @unique_cap;

           my $real_cnt = @unique_True;
           my $cap_cnt = @unique_cap;
           my $correct_cnt = @common;
           my $ppv = $correct_cnt / $cap_cnt;
           $ppv = sprintf "%.2f",$ppv;
           my $fnr = 1 - $correct_cnt / ($real_cnt + 1);
           $fnr = sprintf "%.2f",$fnr;
           my $fdr = 1 - $ppv;
           $fdr = sprintf "%.2f",$fdr;
           
           if ($ppv >= $max_p[3]){
              $max_p[0] = $correct_cnt;
              $max_p[1] = $real_cnt;
              $max_p[2] = $cap_cnt;
              $max_p[3] = $ppv;
              $max_p[4] = $fdr;
              $max_p[5] = $fnr;
              $max_p[6] = $key;
           }
         }
     }
     print OUT "$seed_ID\t$max_p[6]\t$seed_info{$seed_ID}{'unique_cnt'}\t$seed_info{$seed_ID}{'kmer_cnt'}\t$seed_info{$seed_ID}{'rate'}\t$max_p[0]\t$max_p[1]\t$max_p[2]\t$max_p[3]\t$max_p[4]\t$max_p[5]\n"; 
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
 #find real wellId between in target seed barcode.
 my $new_hash = $bar_pos{$chr[0]};
 while (my ($key,$value) = each %$new_hash){
    #print "$key\t$value\n";
    while (my ($k1,$v1) = each %$value){
       if((($bar_pos{$chr[0]}{$key}{$k1}{'min'} > $start) && ($bar_pos{$chr[0]}{$key}{$k1}{'min'} < $end)) || (($bar_pos{$chr[0]}{$key}{$k1}{'max'} > $start) && ($bar_pos{$chr[0]}{$key}{$k1}{'max'} < $end))){ 
          # print "$k1\t";
           push (@real_wellId,$k1);
       }
     }
  }
  #print "\n";
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
  my @uniqArr = grep {++$count{$_} == 1} @$arrRef;
  return @uniqArr;
}

close IN;
close IN1;

