#!/usr/bin/env perl
#
# runManyTimes.pl <NT> <NIPT> [<COMMAND>]
#
#   NT = number of threads
#   NIPT = number of iterations per thread
#   COMMAND = executable command (default = "a.out")
#
#   runs a command NT*NIPT times by creating NT threads and having
#   each thread run COMMAND for NIPT iterations; standard error and
#   output are saved in the file runManyTimes.out
#

use threads;

die "error: runManyTimes.pl <NT> <NIPT> [<COMMAND>]" unless $#ARGV > 0;

$NT = @ARGV[0];
$NIPT = @ARGV[1];

if ($#ARGV >= 2) {
    @COMMAND = @ARGV[2..$#ARGV];
} else {
    @COMMAND = ("a.out");
}

print "Threads               = $NT\n";
print "Iterations Per Thread = $NIPT\n";
print "Command               = @COMMAND\n";

for ($i = 0; $i < $NT; $i++) {
    @thr[$i] = threads->new(\&com, $i);
}

for ($i = 0; $i < $NT; $i++) {
    @thr[$i]->join;
}

system("cat .runManyTimes.out.* > runManyTimes.out");
system("rm .runManyTimes.out.*");

print "Output left in runManyTimes.out\n";

sub com {
    my $i = @_[0];
    my $hundredths = 1;
    for ($j = 1; $j <= $NIPT; $j++) {
        system("@COMMAND >& .runManyTimes.out.$i.$j");
        system("echo \"----------\" >> .runManyTimes.out.$i.$j");
        while ($j >= ($NIPT / 100 * $hundredths)) {
            printf(".");
            if ($hundredths % 10 == 0) {
                printf("|");
            }
            $hundredths++;
        }
        system("cat .runManyTimes.out.$i.$j >> .runManyTimes.out.$i");
        system("rm .runManyTimes.out.$i.$j");
    }
}
