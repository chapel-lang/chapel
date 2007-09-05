#!/usr/bin/perl

$suppressions = $ARGV[0];
$summaryfile = $ARGV[1];
$tmpfile = "$summaryfile.tmp";

open SUPPRESSIONS, "$suppressions" or die "can't open $suppressions $!";
my @suppressions = <SUPPRESSIONS>;
close (SUPPRESSIONS);

foreach my $suppression (@suppressions) {
    chomp($suppression);
    if (substr($suppression, 0, 1) eq "#") {
    } else {
        print "Removing suppression for: $suppression\n";
        $match = `grep \" $suppression\]\" $summaryfile`;
        if ($match eq "") {
            system("echo \"[Error: didn\'t find expected suppression $suppression]\" >> $summaryfile");
        } else {
            system("grep -v \" $suppression\]\" $summaryfile > $tmpfile");
            system("mv $tmpfile $summaryfile");
        }
    }
}
