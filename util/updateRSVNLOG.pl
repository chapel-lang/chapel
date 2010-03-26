#!/usr/bin/perl
# Changes: Modifed original script to print the log in the reverse order :)

$chplhome = $ENV{'CHPL_HOME'};
if ($chplhome eq "") {
    print "Error: CHPL_HOME must be set to use updateRSVNLOG.pl\n";
    exit(0);
}

$chplsvn = "https://chapel.svn.sourceforge.net/svnroot/chapel";

$svnlogfile = "$chplhome/RSVNLOG";
$svnnumfile = "$chplhome/.RSVNLOG-revnum";
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

#
# if set to 1, this will only update to match the user's local version;
# currently the default is to update to match the latest version that's
# committed
#
$update_to_users_local_version = 0;

if ($update_to_users_local_version == 0) {
    $svninfoflags = "-rHEAD";
}

$revline = `svn info $svninfoflags $chplsvn | grep Revision`;
chomp($revline);

if ($revline =~ m/Revision: (\d*)/) {
    $currentrevnum = $1;
} else {
    print "ERROR: couldn't determine revision number of $chplsvn\n";
    exit(1);
}

if ($completelog == 1) {
    print "RSVNLOG being recomputed from scratch\n";
    `cd $chplhome && svn log --verbose --incremental -r$currentrevnum:1 $chplsvn > RSVNLOG`;
} else {
    if ($revnum eq $currentrevnum) {
        print "RSVNLOG is already up-to-date\n";
    } else {
        $revnum += 1;
        print "RSVNLOG being updated from $revnum to $currentrevnum\n";
        `cd $chplhome && svn log --verbose --incremental -r$currentrevnum:$revnum $chplsvn > .RSVNLOGtmp`;
	`cd $chplhome && cat RSVNLOG >> .RSVNLOGtmp`;
	`cd $chplhome && mv .RSVNLOGtmp RSVNLOG`;
    }
}

open SVNLOGNUM, ">$svnnumfile" or die "Cannot open $svnnumfile for writing";
print SVNLOGNUM "r$currentrevnum\n";
close (SVNLOGNUM);
