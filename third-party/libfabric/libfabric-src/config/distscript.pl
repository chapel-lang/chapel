#!/usr/bin/env perl

use strict;
use warnings;

use POSIX qw(strftime);

my $builddir = shift(@ARGV);
my $version = shift(@ARGV);

# Sanity check
die "Must specify builddir, version"
    if (!defined($builddir) || !$builddir || ! -d $builddir ||
        !defined($version) || !$version);

my $today = strftime "%Y-%m-%d", localtime;

#------------------------------------------------------------------------------
# Helper function to re-write files
#------------------------------------------------------------------------------

sub subst {
    my $file = shift;

    my $orig;
    open(IN, $file) || die "Can't read $file: $!";
    $orig .= $_
        while (<IN>);
    close(IN);

    my $copy = $orig;
    $copy =~ s/\@VERSION\@/Libfabric v$version/g;
    $copy =~ s/\@DATE\@/$today/g;

    # Note that there appears to be a bug in some versions of Pandoc
    # that will escape the appearance of @ in generated man pages
    # (e.g., in the "@VERSION@" that appears in the man page version
    # field).  So rather than be clever in the regexp's above, do the
    # simple/clear thing and repeat the same regexp's as above, but
    # with double-escaped @'s.
    $copy =~ s/\\\@VERSION\\\@/Libfabric v$version/g;
    $copy =~ s/\\\@DATE\\\@/$today/g;

    if ($copy ne $orig) {
        print "*** VERSION/DATE-ifying $file...\n";
        open(OUT, ">$file") || die "Can't write to $file: $!";
        print OUT $copy;
        close(OUT);
    }
}

###############################################################################
# Check to see that the source tree is clean / has no local changes
###############################################################################

if (-d ".git") {
    open(GIT_STATUS, "git status --porcelain|") ||
        die "Can't run git status to verify that the source tree is clean";
    my $clean = 1;

    # Allow the caller to specify a list of dirty files that can be
    # skipped in this git cleanliness check.  The nightly tarball
    # script uses this mechanism (because it needs to modify the
    # version in configure.ac).  Others may use this mechanism, too --
    # it allows them the safety of checking that the *rest* of their
    # tree is git clean (e.g., perhaps they have modified
    # libfabric.spec.in with a local release number).
    my @dirty_files;
    @dirty_files = split(/\s+/, $ENV{'LIBFABRIC_DISTSCRIPT_DIRTY_FILES'})
        if (exists($ENV{'LIBFABRIC_DISTSCRIPT_DIRTY_FILES'}));

    while (<GIT_STATUS>) {
        chomp;
        if ($_ =~ m/^([^?! ].|.[^?! ]) (.+)$/) {
            my $file = $2;
            print "*** WARNING: found modified file in source tree: $file\n";

            my $found = 0;
            foreach my $dirty_file (@dirty_files) {
                if ($file eq $dirty_file) {
                    print "*** ...but an environment variable override says that this is ok!\n";
                    $found = 1;
                    last;
                }
            }

            $clean = 0
                if (!$found);
        }
    }
    close(GIT_STATUS);
    if (!$clean) {
        print "*** WARNING: Source tree is not clean.\n";
        die "Refusing to make tarball";
    }
}

###############################################################################
# Change into the new distribution tree
###############################################################################

chdir($builddir);
subst("README");

chdir("man");
opendir(my $dh, ".") || die "Can't open man directory: $!";
my @subdirs = grep { /man\d+/ && -d "./$_" } readdir($dh);
closedir $dh;

foreach my $dir (@subdirs) {
    opendir(my $dh, $dir) || die "Can't open man/$dir directory: $!";
    my @files = grep { /\.\d$/ && -f "$dir/$_" } readdir($dh);
    closedir $dh;

    foreach my $file (@files) {
        subst("$dir/$file");
    }
}

exit(0);
