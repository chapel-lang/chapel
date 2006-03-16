#!/usr/bin/perl

# Usage: paratest.client.pl id chapeltestdir testdir compiler
#  
# Used remotely by paratest.server.pl to run start_test locally.
#  id - used to create a file to synchronize with paratest.server.pl
#  chapeltestdir - root dir of Chapel test infrastructure
#  testdir - directory to run start_test on
#  compiler - Chapel compiler to use
# 

$debug = 1;
$logdir = "Logs";
$synchdir = "Logs/.synch";
$testcmd = "nice start_test";

sub systemd {
    my ($cmd) = @_;

    print "$cmd\n" if ($debug);
    system ($cmd);
}


sub main {
    my ($node) = `uname -n`;
    my ($logfile, $synchfile);

    ($node, $junk) = split (/\./, $node, 2);

    if ($#ARGV != 3) {
        print "@ARGV\n";
        print "usage: multitest.client.pl id chapeltestdir testdir compiler\n";
        exit (3);
    }

    $id = $ARGV[0];
    $workingdir = $ARGV[1];
    $testdir = $ARGV[2];
    $compiler = $ARGV[3];
    print "$node $workingdir $testdir $compiler\n";

    $synchfile = "$synchdir/$node.$id";

    unless (chdir ($workingdir)) {
        print "Error: cannot change to directory $workingdir\n";
        exit (2);
    }
    print "\n* $node up @ $workingdir *\n";

    unless (-e $synchdir and -d $synchdir) {
        print "Error: synch directory $synchdir does not exist\n";
        exit (2);
    }

    $dirfname = $testdir;
    $dirfname =~ s/\//-/g;
    $logfile = "$logdir/$dirfname.$node.log";
    unlink $logfile if (-e $logfile);
    # print "echo $testdir.$node.log > $logfile\n";
    # systemd ("echo $testdir.$node.log > $logfile");

    $testarg = "-compiler $compiler -startdir $testdir -logfile $logfile";
    systemd ("$testcmd $testarg");

    systemd ("echo feed me > $synchfile");  # signal ready for more
}


main ();
