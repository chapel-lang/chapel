#!/usr/bin/perl

#
# In general this value should be set to the SVN revision number
# at the time of the last release.
#
$LAST_RELEASE_BUILD_NUM = 13146;
#
# For this first stage of converting to subversion, I've set it
# artificially since all of the automated CVS updates to CVSLOG 
# and BUILD_VERSION were counted as revisions, doubling the count
# compared to how we normally count BUILD_VERSIONS
#

$chplhome = $ENV{'CHPL_HOME'};
$build_version_file_dir = $ARGV[0];

if ($build_version_file_dir eq "") {
    print "usage: computeBuildVersion.pl <directory to place BUILD_VERSION";
    exit(1);
}

$build_version_file = "$build_version_file_dir/BUILD_VERSION";

if (-r "$build_version_file") {
    $last_build_version = `cat $build_version_file`;
    chomp($last_build_version);
} else {
    $last_build_version = "!!!";
}

if (defined $ENV{'CHPL_DEVELOPER'} &&
    defined $ENV{'CHPL_HOME'} &&
    -e "$chplhome/.svn") {

    $revline = `svn info $ENV{'CHPL_HOME'} | grep Revision`;
    chomp($revline);

    if ($revline =~ m/Revision: (\d*)/) {
        $CURRENT_BUILD_NUM = $1;
        $build_version = $CURRENT_BUILD_NUM - $LAST_RELEASE_BUILD_NUM;
    } else {
        print "ERROR: computeBuildVersion.pl failed!";
        exit(1);
    }
} else {
    $build_version = "-999";
}

if ($build_version eq $last_build_version || 
    ($build_version eq "-999" && $last_build_version ne "!!!")) {
#    print "Build version is up-to-date\n";
} else {
#    print "Updating build version ($build_version != $last_build_version)\n";
    open BUILD_VERSION, ">$build_version_file" or die "can't open $build_version_file for writing $!";
    print BUILD_VERSION "$build_version\n";
    close BUILD_VERSION;
}
