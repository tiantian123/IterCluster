use strict;
open IN,"uni_barkmer_hash" or die("cannot open file");
open IN2,"$ARGV[0]" or die("cannot open file");		#seed_list
open IN3,"uni_barkmer_num" or die("cannot open file");
my %uni_bar_kmer_hash;
while(<IN>){
  chomp;
  my @line = split;
  $uni_bar_kmer_hash{$line[0]}{$line[1]} = 1;
}
my %seed_hash;
my $seed_num = 0;
my %seed_index;
my %re_seed_index;
while(<IN2>){
  chomp;
  my @line = split;
  $seed_hash{$line[0]} = 1;
  $seed_index{$line[0]} = $seed_num;
  $re_seed_index{$seed_num} = $line[0];
  $seed_num ++;
}
my %uni_bar_index;
my %re_uni_bar_index;
my %uni_bar_kmer_num;my $uni_bar_type = 0;
while(<IN3>){
  chomp;
  my @line = split;
  $uni_bar_kmer_num{$line[0]} = $line[1];
  $uni_bar_index{$line[0]} = $uni_bar_type;
  $re_uni_bar_index{$uni_bar_type} = $line[0];
  $uni_bar_type ++;
}

print "begin to calculate barcodes' relation\n";
my @matrix;
while(my($key,$value) = each %seed_hash){
  chomp;
  my @line = split;
  my $bar1 = $key;
  my $bar1_unikmer_num = $uni_bar_kmer_num{$key};
  while(my($k1,$v1) = each %uni_bar_kmer_hash){
    my $sum = 0;
    while(my($k2,$v2) = each %$v1){
      if(exists ${$uni_bar_kmer_hash{$bar1}}{$k2}){
        $sum ++;
      }
    }
    my $percent = $sum / $bar1_unikmer_num;
    $matrix[$seed_index{$bar1}][$uni_bar_index{$k1}] = $percent;
#    if($percent >= 0.2){
#      $cluster_set{$bar1}{$k1} = $percent;
#    }
  }
}
open OUT,">$ARGV[1].relation_matrix" or die("cannot open file");
print OUT "head";
foreach(0..$uni_bar_type-1){
  print OUT "\t$re_uni_bar_index{$_}";
}
print OUT "\n";
foreach(0..$seed_num-1){
  my $m1 = $_;
  print OUT "$re_seed_index{$m1}\t";
  foreach(0..$uni_bar_type-1){
    my $m2 = $_;
    print OUT sprintf "%.5f",$matrix[$m1][$m2];
    print OUT "\t";
  }
  print OUT "\n";
}

close IN;
close IN2;
close IN3;
close OUT;
