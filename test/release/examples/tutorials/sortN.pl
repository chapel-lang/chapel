#!/usr/bin/env perl


use Getopt::Std;
getopt('nio', \%opts);

if (defined $opts{n}) {
    $sortLines =  $opts{n};
} else {
    $sortLines = 5;
}

if (@ARGV > 0) {
    open INFILE, @ARGV[0] or die $!;
} else {
    die "usage: sortN.pl <-n #> infile\n";
}

$i = 0;
while (<INFILE>) {
    if ($i++ < $sortLines) {
        push(@sortme, $_);
        if ($i == $sortLines) {
            @sorted = sort @sortme;
            print @sorted;
        }
    } else {
        print "$_";
    }
}

