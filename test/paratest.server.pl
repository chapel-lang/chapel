#!/usr/bin/perl

# Usage: paratest.server.pl [-dirfile d] [-nodefile n] [-logfile l] [-help|-h]
#   -dirfile  d: d is a file listing directories to test. Default is ".". Lines
#                beginning with # are ignored.
#   -nodefile n: n is a file listing nodes to run on. Default is current node.
#                To run multiple processes on MP nodes, list the node multiple
#                times, once for each desired process. Lines with are ignored.
#   -logfile  l: l is the output logfile. Default is "user"."platform".log.
#
# Requirements:
#  - paratest.server.pl is run in $CHPLHOME/test.
#  - Chapel compiler bin as $CHPLHOME/compiler/"platform"/chpl.
#  - Scripts start_test and paratest.client.pl in the same directory
#      as paratest.server.pl. It will create a temporary directory
#      (.synch) to synchronize the distribution of work to the client
#      processes.
#  - Be able to run start_test remotely. This may include the following:
#    - Chapel built without node-specific local temporary directories.
#        Nodes must be able to execute start_test. For example, the
#        start_test script may invoke the compiler as ../compiler/linux/chpl.
#        If Chapel is built with CHPLDEVTMP defined to a machine-specific local
#        tmp directory (e.g., /ptmp), the script may not be able to execute
#        chpl on a different machine. A good check is to run start_test with 
#        a different machine to see if it can run successfully.
#    - To run jobs remotely over a secured network without having to
#      type a password each time, use ssh-agent and ssh-add. See
#      http://upc.lbl.gov/docs/user/sshagent.html for a short tutorial.
#

$debug = 0;
$verbose = 0;

$dirs_to_ignore = "CVS|Bin|Logs|Samples|Share|OUTPUT|RCS";

$logdir = "Logs";
$synchdir = "$logdir/.synch";
#$compiler = "../compiler/chpl";
$cmd = "paratest.client.pl";
#$rem_exe = "ssh -v";
$rem_exe = "ssh";
$pwd = `pwd`; chomp $pwd;
$summary_len = 2;
$sleep_time = 5;                                 # time (sec) between checks

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

    if ($platform eq "linux") {
        $head_opts = "-q";                     # quiet mode
    }

    systemd ("echo \\[Parallel testing started at $starttime\\] > $fin_log");
    print "Collecting logs to $fin_log\n" if $verbose;
    foreach $log (@logs) {
        if (-e $log) {
            print "Merging $log\n" if $verbose;

            # output of grep seem broken on sunos; hack one for now
            # $len = `wc -l $log`;
            # ($len, $junk) = split (/\s/, $len, 2);
            #
            # $grep_out = `grep -n "^\\[Test Summary" $log`;
            # print "grep_out = $grep_out\n" if $debug;
            # if ($grep_out =~ /Summary/) {
            #    ($len, $junk) = split (/:/, $grep_out, 2);
            #    $len--;
            #}

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
    systemd ("echo \\[Test Summary - $date\\] >> $fin_log");
    $successes = `grep -a "^\\[Success matching" $fin_log | wc -l`;
    $successes =~ s/\s//g;
    ($successes, $junk) = split (/\s+/, $successes, 2);
    $failures = `grep -a "^\\[Error" $fin_log | wc -l`;
    $failures =~ s/\s//g;
    ($failures, $junk) = split (/\s+/, $failures, 2);
    $futures = `grep -a "^Future" $fin_log | wc -l`;
    $futures =~ s/\s//g;
    ($futures, $junk) = split (/\s+/, $futures, 2);
    systemd ("echo \\[Summary: \\#Successes = $successes \\| \\#Failures = $failures \\| \\#Futures = $futures\\] >> $fin_log");
    systemd ("echo \\[END\\] >> $fin_log");

    print "\n[Summary: #Successes = $successes | #Failures = $failures | #Futures = $futures]\n";

    # Generate summary file
    systemd ("echo \\[Test Summary - $date\\] > $fin_log.summary");
    systemd ("grep '^\\[Error' $fin_log >> $fin_log.summary");
    systemd ("grep '^Future' $fin_log >> $fin_log.summary");
    systemd ("echo \\[Summary: \\#Successes = $successes \\| \\#Failures = $failures \\| \\#Futures = $futures\\] >> $fin_log.summary");
    systemd ("echo \\[END\\] >> $fin_log.summary");
}


# Return a list of IDs of nodes ready to work
sub free_workers {
    local (@readyv, @readyids, $node, $id);

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


sub feed_nodes {
    local (@readyidv, $logfile, @logs, $testdir, $node);
    # local ($compiler) = `which chpl`;
    # chomp $compiler;
    # local ($compiler) = "../compiler/$platform/chpl";

    $| = 1;    # autoflush stdout

    @testdir_list = sort @testdir_list;
    print $#node_list+1; print " worker(s) (@node_list)\n";
    print $#testdir_list+1; print " test(s) (@testdir_list)\n";

    while ($#testdir_list >= 0) {         # while still have work to do
        @readyidv = free_workers ();      # get IDs of nodes that are ready

        print "\n" if ($#readyidv >= 0);
        foreach $readyid (@readyidv) {    # for ready nodes
            next if ($#testdir_list < 0);
            $testdir = $testdir_list[0];
            $node = $node_list[$readyid]; # machine name to rem exec to
            $synchfile = "$synchdir/$node.$readyid";

            # remove synch file before forking work to worker
            unless (-e $synchfile) {
                printf ("Error: cannot remove synch file '$synchfile'\n");
                exit (7);
            }
            unlink $synchfile;

            print "$node <- $testdir ($#testdir_list left)\n";
            # print ("$rem_exe $node $pwd/$cmd $readyid $pwd $testdir\n");

            # fork work
            unless ($pid = fork) {        # child
                if ($verbose) {
                    # systemd ("$rem_exe $node $pwd/$cmd $readyid $pwd $testdir $compiler");
                    systemd ("$rem_exe $node $pwd/$cmd $readyid $pwd $testdir");
                } else {
                    # systemd ("$rem_exe $node $pwd/$cmd $readyid $pwd $testdir $compiler >& /dev/null");
                    systemd ("$rem_exe $node $pwd/$cmd $readyid $pwd $testdir > /dev/null 2>& 1");
                }
                exit (0);
            }
            # print $testdir\n";
            shift @testdir_list;

            $testdir =~ s/\//-/g;
            $logfile = "$logdir/$testdir.$node.log";
            push @logs, $logfile;
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
        # print "$fname\n";
        systemd ("echo feed me > $synchdir/$fname");
    }
}


sub chpl_files {
    local (@fnames) = @_;
    foreach $fname (@fnames) {
        if ($fname =~ /\.chpl$/) {
            # printf "found .chpl\n";
            return 1;
        }
    }
    return 0;
}


sub find_subdirs {
    local ($targetdir, $level) = @_;
    local ($filen, @cdir, $subdir, @subdirs, @founddirs, @subdirs, $i);

    print "looking in '$targetdir'\n" if $debug;
    chdir $targetdir;
    opendir CURRDIR, "." or die "Cannot open directory '$targetdir'\n";
    #@cdir = grep !/^\.\.?$/, readdir CURRDIR;
    @cdir = grep !/^\./, readdir CURRDIR;             # curr dir list of files
    closedir CURRDIR;

    # print "$targetdir: ";
    if (chpl_files (@cdir)) {                         # if *.chpl files in curr
        unless ($targetdir eq ".") {chdir "..";}
        return (".");
    } else {
        foreach $filen (@cdir) {
            next if ($filen =~ /$dirs_to_ignore/);
            if (-d $filen) {                          # if dir
                if ($debug) {for ($i=0; $i<$level; $i++)  {print "    ";}}
                @subdirs = find_subdirs ($filen, $level+1);
                if ($#subdirs >= 0) {
                    foreach $subdir (@subdirs) {
                        push @founddirs, "$filen/$subdir";
                    }
                } else {
                    push @founddirs, $filen;            # filesys leaf dir
                }
            }
        }
    }
    unless ($targetdir eq ".") {chdir "..";}
    return @founddirs;
}


sub print_help {
    print "Usage: paratest.server.pl [-dirfile d] [-nodefile n] [-help|-h]\n";
    print "    -dirfile  d: d is a file listing directories to test. Default is the current diretory.\n";
    print "    -nodefile n: n is a file listing nodes to run on. Default is current node.\n";
    print "    -logfile  l: l is the output log file. Default is \"user\".\"platform\".log.\n";
}


sub main {
    local ($id, $synchfile);
    
    $user = `whoami`; chomp $user;
    $platform = `../util/platform`; chomp $platform;
    $fin_logfile = "$logdir/$user.$platform.log";      # final log file name
    # $fin_logfile = "$logdir/$user.$platform.log";
    unlink $fin_logfile if (-e $fin_logfile);          # remove final log file
  
    $starttime = `date`; chomp $starttime;

    while ($#ARGV >= 0) {
        $_ = $ARGV[0];
        if (/^-dirfile/) {
            shift @ARGV;
            if ($#ARGV >= 0) {
                $dirfile = $ARGV[0];
            } else {
                print "missing -dirfile arg\n";
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
        } elsif (/^-logfile/) {
            shift @ARGV;
            if ($#ARGV >= 0) {
                $fin_logfile = $ARGV[0];
            } else {
                print "missing -logfile arg\n";
                exit (8);
            }
        } elsif (/^-help|^-h/) {
            print_help;
            exit (9);
        } else {
            print "unknown arg $_\n";
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
        # @node_list = sort @node_list;
    } else { # else, just current node
        local ($node) = `uname -n`;
        ($node, $junk) = split (/\./, $node, 2);
        push @node_list, $node;
    }

    if (defined $dirfile) {
        open dirfile or die "Cannot open directory file '$dirfile'\n";
        while (<dirfile>) {
            next if /^$|^\#/;
            chomp;
            push @testdir_list, $_;
            # print "$_\n";
        }
    } else { # else, current working dir
        @testdir_list = find_subdirs (".", 0);
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
