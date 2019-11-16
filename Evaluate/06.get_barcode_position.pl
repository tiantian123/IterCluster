#!/usr/bin/perl
use strict;
use List::Util qw(max min);
use Data::Dumper;

my $usage =<<USAGE;
version: v3,2018-05-24
example: perl $0 <aln_file file> <split_log file> <prefix, string> <distance between LFR, int>
USAGE

die $usage if(@ARGV != 4);
my $file = shift;
my $log_file = shift;
my $out_prefix = shift;
my $threshold = shift;

open (IN,$log_file) or die ("can't open the $log_file!\n");
open (IN1,$file) or die ("can't open the $file!\n");
open (OUT,">$out_prefix.barcode_info") or die ("can't create the file!\n");

#recode the barcode ID
my %barcode_ID;

while(<IN>){
	chomp;
	next if(/Barcode|Reads/);
	my @line = split;
	$barcode_ID{$line[2]}=$line[0];
	}
	
#find the barcode ID start Pos and end Pos	
my %bar_info;
#my %bar_pos;
while(<IN1>){
	chomp;
        next if(/\@SQ|\@PG/);
	my @line = split;
	my @ID = split(/#|-/,$line[0]);
	my $index = $barcode_ID{$ID[1]};
        push (@{$bar_info{$index}{$line[2]}},$line[3]);
}

print OUT "bar_ID\tbar_strand\tstart_Pos\tend_Pos\tlenth\tcenter\treads_cnt\n";

foreach my $key (sort {$a <=>$b} keys%bar_info){
       my $value = $bar_info{$key};
       while (my ($k1,$v1) = each %$value){ 
         print "$key  $k1:\n";
         my @sortlist = sort{$a<=>$b} @{$bar_info{$key}{$k1}};
         foreach (@sortlist){
           print "$_\n";
         }
         my @center = Multi_center(@sortlist,$threshold);
         
         my $center_len = @center;
         print "$center_len: ";
         foreach (@center){
               print "$_  ";
         }
         print "\n";
         my $count = 0;
         for (my $i =0; $i < $center_len; $i+=2){
            my @sub_list = ();
            print "$center[$i]\t$center[$i+1]\n";
            foreach($center[$i]..$center[$i+1]){
              push (@sub_list,$sortlist[$_]);
            }
            my $max = max @sub_list;
            my $min = min @sub_list;
            my $len = $max - $min;
            my @restlist = grep (/$max|$min/,@sub_list);
            my $center = average(@restlist);
            my $reads_cnt = @sub_list;
            print OUT "$key\t$k1.$count\t$min\t$max\t$len\t$center\t$reads_cnt\n";
            $count ++;
         }
      }
}

sub average{
    my @list = @_;
    my $len = @list;
    my $total = 0;
    foreach my $value (@list){
      $total += $value;
    }
    return ($total/$len);
}
#split the barcode responed LFR
sub Multi_center{
  my ($threshold,@list) = (pop@_,@_);
  my @Dis_Pos; 
  push (@Dis_Pos,0);
  my $key;
  foreach $key (1..@list-1){
    my $diff = $list[$key]-$list[$key-1];
    if($diff > $threshold){
       push (@Dis_Pos,$key-1);
       push (@Dis_Pos,$key);
    }
  }
  my $len = @list -1;
  push (@Dis_Pos,$len);
 return @Dis_Pos;
}

close IN;
close IN1;
close OUT;
