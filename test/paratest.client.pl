#!/usr/bin/perl

# Client-side of the parallel testing script, paratest.server.pl.
# Used remotely by paratest.server.pl to run start_test locally.
#
# Usage: paratest.client.pl id chapeltestdir testdir distmode futures valgrind [compopts] [comm]
#  
#  id - used to create a file to synchronize with paratest.server.pl
#  chapeltestdir - root dir of Chapel test infrastructure
#  testdir - directory to run start_test on
#  distmode - work distribution mode (0=directory, 1=file)
#  futures - include .future tests (0=no, 1=yes)
#  valgrind - run valgrind (0=no, 1=yes)
#  compopts - optional Chapel compiler options
#  comm - optional Chapel CHPL_COMM setting
# 

$debug = 0; # set $verbose=1 in server
$logdir = "Logs";
$synchdir = "Logs/.synch";
$testcmd = "nice ../util/start_test";
$publish_delay = 2;

sub systemd {
    my ($cmd) = @_;
    print "$cmd\n" if ($debug);
    system ($cmd);
}


sub main {
    my ($logfile, $synchfile, $workingdir, $testdir, $compiler, $platform);
    my ($node) = `uname -n`;
    chomp $node;
    ($node, $junk) = split (/\./, $node, 2);

    if ($#ARGV < 4) {
        print "@ARGV\n";
        print "usage: paratest.client.pl id chapeltestdir testdir futures valgrind [compopts] [comm]\n";
        exit (3);
    }

    $id = $ARGV[0];
    $workingdir = $ARGV[1];
    $testdir = $ARGV[2];
    $incl_futures = ($ARGV[3] == 1) ? "-futures" : "" ;
    $valgrind = ($ARGV[4] == 1) ? "-valgrind" : "";

    print "$id $workingdir $testdir $incl_futures $valgrind" if $debug;
    if ($#ARGV>=5) {
        $compopts = "-compopts \"" . $ARGV[5] . "\"";
    }
    if ($#ARGV>=6) {
        $comm = "-comm \"" . $ARGV[6] . "\"";
    }

    $synchfile = "$synchdir/$node.$id";

    unless (chdir ($workingdir)) {
        print "Error: cannot change to directory $workingdir\n";
        exit (2);
    }
    print "\n* $node up @ $workingdir *\n";

    $platform = `../util/platform.pl`; chomp $platform;
    $compiler = "../bin/$platform/chpl";
    unless (-e $compiler) {
        print "Error: cannot find chpl as '$compiler'\n";
        exit (2);
    }

    unless (-e $synchdir and -d $synchdir) {
        print "Error: synch directory $synchdir does not exist\n";
        exit (2);
    }

    print "$node $workingdir $testdir $compiler\n";

    $dirfname = $testdir;
    $dirfname =~ s/\//-/g;
    $logfile = "$logdir/$dirfname.$node.log";
    unlink $logfile if (-e $logfile);

    $testarg = "-compiler $compiler -logfile $logfile $incl_futures $valgrind $compopts $comm";
    $testarg = "$testarg $testdir -norecurse";
    $chplhome = `cd .. && pwd && cd test`; chomp $chplhome;
    $ENV{'CHPL_HOME'} = $chplhome;
    
    systemd ("$testcmd $testarg");

    sleep $publish_delay;

    systemd ("echo feed me > $synchfile");  # signal ready for more
}


main ();
