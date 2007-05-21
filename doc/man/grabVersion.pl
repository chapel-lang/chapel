#!/usr/bin/perl

my $version_file = "../../compiler/main/version_num.h";

open VERSION, "$version_file" or die "can't open $version_file $!";
my @versionLines = <VERSION>;
close (VERSION);

foreach my $line (@versionLines) {
    if ($line =~ m/MAJOR_VERSION(\s*)(\S*)/) {
        print "$2.";
    }
    if ($line =~ m/MINOR_VERSION(\s*)(\S*)/) {
        print "$2\n";
    }
}
