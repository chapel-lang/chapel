#!/usr/bin/perl

@ARGV == 2 or die "usage: $0 task-model-name threads-model-name\n";

$tasks=$ARGV[0];
$threads=$ARGV[1];

unless (($tasks eq "fifo" && $threads eq pthreads) ||
	($tasks eq "mta" && $threads eq "none") ||
	($tasks eq "muxed" &&
	 $threads =~ /^pthreads|soft-threads$/) ||
	($tasks eq "nanos" && $threads eq "pthreads") ||
	($tasks eq "none" && $threads eq "none")) {
    die "Tasks \"$tasks\" is incompatible with threads \"$threads\".\n";
}

exit(0);
