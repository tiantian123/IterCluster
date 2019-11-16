#!/usr/bin/perl
use strict;

if(@ARGV != 3)
{
        print "Example: perl split_10X_reads.pl barcoded.fastq.gz 50 ZEBRA \n";
        exit(0);
}

my @line;
my $filter = 50;
my $read_len = $ARGV[1];
my $read_len1 = 0;
my $read_len2 = 0;
my $n=0;
my $check = 0;
my %hash;
my %bar_hash;
my $barcode;
my $num = 0;
my $seq;
my $reads_num = 0;
my $reads_num_filte = 0;
my $Bases_filter = 0;
my $Bases = 0;
my @A;
my ($key,$value);

open OUT1,"| gzip > $ARGV[2].1.fq.gz" or die "Can't write file";
open OUT2,"| gzip > $ARGV[2].2.fq.gz" or die "Can't write file";
open OUT3,"> $ARGV[2].list" or die "Can't write file";

############### First loop
open IN,"gzip -dc $ARGV[0] |" or die "Can't open file";
while(<IN>)
{
	chomp;
  @line = split;	
  $n++;
  
  if($n%8 == 1)
  {  if(/BX:Z/)
  	 {   	  
        @A = split(/:/,$line[1]);
  	 	  $barcode = $A[2]; 	  
  	 	  $bar_hash{$barcode} ++;
   	 }
  }
  elsif($n == 2)
  { $read_len1 =length($line[0]);
  }
  elsif($n == 6)
  { $read_len2 =length($line[0]);
  }

}
close(IN);

############### Second loop
open IN,"gzip -dc $ARGV[0] |" or die "Can't open file";
while(<IN>)
{
	chomp;
  @line = split;	
  $n++;
  
  if($n%8 == 1)
  {  if(/BX:Z/)
  	 {  $reads_num ++; 
  	 	  $check = 1; 	 	  
        @A = split(/:/,$line[1]);
  	 	  $barcode = $A[2];
        if($bar_hash{$barcode} > $filter)
  	 	  {  $reads_num_filte ++;  	 	  
		  	 	  if(!(exists $hash{$barcode}))
		  	 	  {  $num ++;
		  	 	  	 $hash{$barcode} = $num;
		  	 	  }  	 	  
		
		        print OUT1 "$line[0]\-$hash{$barcode}\/1\t$hash{$barcode}\t1\n";
  	 	  }
  	 	  else
  	 	  {  $check = 0;
  	 	  }
   	 }
   	 else
   	 {  $check = 0;
   	 }
  }
  elsif($n%8 == 5)
  {  if(/BX:Z/)
  	 {  
  	 	  $check = 1;  	 	 
        @A = split(/:/,$line[1]);
        $barcode = $A[2]; 	 
 
        if($bar_hash{$barcode} > $filter)
  	 	  {  print OUT2 "$line[0]\-$hash{$barcode}\/2\t$hash{$barcode}\t1\n";
  	 	  }
  	 	  else
  	 	  {  $check = 0;
  	 	  }  	 	     
   	 }
   	 else
   	 {  $check = 0;
   	 }	
  }
  elsif(($check==1) && ($n%8==2 || $n%8==3 || $n%8==4))
  {
  	 $seq = substr($line[0],0,$read_len);
     print OUT1 "$seq\n";
  }
  elsif(($check==1) && ($n%8==6 || $n%8==7 || $n%8==0))
  {
  	 $seq = substr($line[0],0,$read_len);
     print OUT2 "$seq\n";
  }  
}
close(IN);

###############
foreach $key (sort {$hash{$a} <=> $hash{$b}} keys %hash) {print OUT3 "$hash{$key}\t$key\t$bar_hash{$key}\n"; }

$Bases = ($reads_num * ($read_len1+$read_len2))/(1e+9);
$Bases_filter = ($reads_num_filte * ($read_len1+$read_len2))/(1e+9);
print "\n Barcode_num= $num \n reads_num(PE)= $reads_num   reads_len= $read_len1;$read_len2   Bases= $Bases Gbp  \n reads_num_filte= $reads_num_filte   Bases_filter= $Bases_filter Gbp \n";

close(OUT1);    
close(OUT2);    
close(OUT3);    
