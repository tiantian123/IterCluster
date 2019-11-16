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
open (OUT,">evaluate_$expand_len.txt") or die("can't create the file!\n");

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
print OUT "seed_id  unique_kme kmer_cnt   rate   correct_cnt  real_cnt   cap_cnt   ppv  fdr  fnr\n";

my @seed=();
my @set1=();
my @set2=();

my $seq = <IN>;
chomp $seq;
my @line = split(/\t| /,$seq);
@seed = @line[0,1];
#print "first seed: $seed[0]\t$seed[1]\n";
#print "real seed: $line[0]\t$line[1]\n";
my @seed1 = grep(/^$line[0]$/,@line);
my @seed2 = grep(/^$line[1]$/,@line);
splice(@line,0,2);
if(@seed1>@seed2){
     push(@set1,@line);
  }elsif(@seed1<@seed2){
     push(@set2,@line);
 }else{                  #the 2 seed ID  are both appear in  cluster set.
     print "the same cluster set: $seed[0] $seed[1]\n";
       push(@set1,@line);
       push(@set2,@line);
  }

while(<IN>){
  chomp;
  #last if($seed_cnt > $iter);
  @line = split;
  if($line[0] != $seed[0] && $line[1]!= $seed[1]){
    #search seed barcode info
    if(@set1 != 0){
       Match_seed(@set1,$seed[0]);
    }
    if(@set2 != 0){
      Match_seed(@set2,$seed[1]);
    }
    @set1=();
    @set2=();
  }
  @seed = @line[0,1];
  @seed1 = grep(/^$line[0]$/,@line);
  @seed2 = grep(/^$line[1]$/,@line);
  splice(@line,0,2);
  if(@seed1>@seed2){
       push(@set1,@line);
  }elsif(@seed1<@seed2){
       push(@set2,@line);
  }else{                  #the 2 seed ID  are both appear in  cluster set.
       print "the same cluster set: $seed[0] $seed[1]\n";
       push(@set1,@line);
       push(@set2,@line);
  }
  # print OUT "\n";
  $seed_cnt ++;   
}
if(@set1 != 0){
   Match_seed(@set1,$seed[0]);
}
if(@set2 != 0){
  Match_seed(@set2,$seed[1]);
}
sub Match_seed {
  my ($seed_ID,@Cluster_set) = (pop@_,@_);
 # splice(@Cluster_set,0,2); #delete the seed tag
  my $bar_strand = $bar_info{$seed_ID};
#  print "$bar_strand\n";
  my @True_well = ();
  my @True_well_sub = ();
  while (my ($key,$value) = each %$bar_strand){
         my @bar = split(/\t/,$value);
         if ($bar[4] >= $LFR_len_thr && $bar[6] >= $reads_low_cnt){ #add the condition: the initial seed's count should be larger than reads_low_cnt
          # print "$bar[5]\t$bar[6]\n";
           @True_well_sub = evaluate($bar[0],$bar[1],$bar[2],$bar[3],$expand_len);#use seed ranges instead of center point
           my $set_len = @True_well_sub;
           #print "$set_len\n";
           if ($set_len > $real_bar_thr ){
               push (@True_well,@True_well_sub);
           }
         }
     }
     my @unique_True = removeRepeat(\@True_well);
     my @unique_cap = removeRepeat(\@Cluster_set);
     my %hash_True = map{$_=>1} @unique_True;
    # my %hash_cap = map{$_=>1} @unique_cap;
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
     print OUT "$seed_ID\t$seed_info{$seed_ID}{'unique_cnt'}\t$seed_info{$seed_ID}{'kmer_cnt'}\t$seed_info{$seed_ID}{'rate'}\t$correct_cnt\t$real_cnt\t$cap_cnt\t$ppv\t$fdr\t$fnr\n"; 
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

