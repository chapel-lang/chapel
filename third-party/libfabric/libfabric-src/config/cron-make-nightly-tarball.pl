#!/usr/bin/env perl
#
# Copyright (c) 2016 Cisco Systems, Inc.  All rights reserved.
#
# Script to automate the steps to make pristine nightly tarballs and
# place them in a specified output directory (presumeably where the
# web server can serve them up to the world).  Designed to be invoked
# via cron (i.e., no output unless --verbose is specified).
#
# The specific steps are:
#
# 1. Ensure we have a pristine, clean git tree
# 2. git pull the latest down from upstream
# 3. Use "git describe" to get a unique string to represent this tarball
# 4. If we already have a tarball for this git HEAD in the destination
#    directory, no need to do anything further / quit
# 5. Re-write configure.ac to include the git describe string in the
#    version argument to AC_INIT
# 6. autogen.sh/configure/make distcheck
# 7. Move the resulting tarballs to the destination directory
# 8. Re-create sym links for libfabric-latest.tar.(gz|bz2)
# 9. Re-generate md5 and sh1 hash files
#
# Note that this script intentionally does *not* prune old nightly
# tarballs as result of an OFIWG phone discussion, the conlcusion of
# which was "What the heck; we have lots of disk space.  Keep
# everything."
#

use strict;
use warnings;

use File::Temp qw/ tempdir /;
use File::Basename;
use Getopt::Long;

my $libfabric_dir_arg;
my $download_dir_arg;
my $libfabric_coverity_token_arg;
my $fabtests_coverity_token_arg;
my $logfile_dir_arg;
my $help_arg;
my $verbose_arg;
my $debug_arg;

my $ok = Getopt::Long::GetOptions("libfabric-source-dir=s" => \$libfabric_dir_arg,
                                  "download-dir=s" => \$download_dir_arg,
                                  "libfabric-coverity-token=s" => \$libfabric_coverity_token_arg,
                                  "fabtests-coverity-token=s" => \$fabtests_coverity_token_arg,
                                  "logfile-dir=s" => \$logfile_dir_arg,
                                  "verbose" => \$verbose_arg,
                                  "debug" => \$debug_arg,
                                  "help|h" => \$help_arg,
                                  );

if ($help_arg || !$ok) {
    print "$0 --libfabric-source-dir libfabric_git_tree --download-dir download_tree\n";
    exit($ok);
}

# Sanity checks
die "Must specify --libfabric-source-dir, --download-dir, and --logfile-dir"
    if (!defined($libfabric_dir_arg) || $libfabric_dir_arg eq "" ||
        !defined($download_dir_arg) || $download_dir_arg eq "" ||
        !defined($logfile_dir_arg) || $logfile_dir_arg eq "");
die "$libfabric_dir_arg is not a valid directory"
    if (! -d $libfabric_dir_arg);
die "$libfabric_dir_arg is not libfabric git clone"
    if (! -d "$libfabric_dir_arg/.git" || ! -f "$libfabric_dir_arg/src/fi_tostr.c");
die "$download_dir_arg is not a valid directory"
    if (! -d $download_dir_arg);
die "$logfile_dir_arg is not a valid directory"
    if (! -d $logfile_dir_arg);

$verbose_arg = 1
    if ($debug_arg);

#####################################################################

sub doit {
    my $allowed_to_fail = shift;
    my $cmd = shift;
    my $stdout_file = shift;

    # Redirect stdout if requested
    if (defined $stdout_file) {
        $stdout_file = "$logfile_dir_arg/$stdout_file.log";
        unlink($stdout_file);
        $cmd .= " >$stdout_file";
    } elsif (!$verbose_arg && $cmd !~ />/) {
        $cmd .= " >/dev/null";
    }
    $cmd .= " 2>&1";

    my $rc = system($cmd);
    if (0 != $rc && !$allowed_to_fail) {
        # If we die/fail, ensure to a) restore the git tree to a clean
        # state, and b) change out of the temp tree so that it can be
        # removed upon exit.
        chdir($libfabric_dir_arg);
        system("git clean -dfx 2>&1 > /dev/null");
        system("git reset --hard HEAD 2>&1 > /dev/null");
        chdir("/");

        print "Command $cmd failed: exit status $rc.  Stdout:\n";
        system("cat $stdout_file")
            if (defined($stdout_file) && -f $stdout_file);
        die "Cannot continue";
    }
    system("cat $stdout_file")
        if ($debug_arg && defined($stdout_file) && -f $stdout_file);
}

sub verbose {
    print @_
        if ($verbose_arg);
}

sub git_cleanup {
    verbose("*** Ensuring we have a clean git tree...\n");

    doit(0, "git clean -dfx", "git-clean");
    doit(0, "git reset --hard HEAD", "git-reset");
    doit(0, "git pull", "git-pull");
}

sub get_git_version {
    # Get a git describe id (minus the initial 'v' in the tag name, if any)
    my $version = `git describe --tags --always`;
    chomp($version);

    verbose("*** Git describe: $version\n");
    $version =~ s/^v//;
    $version =~ y/-/./;
    return $version;
}

sub make_tarball {
    my $base_name = shift;
    my $version = shift;
    my $installdir = shift;

    my $configure_args = "CPPFLAGS=-I$installdir/include LDFLAGS=-L$installdir/lib";

    # Read in configure.ac
    verbose("*** Reading version number from configure.ac...\n");
    open(IN, "configure.ac") || die "Can't open configure.ac for reading";
    my $config;
    $config .= $_
    while(<IN>);
    close(IN);

    # Get the original version number
    $config =~ m/AC_INIT\(\[\Q$base_name\E\], \[(.+?)\]/;
    my $orig_version = $1;
    verbose("*** Replacing configure.ac version: $orig_version\n");
    verbose("*** Nightly tarball version: $version\n");

    # Update the version number with the output from "git describe"
    verbose("*** Re-writing configure.ac with git describe results...\n");
    $config =~ s/(AC_INIT\(\[\Q$base_name\E\], \[).+?\]/$1$version]/;
    open(OUT, ">configure.ac");
    print OUT $config;
    close(OUT);

    # Now make the tarball
    verbose("*** Running autogen.sh...\n");
    doit(0, "./autogen.sh", "autogen");
    verbose("*** Running configure...\n");
    doit(0, "./configure $configure_args --prefix=$installdir", "configure");
    verbose("*** Running make install...\n");
    doit(0, "make install", "make-install");

    # Is there already a tarball of this version in the download
    # directory?  If so, just exit now without doing anything.
    if (-f "$download_dir_arg/$base_name-$version.tar.gz") {
        verbose("*** Target tarball already exists: $base_name-$version.tar.gz\n");
        return 0;
    }

    # Note that distscript.pl, invoked by "make dist", checks for a dirty
    # git tree.  We have to tell it that a modified configure.ac is ok.
    # Put the name "configure.ac" in a magic environment variable.
    $ENV{'LIBFABRIC_DISTSCRIPT_DIRTY_FILES'} = "configure.ac";

    verbose("*** Running make distcheck...\n");
    doit(0, "AM_MAKEFLAGS=V=1 DISTCHECK_CONFIGURE_FLAGS=\"$configure_args\" make distcheck", "distcheck");

    delete $ENV{'LIBFABRIC_DISTSCRIPT_DIRTY_FILES'};

    # Restore configure.ac
    verbose("*** Restoring configure.ac...\n");
    doit(0, "git checkout configure.ac");

    # Move the resulting tarballs to the downloads directory
    verbose("*** Placing tarballs in download directory...\n");
    doit(0, "mv $base_name-$version.tar.gz $base_name-$version.tar.bz2 $download_dir_arg");

    return 1;
}

sub write_file {
    my $filename = shift;
    my $str = shift;

    open(OUT, ">$filename") || die "Can't write to $filename";
    print OUT $str;
    close(OUT);
}

sub submit_to_coverity {
    my $project_name = shift;
    my $version = shift;
    my $configure_args = shift;
    my $coverity_token = shift;

    verbose("*** Preparing/submitting to Coverity...\n");

    # The coverity script will be in the same directory as this script
    my $dir = dirname($0);
    my $base_name = $project_name;
    $base_name =~ s/^ofiwg(%2F|-)([a-z]+)$/$2/;
    my $cmd = "$dir/cron-submit-coverity.pl " .
        "--filename $download_dir_arg/$base_name-$version.tar.bz2 " .
        "--coverity-token $coverity_token " .
        "--make-args=-j8 " .
        "--configure-args=\"$configure_args\" " .
        "--project=$project_name ";

    $cmd .= "--verbose "
        if ($verbose_arg);
    $cmd .= "--debug "
        if ($debug_arg);
    $cmd .= "--logfile-dir=$logfile_dir_arg"
        if (defined($logfile_dir_arg));

    # Coverity script will do its own logging
    doit(0, $cmd);
}

#####################################################################

# Create a temporary directory to install into
my $installdir = tempdir(CLEANUP => 1);

verbose("*** Building libfabric...\n");
chdir($libfabric_dir_arg);
git_cleanup();
my $libfabric_version = get_git_version();
my $rebuilt_libfabric = make_tarball("libfabric",
				     $libfabric_version, $installdir);

verbose("\n\n*** Building fabtests...\n");
chdir('fabtests');
my $fabtests_version = get_git_version();
my $rebuilt_fabtests = make_tarball("fabtests",
                                    $fabtests_version, $installdir);

if ($rebuilt_libfabric || $rebuilt_fabtests) {
    # Re-generate hashes
    verbose("*** Re-generating md5/sha1sums...\n");
    chdir($download_dir_arg);
    doit(0, "md5sum libfabric*tar* fabtests*tar* > md5sums.txt");
    doit(0, "sha1sum libfabric*tar* fabtests*tar* > sha1sums.txt");

    # Write latest-snapshot.txt files
    verbose("** Re-writing latest-snapshot.txt file...\n");
    my $str = "libfabric-$libfabric_version\n" .
        "fabtests-$fabtests_version\n";
    write_file("$download_dir_arg/latest-snapshot.txt", $str);

    verbose("** Re-writing latest-snapshot.txt file...\n");
    my $dirname = basename($download_dir_arg);
    $str = "$dirname/libfabric-$libfabric_version\n" .
        "$dirname/fabtests-$fabtests_version\n";
    write_file("$download_dir_arg/../latest-snapshot.txt", $str);
}

# Run the coverity script if requested
if (defined($libfabric_coverity_token_arg) && $rebuilt_libfabric) {
    submit_to_coverity("ofiwg%2Flibfabric", $libfabric_version,
            "--enable-sockets --enable-udp --enable-verbs --enable-usnic",
            $libfabric_coverity_token_arg);
}
if (defined($fabtests_coverity_token_arg) && $rebuilt_fabtests) {
    submit_to_coverity("ofiwg%2Ffabtests", $fabtests_version,
            "CPPFLAGS=-I$installdir/include LDFLAGS=-L$installdir/lib",
            $fabtests_coverity_token_arg);
}

# All done
verbose("*** All done / nightly tarball\n");
exit(0);
