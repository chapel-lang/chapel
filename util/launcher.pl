#!/usr/bin/perl

use File::Basename;

$utildirname = dirname($0);

my $flag = $ARGV[0];

$preset_launcher=$ENV{'CHPL_LAUNCHER'};

if ($preset_launcher eq "") {
    $comm = `$utildirname/comm.pl --target`; chomp($comm);
    $substrate = `$utildirname/commSubstrate.pl`; chomp($substrate);
    $platform = `$utildirname/platform.pl --target`; chomp($platform);

    if ($comm eq "armci") {
        if ($substrate eq "mpi") {
            $launcher = "mpirun";
        } else {
            $launcher = "none";
        }
    } elsif ($platform eq "xmt-sim") {
	$launcher = "zebra";
    } else {
        $launcher = "none";
    }
} else {
    $launcher = $preset_launcher;
}

print "$launcher\n";
exit(0);
