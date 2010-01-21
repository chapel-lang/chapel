#!/usr/bin/perl

use File::Basename;

$utildirname = dirname($0);

$uname = `uname -a`;
$pwd = `pwd`; chomp($pwd);

print "uname -a: $uname";

print "\$CHPL_HOME: $ENV{'CHPL_HOME'}\n";
print "pwd: $pwd\n";
print "script location: $utildirname\n";

$hostplatform = `$utildirname/platform.pl --host`; chomp($hostplatform);
$hostcompiler = `$utildirname/compiler.pl --host`; chomp($hostcompiler);

print "CHPL_HOST_PLATFORM: $hostplatform\n";
print "CHPL_HOST_COMPILER: $hostcompiler\n";


$targplatform = `$utildirname/platform.pl --target`; chomp($targplatform);
$targcompiler = `$utildirname/compiler.pl --target`; chomp($targcompiler);

print "CHPL_TARGET_PLATFORM: $targplatform\n";
print "CHPL_TARGET_COMPILER: $targcompiler\n";

$comm = `$utildirname/comm.pl`; chomp($comm);
print "CHPL_COMM: $comm\n";
if ($comm eq "none") {
} else {
    $commsubstrate = `$utildirname/commSubstrate.pl`; chomp($commsubstrate);
    print "  CHPL_COMM_SUBSTRATE: $commsubstrate\n";
    if ($comm eq "gasnet") {
        $commsegment = `$utildirname/commSegment.pl`; chomp($commsegment);
        print "  CHPL_GASNET_SEGMENT: $commsegment\n";a
    }
}

$tasks = `$utildirname/tasks.pl`; chomp($tasks);
print "CHPL_TASKS: $tasks\n";
$threads = `$utildirname/threads.pl`; chomp($threads);
print "CHPL_THREADS: $threads\n";
$launcher = `$utildirname/launcher.pl`; chomp($launcher);
print "CHPL_LAUNCHER: $launcher\n";
$mem = `$utildirname/mem.pl`; chomp($mem);
print "CHPL_MEM: $mem\n";
$make = `$utildirname/chplmake`; chomp($make);
print "CHPL_MAKE: $make\n";
