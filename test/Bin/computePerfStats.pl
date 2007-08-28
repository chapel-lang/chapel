#!/usr/bin/perl

$testname = $ARGV[0];
$outputdir = $ARGV[1];

my $key_file = "$testname.perfkeys";
my $test_output_file = "$testname.exec.out.tmp";
my $data_file = "$outputdir/$testname.dat";

open KEYS, "$key_file" or die "can't open $key_file $!";
my @keys = <KEYS>;
close (KEYS);
foreach my $key (@keys) {
    chomp($key);
}

open TEST, "$test_output_file" or die "can't open $test_output_file $!";
my @file_lines = <TEST>;
close (TEST);

if (-e "$data_file") {
} else {
    open STATS, ">$data_file" or die "can't open $data_file for creating $!";
    print STATS "# Date";
    foreach my $key (@keys) {
        print STATS "\t$key";
    }
    print STATS "\n";
    close (STATS);
}
open STATS, ">>$data_file" or die "can't open $data_file for appending $!";

my ($sec,$min,$hour,$mday,$mon,$year,$wday,$yday,$isdst) = localtime time;

$mon += 1;
$year -= 100;

my $foundEverything = 1;
printf STATS "%.2d/%.2d/%.2d ", $mon, $mday, $year;
foreach my $key (@keys) {
    print "Looking for $key...";
    print STATS "\t";
    my $found = 0;
    foreach my $line (@file_lines) {
        if ($line =~ m/\Q$key\E(\s*)(\S*)/ && $found == 0) {
            print "found it: $2\n";
            print STATS "$2";
            $found = 1;
        }
    }
    if ($found == 0) {
        print STATS "-";
        print "didn't find it\n";
        $foundEverything = 0;
    }
}
print STATS "\n";

if ($foundEverything == 1) {
    exit 0;
} else {
    print "output was:\n";
    system("cat $testname.exec.out.tmp");
    exit 1;
}
