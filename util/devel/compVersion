#!/usr/bin/env perl

use File::Basename;

$utildirname = dirname($0);

my $version_file = "$utildirname/../../compiler/main/version_num.h";

open VERSION, "$version_file" or die "can't open $version_file $!";
my @versionLines = <VERSION>;
close (VERSION);

foreach my $line (@versionLines) {
    if ($line =~ m/MAJOR_VERSION(\s*)(\S*)/) {
        print "$2.";
    }
    if ($line =~ m/MINOR_VERSION(\s*)(\S*)/) {
        print "$2.";
    }
    if ($line =~ m/UPDATE_VERSION(\s*)(\S*)/) {
        print "$2\n";
    }
}
