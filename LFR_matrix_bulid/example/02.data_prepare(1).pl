#!/usr/BIN/perl
use strict;

open IN,"$ARGV[0]" or die ("cannot open file");
open OUT,">kmer_hash" or die ("cannot open file");
open OUT2,">bar_kmer_hash" or die ("cannot open file");
open TEM, ">output.log" or die ("cannot open file!\n");

my $up_bound = $ARGV[3];
my $down_bound = $ARGV[2];
my $kmer_len = $ARGV[1];
my $n = 0;
my $bar_num;
my $read;
my %kmer_hash;
my %read_hash;
my %bar_kmer_hash;
my %bar_kmer_num;
my %seed_hash;
my %cluster_set;
my $uni_bar_type = 0;
my ($key,$value);

while(<IN>){
  chomp;
  my @line = split;
  $n ++;
  if($n % 4 == 1){
    $bar_num = $line[1];
    if(!(exists $read_hash{$bar_num})){
      $read_hash{$bar_num} = 0;
    }
    $read_hash{$bar_num} += 1 ;
  }
  if($n % 4 == 2){
    $read = $line[0];
    my $read_len = length($read);
    my $kmer_num = $read_len - $kmer_len + 1;
    foreach(0..$kmer_num-1){
       my $kmer = substr($read, $_, $kmer_len);
       my @reverse_kmer = split(//,$kmer);
       @reverse_kmer = reverse @reverse_kmer;
       my $kmer2 = join "",@reverse_kmer;
       $kmer2 =~ tr/ATCG/TAGC/;
       my $usedkmer=$kmer
       if(exists($kmer_hash{$kmer})){
          $kmer_hash{$kmer} += 1;
       }elsif(exists($kmer_hash{$kmer2})){
          $kmer_hash{$kmer2} += 1;
          $usedkmer = $kmer2;
       }else{
          $kmer_hash{$kmer} = 1;
       }
       if(exists($bar_kmer_hash{$bar_num}{$usedkmer})){
         $bar_kmer_hash{$bar_num}{$usedkmer} += 1;
       }else{
         $bar_kmer_hash{$bar_num}{$usedkmer} = 1;
         if(!exists($bar_kmer_num{$bar_num})){$bar_kmer_num{$bar_num} = 0;}
         $bar_kmer_num{$bar_num} ++ ;
       }
    }
  }
}
print TEM "finish kmer hash and bar kmer hash\n";
open OUT3, ">bar_kmer_num";
while(my($key,$value) = each %bar_kmer_hash){
  while(my($k1,$k2) = each %$value){
    print OUT2 "$key\t$k1\t$k2\n";
  }
}
while(my($key,$value) = each %bar_kmer_num){
  print OUT3 "$key\t$value\n";
}

close OUT;
close OUT2;
close OUT3;

open OUT4,">uni_kmer_hash" or die("cannot open file");
my %uni_kmer_hash;
my $uni_kmer_num = 0;
while(($key,$value) = each %kmer_hash){
  if($value >= $down_bound and $value <= $up_bound){
    $uni_kmer_hash{$key} = 1;
    $uni_kmer_num ++;
  }
}
print TEM "finish nui_kmer_hash\n";
print OUT4 "unique kmer number = $uni_kmer_num\n";
while(($key,$value) = each %uni_kmer_hash){
  print OUT4 "$key\n";
}
close OUT4;

my %uni_bar_kmer_hash;
my %uni_bar_kmer_num;
while(($key,$value) = each %bar_kmer_hash){
  while(my($k1,$k2) = each %$value){
    if(exists $uni_kmer_hash{$k1}){
      $uni_bar_kmer_hash{$key}{$k1} = 1;
      if(!exists $uni_bar_kmer_num{$key}){$uni_bar_kmer_num{$key} = 0;}
      $uni_bar_kmer_num{$key} += 1;
    }
  }
}
print TEM "finish uni_bar_kmer_hash\n";
open OUT5,">uni_barkmer_num" or die("cannot open file");
open OUT6,">uni_barkmer_hash" or die("cannot open file");
my %uni_bar_index;
my %re_uni_bar_index;
while(($key,$value) = each %uni_bar_kmer_num){
  print OUT5 "$key\t$value\n";
  $uni_bar_index{$key} = $uni_bar_type;
  $re_uni_bar_index{$uni_bar_type} = $key;
  $uni_bar_type ++;
  
}
while(($key,$value) = each %uni_bar_kmer_hash){
  while(my($k1,$k2) = each %$value){
    print OUT6 "$key\t$k1\t$k2\n";
  }
}
close OUT5;
close OUT6;
