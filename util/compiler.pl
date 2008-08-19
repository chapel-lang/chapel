#!/usr/bin/perl

use File::Basename;

$utildirname = dirname($0);

my $flag = $ARGV[0];

if ($flag eq "" || $flag eq "--host") {
    $preset_compiler=$ENV{'CHPL_HOST_COMPILER'};
} elsif ($flag eq "--target") {
    $preset_compiler=$ENV{'CHPL_TARGET_COMPILER'};
    if ($preset_compiler eq "") {
	if (`$utildirname/platform.pl --host` eq `$utildirname/platform.pl --target`) {
	    $preset_compiler=$ENV{'CHPL_HOST_COMPILER'};
	}
    }
}


if ($preset_compiler eq "") {
    $platform = `$utildirname/platform.pl $flag`;
    chomp($platform);
    if ($platform eq "mta" || $platform eq "xmt" || $platform eq "xmt-sim") {
	$compiler = "cray-mta";
    } elsif ($platform eq "x1" || $platform eq "x2") {
	$compiler = "cray-vec";
    } elsif ($platform eq "xt-cnl") {
	$subcompiler = "-$ENV{'PE_ENV'}";
	if ($subcompiler eq "-") {
	    $subcompiler = "";
	} else {
	    $subcompiler =~ tr/A-Z/a-z/;
	}
	$compiler = "cray-xt$subcompiler";
    } else {
	$compiler = "gnu";
    }
} else {
    $compiler = $preset_compiler;
}


print "$compiler\n";
exit(0);
