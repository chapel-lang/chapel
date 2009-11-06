#!/usr/bin/env perl
#   $Source: /var/local/cvs/gasnet/vapi-conduit/contrib/gasnetrun_vapi.pl,v $
#     $Date: 2007/03/14 22:58:29 $
# $Revision: 1.9 $
# Description: GASNet VAPI and IBV spawner
# Terms of use are as specified in license.txt

require 5.004;
use strict;

# Globals
my @mpi_args = ();
my $numproc = undef;
my $numnode = undef;
my $verbose = 0;
my $keep = 0;
my $dryrun = 0;
my $exebase = undef;
my $exepath = undef;
my $exeindex = undef;
my $nodefile = $ENV{'GASNET_NODEFILE'} || $ENV{'PBS_NODEFILE'};
my @tmpfiles = (defined($nodefile) && $ENV{'GASNET_RM_NODEFILE'}) ? ("$nodefile") : ();
my $spawner = $ENV{'GASNET_IB_SPAWNER'};
my $conduit = $ENV{'GASNET_IB_CONDUIT'};

sub usage
{
    print (@_) if (@_);

    print "usage: gasnetrun -n <n> [options] [--] prog [program args]\n";
    print "    options:\n";
    print "      -n <n>                number of processes to run\n";
    print "      -N <N>                number of nodes to run on (not always supported)\n";
    print "      -E <VAR1[,VAR2...]>   list of environment vars to propagate\n";
    print "      -v                    be verbose about what is happening\n";
    print "      -t                    test only, don't execute anything (implies -v)\n";
    print "      -k                    keep any temporary files created (implies -v)\n";
    print "      -spawner=(ssh|mpi)    force use of MPI or SSH for spawning\n";
    print "      --                    ends option parsing\n";
    exit 1;
}

sub fullpath($)
{
    my $file = shift;
    my $result = undef;
    if ($file =~ m|^/|) {
	# full path, don't do anything to it
	$result = $file;
    } elsif ($file =~ m|/| || -x $file) {
	# has directory components or exists in cwd
	my $cwd = `pwd`;
	chomp $cwd;
	$result = "$cwd/$file";
    } else {
	# search PATH
	foreach (split(':', $ENV{PATH})) {
	    my $tmp = "$_/$file";
	    if (-x $tmp) {
		$result = $tmp;
		last;
	    }
	}
    }
    return $result
}

# We need to parse our command-line arguments
# We also build up @mpi_args, stripping out ones that are purely ours
   
    while (@ARGV > 0) {
	$_ = $ARGV[0];
	push @mpi_args, $_;

	if ($_ eq '--') {
	    shift;
	    last;
	} elsif ($_ eq '-n' || $_ eq '-np') {
	    shift;
	    push @mpi_args, $ARGV[0];
	    usage ("$_ option given without an argument\n") unless @ARGV >= 1;
	    $numproc = 0+$ARGV[0];
	    usage ("$_ option with invalid argument '$ARGV[0]'\n") unless $numproc >= 1;
	} elsif ($_ =~ /^(-np?)([0-9]+)$/) {
	    $numproc = 0+$2;
	    usage ("$1 option with invalid argument '$2'\n") unless $numproc >= 1;
	} elsif ($_ eq '-N') {
	    shift;
	    push @mpi_args, $ARGV[0];
	    usage ("$_ option given without an argument\n") unless @ARGV >= 1;
	    $numnode = 0+$ARGV[0];
	    usage ("$_ option with invalid argument '$ARGV[0]'\n") unless $numnode >= 1;
	} elsif ($_ =~ /^(-N)([0-9]+)$/) {
	    $numnode = 0+$2;
	    usage ("$1 option with invalid argument '$2'\n") unless $numnode >= 1;
	} elsif ($_ eq '-E') {
	    shift;
	    push @mpi_args, $ARGV[0];
	    usage ("-E option given without an argument\n") unless @ARGV >= 1;
	} elsif ($_ =~ /^-spawner=(.+)$/) {
	    $spawner = $1;
	    pop @mpi_args;	# not known to mpi spawner
	} elsif ($_ eq '-v') {
	    $verbose = 1;
	} elsif ($_ eq '-t') {
	    $dryrun = 1;
	    $verbose = 1;
	} elsif ($_ eq '-k') {
	    $keep = 1;
	    $verbose = 1;
	} elsif (m/^-/) {
	    usage ("unrecognized option '$_'\n");
	} else {
	    pop @mpi_args; # pop off program name
	    last;
	}
	shift;
    }
    $spawner = uc($spawner);

# Validate flags
    if (!defined($numproc)) {
        usage "Required option -n was not given\n";
    }
    if (!defined($spawner)) {
        usage "Option -spawner was not given and no default is set\n"
    }
    if (($spawner eq 'MPI') && !$ENV{GASNET_IB_BOOTSTRAP_MPI}) {
        usage "Spawner is set to MPI, but MPI support was not compiled in\n"
    }

# Find the program (possibly a wrapper)
    $exebase = $ARGV[0] or usage "No program specified\n";
    $exepath = fullpath($exebase);
    die "gasnetrun: unable to locate program '$exebase'\n"
			unless (defined($exepath) && -x $exepath);
    print "gasnetrun: located executable '$exepath'\n" if ($verbose);
    $ARGV[0] = $exepath;

# Find the GASNet executable and verify its capabilities
    my $pattern = "^GASNet" . $spawner . "Spawner: 1 \\\$";
    my $found = undef;
    $exeindex = 0;
    foreach my $arg (@ARGV) {
	++$exeindex;
	next if ($arg =~ m/^-/); # skip obvious options
	my $file = fullpath($arg);
	next unless (defined($file) && -x $file); # not found or not executable
        my $is_gasnet = undef;
	next unless open (FILE, $file);
	{   local $/ = '$'; # use $ as the line break symbol
            while (<FILE>) {
                next unless(/^GASNet/);
		if (/GASNetConduitName: $conduit $/) { $is_gasnet = 1; next; }
                if (/$pattern/o) { $found = 1; last; }
            }
        }
        close (FILE);
	if ($found) {
	    if ($exeindex > 1) { # wrapper in use
		$arg = $file;	# canonicalize (foreach is by reference)
		print "gasnetrun: located GASNet executable '$file'\n" if ($verbose);
	    }
	    last;
	} elsif ($is_gasnet) {
	    die "GASNet executable '$file' does not support spawner '$spawner'\n";
	}
    }
    warn "gasnetrun: unable to locate a GASNet program in '@ARGV'\n" unless ($found);

# Run it which ever way makes sense
    $ENV{"GASNET_VERBOSEENV"} = "1" if ($verbose);
    $ENV{'GASNET_IB_SPAWNER'} = $spawner;
    if ($spawner eq 'MPI') {
        print("gasnetrun: forwarding to mpi-based spawner\n") if ($verbose);
        @ARGV = (@mpi_args, @ARGV);
        (my $mpi = $0) =~ s/\.pl$/-mpi.pl/;
        die "cannot find $mpi: $!" unless -f $mpi;
        my $err = do $mpi; # use 'do' to load another perl file (reduce forks, etc)
        if ($@ || $err) {
          die "error running $mpi:\n $@ $err\n";
        }
    } elsif ($spawner eq 'SSH') {
	my $wrapper = ($exeindex > 1) ? join ' ',
					     map { s/'/'\\''/g; "'".$_."'"; }
						 splice @ARGV, 0, $exeindex-1
				      : undef;
	my @extra_args = grep { defined($_); } ('-GASNET-SPAWN-master',
						$verbose ? '-v' : undef,
						$wrapper ? ('-W'.$wrapper) : undef,
						"$numproc" . ($numnode ? ":$numnode" : ''),
						'--');
	my @cmd = @ARGV;
	splice @cmd, 1, 0, @extra_args;
	print("gasnetrun: running: ", join(' ', @cmd), "\n") if ($verbose);
	unless ($dryrun) { exec(@cmd) or die "failed to exec $exebase\n"; }
    } else {
        die "Unknown spawner '$spawner' requested\n";
    }

__END__
