#!/usr/bin/perl

# Usage: paratest.server.pl [-compopts] [-dirfile d] [-filedist] [-futures] 
#                            [-help|-h] [-nodefile n] [-logfile l] [-valgrind]
#   -compopts s: s is a string that is passed with -compopts to start_test.
#   -comm s    : s is a setting for CHPL_COMM, e.g., none or gasnet
#   -dirfile  d: d is a file listing directories to test. Default is ".". Lines
#                beginning with # are ignored.
#   -filedist  : distribute work at the granularity of test files (directory
#                granurality is the default).
#   -futures   : include .future tests (default is none).
#   -nodefile n: n is a file listing nodes to run on. Default is current node.
#                To run multiple processes on MP nodes, list the node multiple
#                times, once for each desired process. Lines with # are ignored.
#   -logfile  l: l is the output logfile. Default is "user"."platform".log in
#                subdirectory Logs.
#   -valgrind  : pass -valgrind flag to start_test
#
# Creating a file name PARAHALT in the root test directory halts the
# distribution of more work.
#
# Requirements:
#  - paratest.server.pl is run in $CHPL_HOME/test.
#  - Chapel compiler bin as $CHPL_HOME/bin/"platform"/chpl.
#  - Scripts start_test and paratest.client.pl in the same directory
#      as paratest.server.pl. It will create a temporary directory
#      (.synch) to synchronize the distribution of work to the client
#      processes.
#  - Be able to run start_test remotely. This may include the following:
#    - Chapel built without node-specific local temporary directories.
#        Nodes must be able to execute start_test. For example, the
#        start_test script may invoke the compiler as ../bin/linux/chpl.
#        If Chapel is built with CHPLDEVTMP defined to a machine-specific local
#        tmp directory (e.g., /ptmp), the script may not be able to execute
#        chpl on a different machine. A good check is to run start_test with 
#        a different machine to see if it can run successfully.
#    - To run jobs remotely over a secured network without having to
#      type a password each time, use ssh-agent and ssh-add. See
#      http://upc.lbl.gov/docs/user/sshagent.html for a short tutorial.
#


# for debugging
$debug = 0;                            # turn on debug output
$verbose = 0;                          # more verbose output

$dirs_to_ignore = "CVS|Bin|Logs|Samples|Share|OUTPUT|RCS";

$logdir = "Logs";                      # dir under test to store logs
$synchdir = "$logdir/.synch";          # where to store temporary metadata
$client_script = "paratest.client.pl";
$rem_exe = "ssh";
$pwd = `pwd`; chomp $pwd;
$summary_len = 2;
$sleep_time = 1;                       # polling time (sec) to distribute work
$incl_futures = 0;
$filedist = 0;
$valgrind = 0;
$suppressions = "";

$localhost = `uname -n`;
($localnode, $junk) = split (/\./, $localhost, 2);
chomp $localnode;


my (@testdir_list, @node_list, $starttime, $endtime);

sub systemd {
    local ($cmd) = @_;
    print "$cmd\n" if $debug;
    system ($cmd);
}


# Collect individual logs into one final one.
sub collect_logs {
    local ($fin_log, @logs) = @_;
    local ($len, $successes, $failures, $futures);
    local ($grep_summ, $head_opts);

    print "collecting logs\n" if $debug;

    if ($platform eq "linux") {
        $head_opts = "-q";                     # quiet mode
    }

    systemd ("echo \\[Parallel testing started at $starttime\\] > $fin_log");
    print "Collecting logs to $fin_log\n" if $verbose;
    foreach $log (@logs) {
        if (-e $log) {
            print "Merging $log\n" if $verbose;
            open GLOG, $log or die "Cannot open log '$log'\n";
            $len = 0;
            while (<GLOG>) {
                last if (/^\[Test Summary/);
                $len++;
            }
            close GLOG;

            print "\nlen = $len\n" if $debug;
            systemd ("head $head_opts -n $len $log >> $fin_log");
            unlink $log if (-e $log);
            unlink "$log.summary" if (-e "$log.summary");
        }
    }

    local ($date) = `date +"%y%m%d.%H%M%S"`;
    chomp $date;

    # Generate the summary info
    systemd ("echo \\[Parallel testing started at $starttime\\] >> $fin_log");
    systemd ("echo \\[Parallel testing ended at $endtime\\] >> $fin_log");

    # Generate summary file
    $summ_log = "$fin_log.summary";
    unlink $summ_log if (-e $summ_log);
    systemd ("echo \\[Test Summary - $date\\] > $fin_log.summary");
    systemd ("grep -a '^\\[Error' $fin_log >> $fin_log.summary");
    if (!($suppressions eq "")) {
        systemd ("Bin/filterSuppressions.pl $suppressions $fin_log.summary");
    }
    systemd ("grep -a '^Future' $fin_log >> $fin_log.summary");

    # Count stuff
    $successes = `grep -a "^\\[Success matching" $fin_log | wc -l`;
    $successes =~ s/\s//g;
    ($successes, $junk) = split (/\s+/, $successes, 2);
    $failures = `grep -a "^\\[Error" $fin_log.summary | wc -l`;
    $failures =~ s/\s//g;
    ($failures, $junk) = split (/\s+/, $failures, 2);
    $futures = `grep -a "^Future" $fin_log.summary | wc -l`;
    $futures =~ s/\s//g;
    ($futures, $junk) = split (/\s+/, $futures, 2);
    systemd ("echo \\[Summary: \\#Successes = $successes \\| \\#Failures = $failures \\| \\#Futures = $futures\\] >> $fin_log.summary");
    systemd ("echo \\[END\\] >> $fin_log.summary");

    systemd ("cat $fin_log.summary >> $fin_log");

    print "\n[Summary: #Successes = $successes | #Failures = $failures | #Futures = $futures]\n";

}


# Return a list of IDs of nodes ready to work
sub free_workers {
    local (@readyv, @readyids, $node, $id);
    print "checking for available workers\n" if $debug;

    opendir WORKDIR, "$synchdir";
    @readyv = readdir WORKDIR;
    closedir WORKDIR;

    foreach $ready (@readyv) {
        next if ($ready =~ /^\./);
        ($node, $id) = split (/\./, $ready, 2);
        push @readyids, $id;
    }

    print ".";
    return @readyids;
}


# While there is still work to do, continually feed the client nodes
# bits of work. If the file "PARAHALT" exists in the test directory,
# distribution of work stops.  This is one hack to stop the testing.
# Of course, you'll have to wait on the client processes or kill them
# manually.
sub feed_nodes {
    local (@readyidv, $logfile, @logs, $testdir, $node, $rem_cmd);
    $| = 1;    # autoflush stdout

    print "about to start distributing work\n" if $debug;

    @testdir_list = sort @testdir_list;
    print $#node_list+1; print " worker(s) (@node_list)\n";
    print $#testdir_list+1; print " test(s) (@testdir_list)\n";

    while (($#testdir_list >= 0) &&       # while still have work to do
           !(-e "PARAHALT")) {
        @readyidv = free_workers ();      # get IDs of nodes that are ready

        print @readyidv if $debug;
        print "\n" if ($#readyidv >= 0);
        foreach $readyid (@readyidv) {    # for ready nodes
            next if ($#testdir_list < 0);

            $testdir = $testdir_list[0];
            $node = $node_list[$readyid]; # machine name to rem exec to
            $synchfile = "$synchdir/$node.$readyid";

            # remove synch file before forking work to worker
            unless (-e $synchfile) {
                printf ("Error: synch file '$synchfile' missing\n");
                exit (7);
            }
            unlink $synchfile;

            print "$node <- $testdir ($#testdir_list left)\n";
            $testdirname = $testdir;
            $testdirname =~ s/\//-/g;
            $logfile = "$logdir/$testdirname.$node.log";
            # fork work
            unless ($pid = fork) {        # child
                if ($node eq $localnode) {
                    $rem_exec_cmd = "";
                    $compopts = "\"$compopts\"";
                    $comm = "\"$comm\"";
                } else {
                    $rem_exec_cmd = "$rem_exe $node";
                    $compopts = "\\\"$compopts\\\"";
                    $comm = "\\\"$comm\\\"";
                }
                $rem_cmd = "$rem_exec_cmd $pwd/$client_script $readyid $pwd $testdir $incl_futures $valgrind $compopts $comm";
                if ($verbose) {
                    systemd ($rem_cmd);
                } else {
                    systemd ("$rem_cmd > /dev/null 2>& 1");
                }
                $partial_errors = `grep -a "^\\[Error" $logfile`;
                if ($partial_errors =~ /^\[Error/) {
                    @errors = split /^/m, $partial_errors;
                    chomp $errors[0];
                    print "\n:( " . $errors[0] . "\n";
                } else {
                    print ":)";
                }
                exit (0);
            }

            push @logs, $logfile;
            shift @testdir_list;
        }

        # wait before checking for free workers
        sleep $sleep_time;                          
    }

    # wait for everyone to finish;
    @readyidv = free_workers ();
    while ($#readyidv != $#node_list) {
        sleep $sleep_time;
        @readyidv = free_workers ();
    }

    $endtime = `date`; chomp $endtime;
    collect_logs ($fin_logfile, @logs);
}


# Signal that all nodes are free to do some work by writing their
# synchronization files for them initially.
sub nodes_free {
    local ($id, $node, $fname, $dirv);

    print "making all nodes available to work\n" if $debug;
    # clean synch dir
    opendir WORKDIR, "$synchdir";
    @dirv = readdir WORKDIR;
    closedir WORKDIR;
    foreach $file (@dirv) {
        unlink "$synchdir/$file";
    }

    systemd ("rm -f $synchdir/*");
    # signal that all nodes are free
    for ($id=0; $id<=$#node_list; $id++) {
        $fname = "$node_list[$id].$id";
        systemd ("echo feed me > $synchdir/$fname");
    }
}


# Return true if a *.chpl exists. Otherwise false.
sub chpl_files {
    local (@fnames) = @_;
    local ($found);
    $found = 0;
    foreach $fname (@fnames) {
        if ($fname =~ /NOTEST$/) {
            return 0;
        }
        if ($fname =~ /\.chpl$/) {
            $found = 1;
        }
    }
    return $found;
}


# Gather all the subdirectories into a flat list and return it.
sub find_subdirs {
    local ($targetdir, $level) = @_;
    local ($filen, @cdir, @founddirs, $i);

    print "looking in '$targetdir'\n" if $debug;
    opendir CURRDIR, $targetdir or die "Cannot open directory '$targetdir'\n";
    @cdir = grep !/^\./, readdir CURRDIR;             # curr dir list of files
    closedir CURRDIR;

    if (chpl_files (@cdir)) {                         # if *.chpl files in dir
	push @founddirs, $targetdir;
	print "$targetdir\n" if $debug;
    }
    
    foreach $filen (@cdir) {
	next if ($filen =~ /$dirs_to_ignore/);
	if (-d "$targetdir/$filen") {                 # if dir
	    if ($debug) {for ($i=0; $i<$level; $i++)  {print "    ";}}
	    push @founddirs, find_subdirs ("$targetdir/$filen", $level+1);
        }
    }
    return @founddirs;
}


# Gather the list of files to test and return it.
sub find_files {
    local ($targetdir, $level, $no_futures, $recursive) = @_;
    local ($filen, @cdir, @foundfiles);

    print "looking in '$targetdir'\n" if $debug;
    opendir CURRDIR, $targetdir or die "Cannot open directory '$targetdir'\n";
    @cdir = grep !/^\./, readdir CURRDIR;             # curr dir list of files
    closedir CURRDIR;

    foreach $filen (@cdir) {
	next if ($filen =~ /$dirs_to_ignore/);
	$filepath = "$targetdir/$filen";
	unless (-e "$targetdir/NOTEST") {             # do not ignore this dir?
	    if ($filepath =~ /\.chpl$/) {
		$futuref = $filepath;
		$futuref =~ s/\.chpl$/.future/;
		next if ((-e $futuref) && $no_futures);
		push @foundfiles, $filepath;
	    }
	}

	if ((-d $filepath) && $recursive) {           # if dir and recursive
	    if ($debug) {for ($i=0; $i<$level; $i++)  {print "    ";}}
	    push @foundfiles, find_files ($filepath, $level+1, $no_futures, $recursive);
        }
    }
    return @foundfiles;
}


sub print_help {
    print "Usage: paratest.server.pl [-comm] [-compopts] [-dirfile d] [-filedist] [-futures] [-logfile l] [-nodefile n] [-valgrind] [-help|-h]\n";
    print "    -compopts s: s is a string that is passed with -compopts to start_test.\n";
    print "    -comm s    : s is a setting for CHPL_COMM, e.g., none or gasnet.\n";
    print "    -dirfile  d: d is a file listing directories to test. Default is the current diretory.\n";
    print "    -filedist  : distribute work at the granularity of files (directory granurality is the default).\n";
    print "    -futures   : include .future tests (default is none).\n";
    print "    -logfile  l: l is the output log file. Default is \"user\".\"platform\".log. in the Logs subdirectory.\n";
    print "    -nodefile n: n is a file listing nodes to run on. Default is current node.\n";
    print "    -valgrind  : pass -valgrind to start_test.\n";
}


sub main {
    local ($id, $synchfile);
    
    $user = `whoami`; chomp $user;
    $platform = `../util/platform.pl`; chomp $platform;
    $fin_logfile = "$logdir/$user.$platform.log";      # final log file name
    # $fin_logfile = "$logdir/$user.$platform.log";
    unlink $fin_logfile if (-e $fin_logfile);          # remove final log file
  
    $starttime = `date`; chomp $starttime;

    while ($#ARGV >= 0) {
        $_ = $ARGV[0];
        if (/^-compopts/) {
            shift @ARGV;
            if ($#ARGV >= 0) {
                $compopts = $ARGV[0];
            } else {
                print "missing -compopts arg\n";
                exit (8);
            }
        } elsif (/^-comm/) {
            shift @ARGV;
            if ($#ARGV >= 0) {
                $comm = $ARGV[0];
            } else {
                print "missing -comm arg\n";
                exit (8);
            }
        } elsif (/^-filedist/) {
            $filedist = 1;
        } elsif (/^-dirfile/) {
            shift @ARGV;
            if ($#ARGV >= 0) {
                $dirfile = $ARGV[0];
            } else {
                print "missing -dirfile arg\n";
                exit (8);
            }
        } elsif (/^-futures/) {
            $incl_futures = 1;
        } elsif (/^-logfile/) {
            shift @ARGV;
            if ($#ARGV >= 0) {
                $fin_logfile = $ARGV[0];
            } else {
                print "missing -logfile arg\n";
                exit (8);
            }
        } elsif (/^-suppress/) {
            shift @ARGV;
            if ($#ARGV >= 0) {
                $suppressions = $ARGV[0];
            } else {
                print "missing -suppress arg\n";
                exit (8);
            }
        } elsif (/^-nodefile/) {
            shift @ARGV;
            if ($#ARGV >= 0) {
                $nodefile = $ARGV[0];
            } else {
                print "missing -nodefile arg\n";
                exit (8);
            }
        } elsif (/^-valgrind/) {
            $valgrind = 1;
        } elsif (/^-help|^-h/) {
            print_help;
            exit (9);
        } else {
            print "unknown arg $_\n";
            exit (9);
        }
        shift @ARGV;
    }

    if (defined $nodefile) {
        open nodefile or die "Cannot open node file '$nodefile'\n";
        while (<nodefile>) {
            next if /^$|^\#/;
            chomp;
            push @node_list, $_;
        }
    } else { # else, just current node
        push @node_list, $localnode;
    }

    if (defined $dirfile) {
        open dirfile or die "Cannot open directory file '$dirfile'\n";
        while (<dirfile>) {
            next if /^$|^\#/;
            chomp;
            if ($filedist) {
                push @testdir_list, find_files( $_, 0, !$incl_futures, 0);
            } else {
                push @testdir_list, $_;
            }
        }
    } else { # else, current working dir
        if ($filedist) {
            @testdir_list = find_files (".", 0, !$incl_futures, 1);
        } else {
            @testdir_list = find_subdirs (".", 0);
        }
    }


    unless (-e $logdir) {
        print "Error: log directory $logdir does not exist\n";
        exit (2);
    }
    unless (-e "$synchdir") { 
        systemd ("mkdir $synchdir"); 
    }

    nodes_free ();     # signal that all nodes free
    feed_nodes ();     # parallel testing

    # cleanup - remove synch files and synch dir
    for ($id=0; $id<=$#node_list; $id++) {
        $synchfile = "$synchdir/$node_list[$id].$id";
        unlink $synchfile if (-e $synchfile);
    }
    rmdir $synchdir;
}


main ();
