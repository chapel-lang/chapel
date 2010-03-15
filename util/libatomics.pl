#!/usr/bin/perl

use File::Basename;

$hostname=$ENV{'HOSTNAME'};
$peenv=$ENV{'PE_ENV'};
$preset_stm=$ENV{'CHPL_STM'};

if ($preset_stm eq "gtm" || $preset_stm eq "now") {
    if ($hostname eq "malaga" || $hostname eq "n34-024.ftpn.ornl.gov" ||
	$hostname eq "dmz00.ornl.gov") {
	$libatomics="/home/ssridhar/research/TM/libatomic_ops-1.2/malaga";
    } else {
	if ($peenv eq "PGI") {
	    $libatomics="/ccs/home/ssridhar/research/TM/libatomic_ops-1.2-pgi/jaguar";
	} else {
	    $libatomics="/ccs/home/ssridhar/research/TM/libatomic_ops-1.2-gcc/jaguar";
	}
    }
} else {
    $libatomics="";
}

print "$libatomics\n";
exit(0);
