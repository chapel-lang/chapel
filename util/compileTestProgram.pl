#!/usr/bin/perl

use File::Basename;

$found_test = 0;
$command = "chpl ";
while (@ARGV) {
    $arg = shift @ARGV;
    if ($arg =~ m/.chpl$/) {
        $found_test = 1;

        ($filename, $path) = fileparse($arg);

        $compopts_file = "$path"."COMPOPTS";

        if (-e $compopts_file) {
            $compopts = `cat $compopts_file`;
            chomp $compopts;
        }

        $filebase = $filename;
        $filebase =~ s/.chpl$//;

        $compopts_file = "$path"."$filebase".".compopts";
        
        if (-e $compopts_file) {
            if ($compopts) {
                $compopts .= " ";
            }
            $compopts .= `cat $compopts_file`;
            chomp $compopts;
        }

        @compopts = split / /, $compopts;

        foreach $compopt (@compopts) {
            if ($compopt =~ m/.chpl$/) {
                $command .= "$path";
            }
            $command .= "$compopt ";
        }
        $command .= "$path$filename ";
    } else {
        $command .= "$arg ";
    }
}

die "no test specified" unless $found_test == 1;

print "$command\n";
exec $command;
