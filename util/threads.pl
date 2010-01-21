#!/usr/bin/perl

use File::Basename;

$utildirname = dirname($0);

$preset_tasks   = (@ARGV == 1) ? $ARGV[0] : $ENV{'CHPL_TASKS'};
$preset_threads = $ENV{'CHPL_THREADS'};

if ($preset_threads eq "") {
    if ($preset_tasks eq "") {
	$preset_tasks = `$utildirname/tasks.pl`;
	chomp $preset_tasks;
    }

    if ($preset_tasks eq "fifo") {
	$threads = "pthreads";
    } elsif ($preset_tasks eq "mta") {
	$threads = "none";
    } elsif ($preset_tasks eq "muxed") {
	$threads = "soft-threads";
    } elsif ($preset_tasks eq "nanos") {
	$threads = "pthreads";
    } elsif ($preset_tasks eq "none") {
	$threads = "none";
    } elsif ($preset_tasks eq "") {
	$platform = `$utildirname/platform.pl --target`;
	chomp $platform;
	if ($platform =~ /^(mta|xmt(-sim)?)$/) {
	    $threads = "none";
	} elsif ($platform =~ /^x[12]-sim$/) {
	    $threads = "none";
	} else {
	    $threads = "pthreads";
	}
    } else {
	die "Unexpected preset tasks value \"$preset_tasks\".\n";
    }
} else {
    $threads = $preset_threads;
}

if ($preset_tasks ne "") {
  system("$utildirname/check-tasks-threads-compatibility.pl",
	 "$preset_tasks",
	 "$threads") == 0 or exit(1);
}

print "$threads\n";
exit(0);
