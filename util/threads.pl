#!/usr/bin/perl

use File::Basename;

$utildirname = dirname($0);

$preset_threads=$ENV{'CHPL_THREADS'};

if ($preset_threads eq "") {
    $platform = `$utildirname/platform.pl --target`;
    chomp($platform);
    if ($platform eq "mta" || $platform eq "xmt" || $platform eq "xmt-sim") {
	$threads = "mta";
    } elsif ($platform eq "x1-sim" || $platform eq "x2-sim") {
        $threads = "none";
    } else {
	$threads = "pthreads";
    }
} else {
    $threads = $preset_threads;
}


print "$threads\n";
exit(0);
