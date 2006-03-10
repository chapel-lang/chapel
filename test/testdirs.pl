#!/usr/bin/perl

# Usage: testdirs.pl [file_of_test_directories]
#

$SUCCESS_INDEX = 4;
$FAILURE_INDEX = 8;
$FUTURES_INDEX = 12;


sub collect_logs {
    local ($dir) = @_;
}


if ($#ARGV > 0) {       # if more than one arg
    print STDERR "usage: testdirs.pl [dirfile]\n";
    exit (1);
} else {                # else, either 0 or 1 arg
    if (0 > $#ARGV) {
        push @testdir_list, ".";
    } else {
        $dirfile = $ARGV[0];
        open dirfile or die "Cannot open directory file '$dirfile'\n";
        while (<dirfile>) {
            next if /^$/;
            chomp;
            push @testdir_list, $_;
        }
    }
}

$user = `whoami`;
chomp $user;
$platform = `../util/platform`;
chomp $platform;
$startdir = `pwd`;
chomp $startdir;
$logdir = "$startdir/Logs";
$logfile = "$logdir/$user.$platform.log";
$tmplogfile = "$logdir/$user.$platform.log.tmp";

if (-e $tmplogfile) {
    unlink $tmplogfile;
}

$goods = 0;
$bads = 0;
$todos = 0;
foreach $testdir (@testdir_list) {      # run tests on each dir
    system ("./start_test -startdir $testdir");
    if (-e $logfile) {
        $summary = `grep "^\\[Summary" $logfile`;
        system ("echo [Test directory $testdir] >> $tmplogfile");
        system ("cat $logfile >> $tmplogfile");
        chomp $summary;
        @ssumm = split (/[\s]|\[|\]/, $summary);
        $goods += $ssumm[$SUCCESS_INDEX];
        $bads += $ssumm[$FAILURE_INDEX];
        $todos += $ssumm[$FUTURES_INDEX];
    } else {
        print STDERR "warning: no logfile for test directory '$testdir'\n";
    }
}

$results = "[Summary: #Successes = $goods | #Failures = $bads | #Futures = $todos]";
print "\n$results\n";

$eresults = "\\[Summary: \\#Successes = $goods \\| \\#Failures = $bads \\| \\#Futures = $todos\\]";

unlink $logfile;
system ("mv $tmplogfile $logfile");     # copy temp log back to orig name
system ("echo $eresults >> $logfile");  # add summary

system ("grep \"^\\[Error\" $logfile > $logfile.summary");
system ("grep \"^Future\" $logfile >> $logfile.summary");
system ("echo $eresults >> $logfile.summary");  # add summary
