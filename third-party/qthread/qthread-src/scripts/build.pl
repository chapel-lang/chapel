#!/usr/bin/perl

use strict;
use warnings;

use Cwd qw/getcwd/;

# Setup configuration options
my @default_conf_names = ('compat', 'unpooled', 'opt', 'nemesis', 'lifo', 'mutexfifo', 'slowcontext', 'shep_profile', 'lock_profile', 'steal_profile', 'tc_profile', 'hi_st', 'hi_mt', 'dev');

my %config = (
    default       => '',
    icc           => 'CC=icc CXX=icc',
    oldgcc        => 'CC=gcc34 CXX=g++34',
    compat        => 'CFLAGS="-m32" CXXFLAGS="-m32" LDFLAGS="-m32" CPPFLAGS="-m32"',
    unpooled      => '--disable-pooled-memory',
    opt           => 'CFLAGS="-O3" CXXFLAGS="-O3"',
    nemesis       => '--with-scheduler=nemesis',
    lifo          => '--with-scheduler=lifo',
    mutexfifo     => '--with-scheduler=mutexfifo',
    mtsfifo       => '--with-scheduler=mtsfifo',
    nottingham    => '--with-scheduler=nottingham',
    slowcontext   => '--disable-fastcontext',
    shavit        => '--with-dict=shavit',
    shep_profile  => '--enable-profiling=shepherd',
    lock_profile  => '--enable-profiling=feb',
    steal_profile => '--enable-profiling=steal',
    tc_profile    => '--enable-profiling=threadc',
    hi_st         => '--disable-hardware-atomics --with-scheduler=nemesis',
    hi_mt         => '--disable-hardware-atomics --with-scheduler=sherwood',
    dev           => 'CFLAGS="-g -O0" CXXFLAGS="-g -O0" --enable-debug --enable-guard-pages --enable-asserts --enable-static --disable-shared --enable-valgrind --disable-pooled-memory --enable-aligncheck',
    debug         => 'CFLAGS="-g -O0" CXXFLAGS="-g -O0" --enable-debug --enable-static --disable-shared',
    hwloc         => '--with-topology=hwloc',
    sinc_stats    => '--enable-profiling=sincs',
    oversubscription => '--enable-oversubscription',
    guard_pages => '--enable-guard-pages',
    chapel_default => '--enable-static --disable-shared --enable-condwait-queue --disable-spawn-cache --with-scheduler=nemesis',
);

my @summaries;

# Collect command-line options
my @conf_names;
my @check_tests;
my @user_configs;
my $qt_src_dir = '';
my $qt_bld_dir = '';
my $qt_install_dir = '';
my $repeat = 1;
my $make_flags = '';
my $force_configure = 0;
my $force_clean = 0;
my $print_info = 0;
my $dry_run = 0;
my $quietly = 0;
my $need_help = 0;

if (scalar @ARGV == 0) {
    $need_help = 1;
} else {
    while (@ARGV) {
        my $flag = shift @ARGV;
    
        if ($flag =~ m/--configs=(.*)/) {
            @conf_names = split(/,/, $1);
        } elsif ($flag =~ m/--with-config=(.*)/) {
            push @user_configs, $1;
        } elsif ($flag =~ m/--source-dir=(.*)/) {
            $qt_src_dir = $1;
        } elsif ($flag =~ m/--build-dir=(.*)/) {
            $qt_bld_dir = $1;
        } elsif ($flag =~ m/--install-dir=(.*)/) {
            $qt_install_dir = $1;
        } elsif ($flag =~ m/--repeat=(.*)/) {
            $repeat = int($1);
        } elsif ($flag =~ m/--make-flags=(.*)/) {
            $make_flags = $1;
        } elsif ($flag eq '--force-configure') {
            $force_configure = 1;
        } elsif ($flag eq '--force-clean') {
            $force_clean = 1;
        } elsif ($flag eq '--verbose' || $flag eq '-v') {
            $print_info = 1;
        } elsif ($flag eq '--dry-run') {
            $dry_run = 1;
        } elsif ($flag eq '--quietly') {
            $quietly = 1;
        } elsif ($flag =~ m/--tests=(.*)/) {
            @check_tests = split(/,/,$1) unless ($1 eq 'all')
        } elsif ($flag eq '--help' || $flag eq '-h') {
            $need_help = 1;
        } else {
            print "Unsupported option '$flag'.\n";
            exit(1);
        }
    }
}

# Aggregate configuration options
while (@user_configs) {
    my $user_config = pop @user_configs;
    my $id = scalar @user_configs;
    my $name = "userConfig$id";

    push @conf_names, $name;
    $config{$name} = $user_config;
}
if (scalar @conf_names == 0) { push @conf_names, 'default' };
@conf_names = sort @conf_names;

if ($need_help) {
    print "usage: perl build.pl [options]\n";
    print "Options:\n";
    print "\t--configs=<config-name> comma-separated list of configurations.\n";
    print "\t                        configuration options can be concatenated using\n";
    print "\t                        the '+' operator (e.g., 'conf1+conf2').\n";
    print "\t                        'all' may be used as an alias for all known\n";
    print "\t                        configurations.\n";
    print "\t--with-config=<string>  a user-specified string of configuration\n";
    print "\t                        options. Essentially, this is used to define\n";
    print "\t                        an unnamed 'config', whereas the previous\n";
    print "\t                        uses pre-defined, named configs. This option\n";
    print "\t                        can be used multiple times.\n";
    print "\t--tests=<test-suite>    comma-separated list of test suites. Valid\n";
    print "\t                        test suites are 'basics', 'features', and\n";
    print "\t                        'stress'. The default is to run all three.\n";
    print "\t--source-dir=<dir>      absolute path to Qthreads source.\n";
    print "\t--build-dir=<dir>       absolute path to target build directory.\n";
    print "\t--install-dir=<dir>     absolute path to target installation directory.\n";
    print "\t--repeat=<n>            run `make check` <n> times per configuration.\n";
    print "\t--make-flags=<options>  options to pass to make (e.g. '-j 4').\n";
    print "\t--force-configure       run `configure` again.\n";
    print "\t--force-clean           run `make clean` before rebuilding.\n";
    print "\t--quietly               only report warnings, errors, and summary stats.\n";
    print "\t--verbose\n";
    print "\t--dry-run\n";
    print "\t--help\n";

    print "Configurations:\n";
    my @names = sort keys %config;
    for my $name (@names) {
        print "\t$name:\n\t\t'$config{$name}'\n";
    }

    exit(1);
}

# Clean up and sanity check script options
my $use_all = 0;
foreach my $name (@conf_names) {
    if ($name eq 'all') {
        $use_all = 1;
    } elsif (not exists $config{$name}) {
        my @subconf_names = split(/\+/, $name);
        my @subconf_profiles = ();
        foreach my $subname (@subconf_names) {
            if (exists $config{$subname}) {
                push @subconf_profiles, $config{$subname};
            } else {
                print "Invalid configuration option '$subname'\n";
                exit(1);
            }
        }

        $config{$name} = join(' ', @subconf_profiles);
    }
}
if ($use_all) {
    @conf_names = @default_conf_names;
}

if ($qt_src_dir eq '') {
    $qt_src_dir = getcwd;
    if ((not -e "$qt_src_dir/README") || 
        (my_system("grep -q 'QTHREADS!' $qt_src_dir/README") != 0)) {
        print "Could not find the source directory; try using --source-dir.\n";
        exit(1);
    }
} elsif (not $qt_src_dir =~ m/^\//) {
    print "Specify full path for source dir '$qt_src_dir'\n";
    exit(1);
}

if ($qt_bld_dir eq '') {
    $qt_bld_dir = "$qt_src_dir/build";
} elsif (not $qt_bld_dir =~ m/^\//) {
    print "Specify full path for build dir '$qt_bld_dir'\n";
    exit(1);
}

if ($qt_install_dir eq '') {
} elsif (not $qt_install_dir =~ m/^\//) {
    print "Specify full path for installation dir '$qt_install_dir'\n";
    exit(1);
} else {
    foreach my $name (@conf_names) {
        $config{$name} = join(' ', "--prefix=$qt_install_dir/$name");
    }
}

# Optionally print information about the configuration
if ($print_info) {
    print "Configurations:   @conf_names\n";
    print "Source directory: $qt_src_dir\n";
    print "Build directory:  $qt_bld_dir\n";
}
if (not $qt_install_dir eq '') {
    print "Install directory: $qt_install_dir\n";
}

# Run the test configurations
foreach my $conf_name (@conf_names) {
    run_tests($conf_name);
}

# Print a summary report
print "\n" . '=' x 50;
print "\nSummary:\n";
foreach my $summary (@summaries) {
    print "$summary\n";
}
print '=' x 50 . "\n";

exit(0);

################################################################################

sub run_tests {
    my $conf_name = $_[0];
    my $test_dir = "$qt_bld_dir/$conf_name";
    
    print "\n### Test: $conf_name\n" unless $quietly;
    print "### Build directory: $test_dir\n" unless $quietly;

    # Setup for configuration
    if (not -e "$qt_src_dir/configure") {
        print "###\tGenerating configure script ...\n" if ($print_info);
        my_system("cd $qt_src_dir && sh ./autogen.sh");
    }
    
    # Setup build space
    print "###\tConfiguring '$conf_name' ...\n" unless $quietly;
    my $configure_log = "$test_dir/build.configure.log";
    my_system("mkdir -p $test_dir") if (not -e $test_dir);
    if (not $qt_install_dir eq '') {
        my_system("mkdir -p $qt_install_dir/$conf_name") if (not -e "$qt_install_dir/$conf_name");
    }
    my_system("cd $test_dir && $qt_src_dir/configure $config{$conf_name} 2>&1 | tee $configure_log")
        if ($force_configure || not -e "$test_dir/config.log");
    print "### Log: $configure_log\n" unless $quietly;

    # Build library
    print "###\tBuilding '$conf_name' ...\n" unless $quietly;
    my $build_log = "$test_dir/build.make.log";
    my $build_command = "cd $test_dir";
    $build_command .= " && make clean > /dev/null" if ($force_clean);
    $build_command .= " && make $make_flags 2>&1 | tee $build_log";
    if (not $qt_install_dir eq '') {
        print "###\tInstalling '$conf_name' ...\n" unless $quietly;
        $build_command .= " && make $make_flags install 2>&1 | tee $build_log";
    }
    my_system($build_command);
    if (not $dry_run) {
        my $build_warnings = qx/awk '\/warning:\/' $build_log/;
        if (length $build_warnings > 0) {
            print "Build warnings in config $conf_name! Check log and/or run again with --force-clean and --verbose for more information.\n";
            print $build_warnings;
        }
        my $build_errors = qx/awk '\/error:\/' $build_log/;
        if (length $build_errors > 0) {
            print "Build error in config $conf_name! Check log and/or run again with --verbose for more information.\n";
            print $build_errors;
            exit(1);
        }
    }

    # Build testsuite
    my %failcounts;
    my $failing_tests = 0;
    my $passing_tests = 0;
    my $pass = 1;
    while ($pass <= $repeat) {
        print "###\tBuilding and testing '$conf_name' pass $pass ...\n"
            unless $quietly;
        my $results_log = "$test_dir/build.$pass.results.log";
        print "### Log: $results_log\n" unless $quietly;
        print "### Results for '$conf_name'\n" unless $quietly;
        my $banner = '=' x 50;
        print "$banner\n" unless $quietly;

        my @make_test_suites = ('basics', 'features', 'stress');
        if (scalar @check_tests == 0) { @check_tests = @make_test_suites};
        foreach my $make_test_suite (@check_tests) {
            my $check_command = "cd $test_dir";
            $check_command .= " && make clean > /dev/null" if ($force_clean);
            $check_command .= " && make $make_flags -C test/$make_test_suite check 2>&1 | tee $results_log";
            my_system($check_command);
            if (not $dry_run) {
                my $check_warnings = qx/awk '\/warning:\/' $results_log/;
                if (length $check_warnings > 0) {
                    print "Build warnings in config $conf_name! Check log and/or run again with --force-clean and --verbose for more information.\n";
                    print $check_warnings;
                }
                my $check_errors = qx/awk '\/error:\/' $results_log/;
                if (length $check_errors > 0) {
                    print "Build error in config $conf_name! Check log and/or run again with --verbose for more information.\n";
                    print $check_errors;
                    exit(1);
                }

                # Display filtered results
                my $digest = qx/grep 'tests passed' $results_log/;
                my $digest_msg = '';
                if ($digest eq '') {
                    $digest = qx/grep '# PASS:' $results_log/;
                }
                if ($digest eq '') {
                    $digest = qx/grep 'tests failed' $results_log/; chomp($digest);
                    $digest =~ /([0-9]+) of ([0-9]+) tests failed/;
                    $failing_tests += $1;
                    $passing_tests += $2 - $1;
                    my $fails = qx/awk '\/FAIL\/{print \$2}' $results_log/;
                    my $fail_list .= join(',', split(/\n/, $fails));
                    foreach my $test (split(/\n/, $fails)) {
                        $failcounts{$test} ++;
                    }
                    $digest_msg = $failing_tests . ' tests failed';
                    $digest_msg .= " ($fail_list)";
                } else {
                    chomp $digest;
                    $digest = qx/grep 'All .* tests passed' $results_log/;
                    if ($digest eq '') {
                        $digest = qx/grep 'TOTAL:' $results_log/;
                        $digest =~ /TOTAL: ([0-9]+)/;
                        $passing_tests += $1;
                    } else {
                        $digest =~ /All ([0-9]+) tests passed/;
                        $passing_tests += $1;
                    }
                    $digest_msg = $passing_tests . ' tests passed';
                }
                print "$digest_msg - $make_test_suite\n" unless $quietly;
            }
        }
        print "$banner\n" unless $quietly;

        $pass++;
    }
    if (not $dry_run) {
        my $summary = sprintf("%17s: ", $conf_name);
        if ($failing_tests eq 0) {
            $summary .= "All $passing_tests tests passed";
        } elsif ($passing_tests eq 0) {
            $summary .= "All $failing_tests tests FAILED!!!";
        } else {
            $summary .= "$passing_tests test".(($passing_tests!=1)?"s":"")." passed, ";
            $summary .= "$failing_tests test".(($failing_tests!=1)?"s":"")." failed (";
            foreach my $test (keys(%failcounts)) {
                $summary .= "$test:$failcounts{$test} ";
            }
            chop($summary);
            $summary .= ")";
        }
        push @summaries, $summary;
    }
}

sub my_system {
    my $command = $_[0];

    $command .= " > /dev/null" if (not $print_info);
    print "\t\$ $command\n" if ($print_info);

    my $status = system($command) if (not $dry_run);

    return $status;
}
# vim:expandtab
