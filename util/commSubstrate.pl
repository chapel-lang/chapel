#!/usr/bin/perl

use File::Basename;

$utildirname = dirname($0);

$preset_substrate=$ENV{'CHPL_COMM_SUBSTRATE'};

if ($preset_substrate eq "") {
    $comm = `$utildirname/comm.pl --target`; chomp($comm);
    $platform = `$utildirname/platform.pl --target`; chomp($platform);

    if ($comm eq "gasnet") {
        if ($platform eq "xt-cnl") {
            $substrate = "portals";
	} elsif ($platform eq "cx1") {
	    $substrate = "ibv";
        } else {
            $substrate = "udp";
        }
    } elsif ($comm eq "armci") {
        $substrate = "mpi";
    } else {
        $substrate = "none";
    }
} else {
    $substrate = $preset_substrate;
}


print "$substrate\n";
exit(0);
