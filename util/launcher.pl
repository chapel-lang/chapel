#!/usr/bin/perl

use File::Basename;

$utildirname = dirname($0);

my $flag = $ARGV[0];

$preset_launcher=$ENV{'CHPL_LAUNCHER'};

if ($preset_launcher eq "") {
    $launcher = "none";
} else {
    $launcher = $preset_launcher;
}


print "$launcher\n";
exit(0);
