#!/usr/bin/perl

use File::Basename;

$utildirname = dirname($0);

my $flag = $ARGV[0];

$preset_launcher=$ENV{'CHPL_LAUNCHER'};

if ($preset_launcher eq "") {
    $comm = `$utildirname/comm.pl --target`; chomp($comm);
    $substrate = `$utildirname/commSubstrate.pl`; chomp($substrate);
    $platform = `$utildirname/platform.pl --target`; chomp($platform);

    $launcher = "none";
    if ($comm eq "gasnet") {
        if ($substrate eq "udp") {
            $launcher = "amudprun";
        } elsif ($substrate eq "portals") {
            $launcher = "aprun";
        } elsif ($substrate eq "ibv") {
	    if ($platform eq "pwr5" || $platform eq "pwr6") {
		$launcher = "loadleveler";
	    } else {
		$launcher = "gasnetrun_ibv";
	    }
	} elsif ($substrate eq "lapi") {
	    $launcher = "loadleveler";
	}
    } elsif ($comm eq "armci") {
        if ($substrate eq "mpi") {
            $launcher = "mpirun";
        }
    } elsif ($platform eq "xmt-sim") {
	$launcher = "zebra";
    } elsif ($platform eq "x1-sim" || $platform eq "x2-sim") {
      $launcher = "apsim";
    } elsif ($comm eq "mpi") {
        $launcher = "mpirun";
    } elsif ($comm eq "pvm") {
	$launcher = "pvm";
    }
} else {
    $launcher = $preset_launcher;
}

print "$launcher\n";
exit(0);
