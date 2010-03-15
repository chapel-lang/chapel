#!/usr/bin/perl

use File::Basename;

$utildirname = dirname($0);

$preset_stm=$ENV{'CHPL_STM'};

if ($preset_stm eq "") {
    $stm = "none";
} else {
    $stm = $preset_stm;
}

print "$stm\n";
exit(0);
