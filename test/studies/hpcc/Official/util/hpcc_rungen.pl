#!/usr/bin/perl -w

#
# HPCC rungen script developed by Steve Whalen, Cray Inc.
#

use strict;
use Getopt::Long qw(:config no_ignore_case);
use Pod::Usage;

sub usage()
{
    print STDERR "Usage: hpl_rungen.pl (-n nprocs|--nprocs=nprocs)\n" .
                 "                     (-m mem|--mem=mem)\n" .	
                 "                     [-N ppn|--ppn=ppn]\n" .
                 "                     [-b nb|--nb=nb]\n" .
                 "                     [--[no]optdist]\n" .
                 "                     [-v|--verbose]\n" .
                 "                     [-q|--quiet]\n";
    print STDERR "  nprocs = number of MPI processes (aka core count)\n";
    print STDERR "  mem    = memory usage per process in MB\n";
    print STDERR "           or GB with \"G\" suffix\n";
    print STDERR "  ppn    = processors per node (cores per socket)\n";
    print STDERR "           default is 1\n";
    print STDERR "  nb     = blocksize\n";
    print STDERR "           default is 128\n";
    print STDERR "  --[no]optdist : [don't try to] size problem to distribute evenly over procs\n";
    print STDERR "  -v|--verbose  : default amount of informative messages\n";
    print STDERR "  -q|--quiet    : no informative messages\n";
    exit;
}

&usage() unless (@ARGV > 2);

my ($ncpus, $mem, $ppn, $nb, $optdist, $verbose, $quiet);

# Process args
GetOptions("n|nprocs=i" => \$ncpus,
           "mem=s"      => \$mem,
           "N|ppn:i"    => \$ppn,
           "b|nb:i"     => \$nb,
           "optdist!"   => \$optdist,
           "verbose!"   => \$verbose,
           "quiet"      => \$quiet) or &usage();

&usage() unless (defined($ncpus) && defined($mem));

$ppn = 1 unless (defined($ppn));
$nb = 128 unless (defined($nb));
$optdist = 0 unless (defined($optdist));
$verbose = 1 unless (defined($verbose));
$quiet = 0 unless (defined($quiet));
$verbose = 0 if ($quiet == 1);

print STDERR "Warning: using $ppn cores per socket\n" if ($ppn > 4);

my $nnodes = $ncpus/$ppn;
#my $flag = "-SN";
#$flag = "-VN" if ($ppn > 1);

my $dirname = "$ncpus";
#$dirname .= "vn" if ($ppn == 2);
$dirname .= ".N$ppn";

if ($mem =~ /G/)
{
    $mem = $` * 1024;
}

# Calculate process grid dimensions
my ($P, $Q);
$P = int(sqrt($ncpus));
$Q = $ncpus/$P;
while ( $Q !~ /^\d+$/ )
{
    $P--;
    $Q = $ncpus/$P;
}

# Calculate HPL matrix size
my $lcm = &lcm( $P*$nb, $Q*$nb );
my $N;
if ($optdist)
{
    $N = int(sqrt($mem * 131072 * $ncpus) / $lcm) * $lcm;
} else {
    $N = int(sqrt($mem * 131072 * $ncpus) / $nb) * $nb;
}
$N--;

my $mbperproc = $N * $N / 131072 / $ncpus;

if ($optdist && $verbose)
{
    print "\nOptimal problem size:      N = $N, using ";
    printf '%5.1f', $mbperproc;
    print " MB per process\n";

    print "Next largest optimal size: N = " . ($N + $lcm) . ", using ";
    $mbperproc = ($N + $lcm) * ($N + $lcm) / 131072 / $ncpus;
    printf '%5.1f', $mbperproc;
    print " MB per process\n";
}

# HPL runtime estimates

my $n_tflops = $N*$N*(2*$N/3 + 3/2);

my $target_percent_peak = 80 - int($ncpus/2500);

my $maxtime = 0.0;

if ($verbose)
{
    print "\n\tEstimated HPL runtime in hours:\n\n";
    print "\t               Percent of Peak\n";
    print "\tCPU GHz    " . ($target_percent_peak - 1) . "%    " .
          $target_percent_peak . "%    " . ($target_percent_peak+1) .
          "%    " . ($target_percent_peak+2) . "%\n";
    print "\t-------   -----  -----  -----  -----\n";

    foreach my $cpu_ghz ( (2.1, 2.2, 2.3) )
    {
        print "\t  $cpu_ghz   ";

        foreach my $percent ( ($target_percent_peak - 1,
                               $target_percent_peak,
                               $target_percent_peak + 1,
                               $target_percent_peak + 2) )
        {
            print '  ';
            my $hours = $n_tflops / ($cpu_ghz * 144000000000 * $percent * $ncpus);
            $maxtime = $hours if ($maxtime < $hours);
            printf '%5.2f', $hours;
        }

        print "\n";
    }

    print "\n";
}

# I've had a lot of trouble getting the unex buffer
# large enough to handle an eager cutoff larger than
# 16M, so enforce a 16M cap.
my $eager = 2**(int(log($N/$P * $nb * 8)/log(2)) + 1);
$eager = 2**24 if ($eager > 2**24);

# If a run directory already exists, we won't provide a runscript
if (! -e $dirname)
{
    my $walltime = int($maxtime + 3) . ':00:00';

    mkdir($dirname) or die "Unable to create directory `$dirname'!";

    open(RUNIT, ">$dirname/runit") or die
        "Unable to open file `$dirname/runit'";
    print RUNIT "#!/bin/sh -v
#PBS -l mppwidth=$ncpus
#PBS -l mppnppn=$ppn
#PBS -N hpcc
#PBS -j oe
#PBS -l walltime=$walltime

# cd to directory where job was submitted from
cd \$PBS_O_WORKDIR

export MPICH_PTL_UNEX_EVENTS=40000
export MPICH_PTL_OTHER_EVENTS=40000
export MPICH_UNEX_BUFFER_SIZE=120M
export MPICH_RMA_BUFFER_SIZE=120M
export MPICH_RANK_REORDER_METHOD=1
export MPICH_FAST_MEMCPY=1
export MPICH_PTL_MATCH_OFF=1

aprun -n $ncpus -N $ppn ../../hpcc

cat hpccoutf.txt

mv -f hpccoutf.txt hpccoutf.txt.`date -Iseconds`
" or die "Unable to write to file `$dirname/runit'!";

    close RUNIT or die "Unable to close file `$dirname/runit'!";
}

open(HPLDAT, ">$dirname/hpccinf.txt") or die
    "Unable to open file `$dirname/hpccinf.txt'!";

print HPLDAT "HPLinpack benchmark input file
Innovative Computing Laboratory, University of Tennessee
HPL.out      output file name (if any)
6            device out (6=stdout,7=stderr,file)
1            # of problems sizes (N)
$N       Ns
1            # of NBs
$nb           NBs
0            PMAP process mapping (0=Row-,1=Column-major)
1            # of process grids (P x Q)
$P           Ps
$Q           Qs
16.0         threshold
1            # of panel fact
2            PFACTs (0=left, 1=Crout, 2=Right)
1            # of recursive stopping criterium
4            NBMINs (>= 1)
1            # of panels in recursion
2            NDIVs
1            # of recursive panel fact.
2            RFACTs (0=left, 1=Crout, 2=Right)
1            # of broadcast
3            BCASTs (0=1rg,1=1rM,2=2rg,3=2rM,4=Lng,5=LnM)
1            # of lookahead depth
2            DEPTHs (>=0)
0            SWAP (0=bin-exch,1=long,2=mix)
64           swapping threshold
1            L1 in (0=transposed,1=no-transposed) form
1            U  in (0=transposed,1=no-transposed) form
0            Equilibration (0=no,1=yes)
8            memory alignment in double (> 0)
##### This line (no. 32) is ignored (it serves as a separator). ######
0                               Number of additional problem sizes for PTRANS
1200                            values of N
5                             number of additional blocking sizes for PTRANS
23 33 63 93 186           values of NB
" or die "Unable to write to file `$dirname/hpccinf.txt'!";

close HPLDAT;

sub lcm($$)
{
    my ($g, $t, $a, $b) = (1, 0, @_);

    while ( (0 == ($a & 1)) && (0 == ($b & 1)) )
    {
	$a >>= 1;
	$b >>= 1;
	$g <<= 1;
    }

    while ($a > 0)
    {
	if (0 == ($a & 1)) { $a >>= 1; }
	elsif (0 == ($b & 1)) { $b >>= 1; }
	else
	{
	    $t = abs($a - $b) >> 1;
	    if ($a < $b) { $b = $t; }
	    else { $a = $t; }
	}
    }

    return $_[0]*$_[1]/($b*$g);
}
 
