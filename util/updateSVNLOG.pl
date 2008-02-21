#!/usr/bin/perl

$chplhome = $ENV{'CHPL_HOME'};
if ($chplhome eq "") {
    print "Error: CHPL_HOME must be set to use updateSVNLOG.pl\n";
}

$svnlogfile = "$chplhome/SVNLOG";
$svnnumfile = "$chplhome/.SVNLOG-revnum";
$completelog = 0;

open SVNLOGNUM, "$svnnumfile" or $completelog = 1;
open SVNLOG, "$svnlogfile" or $completelog = 1;

if ($completelog == 0) {
    my @svnlognum = <SVNLOGNUM>;
    close(SVNLOGNUM);
    close(SVNLOG);
    $revnumline = $svnlognum[0]; chomp($revnumline);
    if ($revnumline =~ m/r(\d+)$/) {
        $completelog = 0;
        $revnum = $1;
    } else {
        $completelog = 1;
    }
}

$revline = `svn info $chplhome | grep Revision`;
chomp($revline);

if ($revline =~ m/Revision: (\d*)/) {
    $currentrevnum = $1;
} else {
    print "ERROR: couldn't determine revision number of $chplhome\n";
    exit(1);
}

if ($completelog == 1) {
    print "SVNLOG being recomputed from scratch\n";
    `cd $chplhome && svn log --verbose --incremental -r1:$currentrevnum > SVNLOG`;
} else {
    if ($revnum eq $currentrevnum) {
        print "SVNLOG is already up-to-date\n";
    } else {
        print "SVNLOG being updated from $revnum to $currentrevnum\n";
        `cd $chplhome && svn log --verbose --incremental -r$revnum:$currentrevnum >> SVNLOG`;
    }
}

open SVNLOGNUM, ">$svnnumfile" or die "Cannot open $svnnumfile for writing";
print SVNLOGNUM "r$currentrevnum\n";
close (SVNLOGNUM);
