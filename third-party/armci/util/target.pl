#!/usr/bin/perl

use File::Basename;

$armciutildirname = dirname($0);
$utildirname = "$armciutildirname/../../../util";

$preset_target = $ENV{'CHPL_ARMCI_PLATFORM'};

if ($preset_target eq "") {
    $platform = `$utildirname/platform.pl --target`;
    chomp($platform);
    if ($platform eq "cygwin") {
        $target = "CYGWIN";
    } elsif ($platform eq "linux" || $platform eq "linux64") {
        $target = "LINUX";
    } elsif ($platform eq "sunos") {
        $target = "SOLARIS";
    } else {
        print "Cannot determine ARMCI target -- set CHPL_ARMCI_PLATFORM manually\n";
        exit(1);
    }
} else {
    $target = $preset_target;
}

print "$target\n";
exit(0);
