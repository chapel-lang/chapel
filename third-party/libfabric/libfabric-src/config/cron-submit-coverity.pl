#!/usr/bin/env perl

use warnings;
use strict;

use Getopt::Long;
use File::Temp qw/ tempfile tempdir /;
use File::Basename;

# Coverity changes this URL periodically
my $coverity_tool_url = "https://scan.coverity.com/download/cxx/linux64";

my $filename_arg;
my $coverity_token_arg;
my $project_arg = "ofiwg%2Flibfabric";
my $dry_run_arg = 0;
my $verbose_arg = 0;
my $debug_arg = 0;
my $logfile_dir_arg;
my $configure_args = "";
my $make_args = "-j8";
my $help_arg = 0;

&Getopt::Long::Configure("bundling");
my $ok = Getopt::Long::GetOptions("filename=s" => \$filename_arg,
                                  "coverity-token=s" => \$coverity_token_arg,
                                  "project=s" => \$project_arg,
                                  "logfile-dir=s" => \$logfile_dir_arg,
                                  "configure-args=s" => \$configure_args,
                                  "make-args=s" => \$make_args,
                                  "dry-run!" => \$dry_run_arg,
                                  "verbose!" => \$verbose_arg,
                                  "debug!" => \$debug_arg,
                                  "help|h" => \$help_arg);

$ok = 0
    if (!defined($filename_arg));
$ok = 0
    if (!defined($coverity_token_arg));
$ok = 0
    if (($debug_arg || $verbose_arg) && !defined($logfile_dir_arg));
if (!$ok || $help_arg) {
    print "Usage: $0 --filename=FILENAME --coverity-token=TOKEN [--dry-run] [--verbose] [--help]\n";
    exit($ok);
}

die "Cannot read $filename_arg"
    if (! -r $filename_arg);

$verbose_arg = 1
    if ($debug_arg);

######################################################################

sub verbose {
    print @_
        if ($verbose_arg);
}

# run a command and save the stdout / stderr
sub doit {
    my $allowed_to_fail = shift;
    my $cmd = shift;
    my $stdout_file = shift;

    # Redirect stdout if requested or not verbose
    if (defined($stdout_file)) {
        $stdout_file = "$logfile_dir_arg/$stdout_file.log";
        unlink($stdout_file);
        $cmd .= " >$stdout_file";
    } elsif (!$debug_arg) {
        $cmd .= " >/dev/null";
    }
    $cmd .= " 2>&1";

    my $rc = system($cmd);
    if (0 != $rc && !$allowed_to_fail) {
        # If we die/fail, ensure to change out of the temp tree so
        # that it can be removed upon exit.
        chdir("/");
        die "Command $cmd failed: exit status $rc";
    }
    system("cat $stdout_file")
        if ($debug_arg && defined($stdout_file) && -f $stdout_file);
}

######################################################################

# Make an area to work

my $dir = tempdir(CLEANUP => 1);
chdir($dir);
verbose "*** Working in $dir\n";

######################################################################

# Get the coverity tool, put it in our path

my $new_tool = "$ENV{HOME}/coverity-tool-new.tgz";
my $old_tool = "$ENV{HOME}/coverity-tool.tgz";

verbose "*** Downloading coverity tool\n";
doit(1, "curl --silent $coverity_tool_url --request POST --data \"token=$coverity_token_arg\&project=$project_arg\" -o $new_tool -z $old_tool");
# With the -z option, curl will download the coverity tool only if it
# is newer than what we already have.  If there's no newer file to
# download, it'll just exit(0) (which is a LOT faster, because the
# coverity tool is giant and it takes a long time to download).
doit(1, "mv $new_tool $old_tool")
    if (-f $new_tool);

verbose "*** Extracting coverity tool\n";
doit(0, "tar xf $old_tool");
opendir(my $dh, ".") ||
    die "Can't opendir .";
my @files = grep { /^cov/ && -d "./$_" } readdir($dh);
closedir($dh);

my $cov_dir = "$dir/$files[0]/bin";
$ENV{PATH} = "$cov_dir:$ENV{PATH}";

######################################################################

# Expand the tarball, build it

verbose "*** Extracting tarball\n";
doit(0, "tar xf $filename_arg");
my $base_name = $project_arg;
$base_name =~ s/^ofiwg(%2F|-)([a-z]+)$/$2/;
my $tarball_filename = basename($filename_arg);
$tarball_filename =~ m/^\Q$base_name\E-(.+)\.tar.+$/;
my $version = $1;
chdir("$base_name-$version");

verbose "*** Configuring tarball\n";
doit(0, "./configure $configure_args", "coverity-configure");

verbose "*** Building libtarball\n";
doit(0, "cov-build --dir cov-int make $make_args", "coverity-build");

# Tar up the Coverity results
verbose "*** Tarring up results\n";
doit(0, "tar jcf $version-analyzed.tar.bz2 cov-int");

# If not dry-run, submit to Coverity
if ($dry_run_arg) {
    verbose "*** Would have submitted, but this is a dry run\n";
} else {
    verbose "*** Submitting results\n";
    doit(0, "curl --form token=$coverity_token_arg " .
         "--form email=ofiwg-bot\@openfabrics.org " .
         "--form file=\@$version-analyzed.tar.bz2 " .
         "--form version=$version " .
         "--form description=nightly-master " .
         "https://scan.coverity.com/builds?project=$project_arg",
         "coverity-submit");
}

verbose("*** All done / Coverity\n");

# Chdir out of the tempdir so that it can be removed
chdir("/");

exit(0);
