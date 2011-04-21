#!/usr/bin/perl
#   $Source: /var/local/cvs/gasnet/gm-conduit/contrib/gasnetrun_gm.pl,v $
#     $Date: 2006/11/01 22:11:24 $
# $Revision: 1.25 $
#
# Included here as a contrib/ from the mpich 1.2.5..10 mpirun script,
# since this is the closest thing myricom ships to a spawner interface.
#
# use strict 'refs';
# use warnings;
use Socket;
use Sys::Hostname;
use Cwd;

use constant MAX_RECV_LEN => 65536;

srand;


###############
#             #
#  Variables  #
#             #
###############

$verbose = 0;
$pwd = &Cwd::cwd();
$delay_rexec = 0;
$np = 1;
$use_shmem = 1;
@extraopts = undef;
@envlist_cmdline = undef;
$ssh_exec = $ENV{"GASNET_SSH"} || "/usr/bin/ssh";
$extraopts{"ssh"} = "";
$rsh_exec = $ENV{"GASNET_RSH"} || "/usr/bin/rsh";
$extraopts{"rsh"} = "";
$rexec = "$ssh_exec";
$rexec_type = "ssh";  
$saw_type_option = 0;
$rexec_reaper = 1;
# May have to change the arch
$arch = "LINUX";
$varenv = '';
$dry_run = 0;
$kill_time = 0;
$recv_mode = 'polling';
$exit_code = 0;
my $envprog = $ENV{'ENVCMD'} || "/usr/bin/env";
$envprog = "env" if (! -x $envprog);

# GEXEC configuration, preconfigured for millennium.
$gm_board_info = `which gm_board_info 2> /dev/null` || "/usr/mill/pkg/gm/bin/gm_board_info";
$gstat         = $ENV{"GASNET_GSTAT_CMD"} || "/usr/bin/gstat -l1";
$gexec         = $ENV{"GASNET_GEXEC_CMD"} || "/usr/bin/gexec -p none";
$extraopts{"gexec"} = "";
$black_listed_hosts = $ENV{"GASNET_GEXEC_BLACKLIST"} || "lime|lemon";
$domainname    = $ENV{"GASNET_GEXEC_DOMAINNAME"} || ".Millennium.Berkeley.EDU";

$totalview = 0;
$totalview_cmd = $ENV{'TOTALVIEW'} || 'totalview';
$ddt = 0;
$ddt_cmd = $ENV{'DDT'} || 'ddt';
$display = $ENV{'DISPLAY'};
$timeout = 0;
$eager = 0;
$close_stdin = 0;
$cleanup_shmem = 0;
$pid_socket = 1;
$pid_rexec = 1;
$default_machinefile = "$ENV{'GASNET_NODEFILE'}" || "$ENV{'PBS_NODEFILE'}";
$magic = int (rand (9999999));
$local_host = hostname;
$local_ip   = inet_ntoa(scalar gethostbyname($local_host || 'localhost'));
$local_port = '8000';
$runcmd = "";


###################
#                 #
#  Sub functions  #
#                 #
###################

sub clean_up {
  print "clean_up\n" if $verbose;
  if ($rexec_type eq "gexec") { 
    while (wait != -1) {
     print "waiting for child processes...\n";
     sleep 1;
    }
    return; 
  }

  # reap remote processes, usefull because ssh is broken and does not
  # clean up remote processes when killed.
  if (($pid_socket == 0) && ($rexec_reaper)) {
    print ("Reap remote processes:\n") if $verbose;
    for ($z=0; $z<$np; $z++) {
      if (defined ($remote_pids[$z])) {
	$pid_reaper = fork;
	if ($pid_reaper == 0) {
	  if ($verbose) {
	    print ("\t ".@rexec_flags." $hosts[$z] -n kill -9 $remote_pids[$z] 2>/dev/null\n");
	  }
	  exec (@rexec_flags, $hosts[$z], '-n', "kill -9 $remote_pids[$z]", "2>/dev/null");
	}
      }
    }

    while (wait != -1) {
      ;
    }
  }

  if (($pid_rexec != 0) && ($pid_socket != 0)) {
    if (defined ($pids[0])) {
      foreach $p (@pids) {
	if (kill 0 => $p) {
	  kill 'KILL', $p;
	}
      }
    }

    if ($pid_socket > 1) {
      if (kill 0 => $pid_socket) {
	kill 'TERM', $pid_socket;
      }
    }

    if (($cleanup_shmem) && ($use_shmem > 0) && defined ($shmem_file)) {
      print "Cleaning up shared memory files on remote nodes.\n" if $verbose;
      for ($i=0; $i<$np; $i++) {
	if (defined ($hosts[$i])) {
	  $unique_hosts{$hosts[$i]}++;
	}
      }
      foreach $host_cleanup (keys (%unique_hosts)) {
	$pid_cleanup = fork;
	if ($pid_cleanup == 0) {
	  if ($verbose) {
	    print ("@rexec_flags $host_cleanup -n rm -f $shmem_file\n");
	  }
	  exec(@rexec_flags, $host_cleanup, '-n', "rm -f $shmem_file");
	}
      }

      while (wait != -1) {
	;
      }
    }
  }

  while (wait != -1) {
    ;
  }

  unlink $ENV{"GASNET_NODEFILE"} if ($ENV{"GASNET_RM_NODEFILE"});
}


sub cleanup_SIGINT {
  if (($pid_rexec != 0) && ($pid_socket != 0)) {
    print ("Received SIGINT. Cleaning up...\n") if $verbose;
  }
  clean_up;
  exit (1);
}

sub cleanup_SIGTERM {
  if (($pid_rexec != 0) && ($pid_socket != 0)) {
    print ("Received SIGTERM. Cleaning up...\n") if $verbose;
  }
  clean_up;
  exit (1);
}

sub cleanup_SIGKILL {
  if (($pid_rexec != 0) && ($pid_socket != 0)) {
    print ("Received SIGKILL. Cleaning up...\n") if $verbose;
  }
  clean_up;
  exit (1);
}

sub cleanup_SIGQUIT {
  if (($pid_rexec != 0) && ($pid_socket != 0)) {
    print ("Received SIGQUIT. Cleaning up...\n") if $verbose;
  }
  clean_up;
  exit (1);
}

sub cleanup_ALARM {
  print ("Received SIGALRM. Cleaning up...\n") if $verbose;
  clean_up;
  exit (1);
}

sub cleanup_TIMEOUT {
  print ("Timeout: still waiting for data from remote GASNet processes !\n");
  print ("Timeout: cleaning up...\n");
  clean_up
  exit (1);
}


#cannonize program
sub find_program {
  my ($prog) = @_;

  if ($prog =~ m|^/|) {
  } elsif ($prog =~ m|/|) {
    $prog = $pwd."/".$prog;
  } else {
    if (-x $prog) {
      $prog = $pwd."/".$prog;
    } else {
      foreach (split (/:/, $ENV{PATH})) {
	if (-x "$_/$prog") {
	  $prog = "$_/$prog";
	  last;
	}
      }
    }
  }

  -e $prog or die "$prog not found !\n";
  -x $prog or die "$prog is not executable !\n";

  print "Program binary is: $prog\n" if $verbose;
  return $prog;
}

sub usage {
  if ($_[0] ne '') {
    print (STDERR "Error in gasnetrun_gm: @_\n\n");
  }

  print (STDERR "Usage:\n \t gasnetrun_gm [options] [-np <n>] prog [flags]\n");
  print (STDERR "   -E   <VAR1[,VAR2...]>   list of environment vars to propagate\n");
  print (STDERR "   -v   Verbose - provide additional details of the script's execution.\n");
  print (STDERR "   -t   Testing - do not actually run, just print what would be executed.\n");
  print (STDERR "   -s   Close stdin - can run in background without tty input problems.\n");
  print (STDERR "   -r   Cleanup remote shared memory files - should be removed automatically,\n");
  print (STDERR "        but always good to have an option to force it.\n");
  print (STDERR "   -machinefile <file>   Specifies a machine file, There is no default\n");
  print (STDERR "   --gexec         The spawner is gexec.\n");
  print (STDERR "   --ssh           The spawner is ssh.\n");
  print (STDERR "   --rsh           The spawner is rsh.\n");
  print (STDERR "   --gexec-options <opt> Additional options to pass gexec.\n");
  print (STDERR "   --ssh-options <opt>   Additional options to pass ssh.\n");
  print (STDERR "   --rsh-options <opt>   Additional options to pass rsh.\n");
  print (STDERR "   --gm-no-shmem   Disable the shared memory support (enabled by default).\n");
  print (STDERR "   --gm-numa-shmem Enable shared memory only for processes sharing the same Myrinet interface.\n");
  print (STDERR "   --gm-wait <n>   Wait <n> seconds between each spawning step.\n");
  print (STDERR "   --gm-kill <n>   Kill all processes <n> seconds after the first exits.\n");
  print (STDERR "   --gm-eager <n>  Specifies the Eager/Rendez-vous protocol threshold size.\n");
  print (STDERR "   --gm-recv <m>   Specifies the receive mode <polling>, <blocking> or\n");
  print (STDERR "                   <hybrid>, <polling> is the default.\n");
  print (STDERR "   -totalview      Specifies Totalview debugging session.\n");
  print (STDERR "   -ddt            Specifies DDT debugging session.\n");
  print (STDERR "   -pg <file>      Specifies the procgroup file.\n");
  print (STDERR "   -wd <path>      Specifies the working directory.\n");
  print (STDERR "   -np <n>         Specifies the number of processes.\n");
  print (STDERR "   prog [flags]    Specifies which command line to run.\n");
  exit (1);
}

#set the current dir
if (defined ($ENV {'PWD'})) {
  my @P = stat($ENV {'PWD'}."/.");
  my @p = stat(".");
  if ($p[0] == $P[0] && $p[1] == $P[1]) {
    $pwd = $ENV{'PWD'};
  }
}

#####################
#                   #
#   Args parsing    #
#                   #
#####################


while (@ARGV > 0) {
  $_ = $ARGV[0];

  if ($_ eq '-v') {
    $verbose = 1;
  } elsif ($_ eq '-t') {
    $dry_run = 1;
  } elsif ($_ eq '-s') {
    $close_stdin = 1;
  } elsif ($_ eq '-r') {
    $cleanup_shmem = 1;
  } elsif ($_ eq '-machinefile') {
    shift;
    usage ("No machine file specified (-machinefile) !") unless @ARGV >= 1;
    $machine_file = $ARGV[0];
  } elsif ($_ eq '--gexec') {
    $saw_type_option = 1;
    $rexec_type = "gexec";
    $rexec = $gexec;
  } elsif ($_ eq '--ssh') {
    $saw_type_option = 1;
    $rexec_type = "ssh";
    $rexec = $ssh_exec;
  } elsif ($_ eq '--rsh') {
    $saw_type_option = 1;
    $rexec_type = "rsh";
    $rexec = $rsh_exec;
  } elsif ($_ eq '--gexec-options') {
    shift;
    usage ("no options specified for --gexec-options !") unless @ARGV >= 1;
    $extraopts{"gexec"} .= $ARGV[0] . " ";
  } elsif ($_ eq '--ssh-options') {
    shift;
    usage ("no options specified for --ssh-options !") unless @ARGV >= 1;
    $extraopts{"ssh"} .= $ARGV[0] . " ";
  } elsif ($_ eq '--rsh-options') {
    shift;
    usage ("no options specified for --rsh-options !") unless @ARGV >= 1;
    $extraopts{"rsh"} .= $ARGV[0] . " ";
  } elsif ($_ eq '--gm-no-shmem') {
    $use_shmem = 0;
  } elsif ($_ eq '--gm-numa-shmem') {
    $use_shmem = 2;
  } elsif ($_ eq '--gm-wait') {
    shift;
    usage ("No waiting time specified (--gm-wait) !") unless @ARGV >= 1;
    $delay_rexec = $ARGV[0];
  } elsif ($_ eq '--gm-kill') {
    shift;
    if ((@ARGV == 0) && ($ARGV[0] !~ /^\d+$/)) {
      usage ("No termination delay specified (--gm-kill) !");
    }
    $kill_time = $ARGV[0];
  } elsif ($_ eq '--gm-eager') {
    shift;
    if ((@ARGV == 0) && ($ARGV[0] !~ /^\d+$/)) {
      usage ("No Eager/Rendez-vous protocol threshold message size (--gm-eager) !");
    }
    $eager = $ARGV[0];
  } elsif ($_ eq '--gm-recv') {
    shift;
    usage ("No receive mode specified (--gm-recv) !") unless @ARGV >= 1;
    if ($ARGV[0] !~ /^(polling|blocking|hybrid)$/) {
      usage ("Bad receive mode specified (--gm-recv) !");
    }
    $recv_mode = $ARGV[0];
  } elsif (($_ eq '-totalview') || ($_ eq '-tv')) {
    $totalview = 1;
  } elsif ($_ eq '-ddt') {
    $ddt = 1;
  } elsif ($_ eq '-pg') {
    shift;
    usage ("No procgroup file specified (-pg) !") unless @ARGV >= 1;
    if ($np > 1) {
      usage ("-np and -pg are exclusive !");
    }
    $procgroup_file = $ARGV[0];
  } elsif ($_ eq '-wd') {
    shift;
    usage ("No working directory specified (-wd) !") unless @ARGV >= 1;
    $wdir = $ARGV[0];
  } elsif ($_ eq '-np') {
    shift;
    if ((@ARGV == 0) || !($ARGV[0] =~ /^(\s*)(\d+)$/)) {
      usage ("Bad number of processes (-np) !");
    }
    if (defined ($procgroup_file) && ($np > 1)) {
      usage ("-np and -pg are exclusive !");
    }
    $np = $ARGV[0];
  } elsif ($_ eq '-E') {
    shift;
    usage ("-E option given without an argument\n") unless @ARGV >= 1;
    foreach (split(',', $ARGV[0])) {
      $envlist_cmdline{$_} = 1;
    }
  } elsif (($_ eq '-help') || ($_ eq '--help') || ($_ eq '-h')) {
    usage ('');
  } elsif ($_ eq '-mvback' ) {
  } elsif ($_ eq '-mvhome' ) {
  } elsif (/=/) {
    $varenv .= " $ARGV[0]";
  } elsif (/^-/) {
    usage ("Unknown option ($_) !");
  } else {
    $app_cmd = find_program ($ARGV[0]);
    @app_flags = (map { "'$_'" } @ARGV[1..$#ARGV]);
    last;
  }
  shift;
}

$ENV{"GASNET_VERBOSEENV"} = "1" if ($verbose);
$ENV{"GASNET_GASNETRUN_GM"} = "1";

# Before going on, check if we should force using GEXEC, if 
# GASNET_GEXEC_CMD is set.
if (defined $ENV{"GASNET_GEXEC_CMD"} && !$saw_type_option) {
    printf "Using gexec command $ENV{GASNET_GEXEC_CMD}\n" if $verbose;
    $rexec = $gexec;
    $rexec_type = "gexec";
}

@rexec_flags = split(/ /, $rexec . " " . $extraopts{${rexec_type}});

$app_cmd or usage (" Missing program name !");


# Change the default machine file to $ENV{"MACHINE FILE"} if exists
if (defined ($ENV{"MACHINE_FILE"})) {
  $default_machinefile = $ENV{"MACHINE_FILE"};
}

# If the machine file is not defined, use the system-wide one.
$machine_file = $default_machinefile unless defined ($machine_file);

if (!$machine_file && ($rexec_type ne "gexec")) {
    printf "Can't detect a PBS or a GEXEC environment.  If you are not running\n"
         . "within a batch system, set the GASNET_NODEFILE environment variable to\n"
         . "a file containing one hostname per line (the first process will use \n"
         . "the host on the first line, etc.)\n";
    exit 1;
}

# If the machine file is not an absolute path, add the current directory.
$machine_file = $pwd."/".$machine_file if !($machine_file =~ m|^/|);


# Print the settings if verbose.
if ($verbose) {
  print ("Dry-run mode enabled (Testing).\n") if $dry_run;
  print ("Machines file is $machine_file\n");
  print ("Shared memory for intra-nodes coms is enabled.\n") if ($use_shmem == 1);
  print ("Shared memory for NUMA intra-nodes coms is enabled.\n") if ($use_shmem == 2);
  print ("Shared memory for intra-nodes coms is disabled.\n") if !$use_shmem;
  print ("Delay of $delay_rexec between spanwing steps.\n") if $delay_rexec;
  print ("Processes will be killed $kill_time after first exits.\n") if $kill_time;
  print ("GM receive mode used: $recv_mode.\n");
  print ("Eager/Rendez-vous threshold: $eager Bytes.\n") if $eager;
  print ("Use Totalview for debugging session.\n") if $totalview;
  print ("Use DDT for debugging session.\n") if $ddt;
  print ("Set working directory to $wdir.\n") if (defined ($wdir));
  print ("$np processes will be spawned: \n") if (!defined ($procgroup_file));
}


if (!defined ($wdir)) {
  $wdir = $pwd;
}

if (defined ($procgroup_file)) {
  # Open the procgroup file, read it and close it.
  open (PROCGROUP_FILE, "$procgroup_file")
    or die "Cannot open the procgroup file $procgroup_file: $!\n";
  @procgroup_file_data = <PROCGROUP_FILE>;
  close(PROCGROUP_FILE);

  # Extract the informations from the procgroup file.
  $np = 0;
  $line_number = 0;
  while (scalar (@procgroup_file_data)) {
    $line = shift (@procgroup_file_data);
    $line_number++;
    next if ($line =~ /^\s*$/);
    next if ($line =~ /^\#/);
    chomp ($line);

    if ($line =~ /^\S+\s+\d+\s*\S*\s*\S*/) {
      @fields = split (/\s+/, $line);
    } else {
      die "Bad line in $procgroup_file (line $line_number): \"$line\"";
    }

    if (scalar (@fields) < 2) {
      die "Bad line at $machine_file:$line_number): \"$line\"";
    }

    # Extract the hostname, the index and the executable (and maybe the login)
    $i = $fields[1];
    if ($np == 0) {
      $i++;
      if ($fields[0] eq "local") {
        $fields[0] = $local_host;
      }
      if (!defined ($fields[2])) {
        $fields[2] = $app_cmd;
      }
    }

    # sanity checks
    if (!defined ($fields[2])) {
      die "Missing progname in $procgroup_file (line $line_number): \"$line\"";
    }

    for ($j=0; $j<$i; $j++) {
      $hosts[$np] = $fields[0];
      $boards[$np] = -1;
      $apps_cmd[$np] = $fields[2];
      if (defined ($fields[3])) {
        $logins[$np] = $fields[3];
      }
      $apps_flags[$np] = ' ';
      if (defined ($fields[4])) {
	for ($k=4; $k<scalar (@fields); $k++) {
	  $apps_flags[$np] .= $fields[$k].' ';
	}
      }
      $np++;
    }
  }
} elsif ($rexec_type ne "gexec") {
  # Open the machines file, read it and close it.
  open (MACHINE_FILE, "$machine_file")
    or die "Cannot open the machines file $machine_file: $!\n";
  @machine_file_data = <MACHINE_FILE>;
  close(MACHINE_FILE);

  # Extract the informations from the machines file.
  $i = 0;
  $line_number = 0;
  while ($i<$np) {
    $line = shift (@machine_file_data);
    push (@machine_file_data, $line);
    $line_number++;
    next if ($line =~ /^\s*$/);
    next if ($line =~ /^\#/);
    chomp ($line);

    if ($line =~ /^\S+\s*\d*\s*$/) {
      @fields = split (/\s+/, $line);
    } else {
      die "Bad line in $machine_file (line $line_number): \"$line\"";
    }

    # Extract the board number if present.
    if (scalar (@fields) > 1) {
      if ($fields[1] =~ /^\d$/) {
	$board_id = $fields[1];
      } else {
	die "Bad board number at $machine_file:$line_number): \"$line\"";
      }
    } else {
      $board_id = -1;
    }

    # Extract the host name and eventually the number of processors.
    if ($fields[0] =~ /^(\S+):(\d+)$/) {
      if ($2 < 1) {
	die "Bad counts in $machine_file (line $line_number): \"$line\"";
      }

      for ($j=0; $j<$2; $j++) {
	$hosts[$i] = $1;
	$boards[$i] = $board_id;
	
	$apps_cmd[$i] = $app_cmd;
	$apps_flags[$i] = '';
	for ($k=0; $k<scalar (@app_flags); $k++) {
	  $apps_flags[$i] .= $app_flags[$k].' ';
	}
	$i++;
      }
    } else {
      $hosts[$i] = $fields[0];
      $boards[$i] = $board_id;

      $apps_cmd[$i] = $app_cmd;
      $apps_flags[$i] = '';
      for ($k=0; $k<scalar (@app_flags); $k++) {
	$apps_flags[$i] .= $app_flags[$k].' ';
      }
      $i++;
    }
  }
}

# Print the configuration.
if ($verbose) {
  for ($i=0; $i<$np; $i++) {
    if ($boards[$i] >= 0) {
      print ("\tProcess $i ($apps_cmd[$i] $apps_flags[$i]) on $hosts[$i] and board $boards[$i]\n");
    } else {
      print ("\tProcess $i ($apps_cmd[$i] $apps_flags[$i]) on $hosts[$i]\n");
    }
  }
}


# Open the first socket with the first available port.
if (!$dry_run) {
  print ("Open a socket on $local_host ($local_ip)...\n") if $verbose;
  socket (FIRST_SOCKET, AF_INET, SOCK_STREAM, getprotobyname ('tcp'))
    or die ("First socket creation failed: $!\n");
  setsockopt (FIRST_SOCKET, SOL_SOCKET, SO_REUSEADDR, 1)
    or warn ("Error setting first socket option: $!\n");

  while (!(bind (FIRST_SOCKET, sockaddr_in ($local_port, INADDR_ANY)))
	 && ($local_port < 20000)) {
    $local_port += 1;
  }
  if ($local_port < 20000) {
    print ("Got a first socket opened on port $local_port.\n") if $verbose;
    $varenv .= " GMPI_MASTER=$local_ip GMPI_PORT=$local_port";
    listen (FIRST_SOCKET, SOMAXCONN)
      or die ("Error when listening on first socket: $!\n");
  } else {
    die ("Unable to open a socket on $local_host !\n");
  }
}


# Initialize the shared memory flag.
if ($use_shmem > 0) {
  if (!$shmem_file) {
    $shmem_file = "/tmp/gmpi_shmem-$magic:[0-9]*.tmp";
  }

  print "Shared memory file: $shmem_file\n\n" if $verbose;
}
$varenv .= " GMPI_SHMEM=$use_shmem";


# Initialize the receive mode flag.
if ($recv_mode ne 'polling') {
  $varenv .= " GMPI_RECV=$recv_mode";
}


# Initialize the Eager size flag.
if ($eager) {
  $varenv .= " GMPI_EAGER=$eager";
}

$SIG{'INT'} = 'cleanup_SIGINT';
$SIG{'TERM'} = 'cleanup_SIGTERM';
$SIG{'KILL'} = 'cleanup_SIGKILL';
$SIG{'QUIT'} = 'cleanup_SIGQUIT';
  
if ($rexec_type eq "gexec") {
  my %gm_hosts;
  my %gstat_hosts;
  my $gstat_output = "";
  my %gstat_hosts_ignore;
  my @gstat_hosts;
  my $gm_hosts_found = 0;
  my $gm_disconnected_spawner = 0;

  # Either keep user-supplied server list.
  $ENV{'GMPI_MAGIC'}  = $magic;
  $ENV{'GMPI_MASTER'} = $local_ip;
  $ENV{'GMPI_PORT'}   = $local_port;
  $ENV{'GMPI_BOARD'}  = -1; # No multiboard support

  if (!defined($ENV{'GEXEC_SVRS'})) {
    open BOARD_INFO, "$gm_board_info |" or die "Can't run $gm_board_info: $!";
    while (<BOARD_INFO>)
    {
      $gm_disconnected_spawner++ if /No boards found/;
      last if(/^---/);
    }
    if ($gm_disconnected_spawner) {
    	printf("WARNING: Spawning from a node disconnected from Myrinet\n") if $verbose;
    }
    else {
        while (<BOARD_INFO>)
        {
          (my $foo, my $gmID, my $MAC, my $gmName, my $Route) = split /\s+/, $_; 
          print "No GM routes found\n" and exit if($gmID eq "***");
          next if( $gmName =~/$black_listed_hosts/ );
          $gm_hosts{$gmName} = $gmID;
          $gm_hosts{$gmName.$domainname} = $gmID;
          $gm_hosts_found++;
        }
        close (BOARD_INFO);
    }

    # Don't die if no hosts found, as no hosts may be found when run from a
    # spawning node disconnected from the Myrinet network
    #die("Can't find any GM hosts on cluster (is the mapper up?)\n") 
    #    if (!$gm_hosts_found);

    #die("GM cluster only has $gm_hosts_found live nodes and $np requested\n")
    #	if ($gm_hosts_found < $np);
      
  
    open GSTAT_INFO, "$gstat $gstat_opts |" or 
    	die "Can't run $gstat $gstat_opts: $!";
    while (<GSTAT_INFO>)
    {
      /^([\w.]+)\s+(\d+)\D+(\d+)\D+(\d+)\D+([\d.]+)/;
      # $1 = name $2 = cpus $3 = running procs $4 total procs, $5 1-min load
      my $available_cpus = int($2);
      if ((not exists $gm_hosts{$1}) && !$gm_disconnected_spawner) {
      	print "Host $1 not found in gstat\n" if $verbose;
	next;
      }
      $gstat_hosts{$1} = $available_cpus;
    }
    close (GSTAT_INFO);
  
    foreach my $host 
       (sort {$gstat_hosts{$b} <=> $gstat_hosts{$a}} (keys(%gstat_hosts)) )
    {
      $host =~ m/([^\.]+)/ || next;
      $gstat_output .= $1 . " ";
    }

    $ENV{'GEXEC_SVRS'} = $gstat_output;
  }

  undef @hosts;
  @gstat_hosts = split(/\s+/, $ENV{'GEXEC_SVRS'});

  # Remove any hosts that were in ignore.
  if (defined($ENV{'GEXEC_SVRS_IGNORE'})) {
    foreach my $host (split(/\s+/, $ENV{'GEXEC_SVRS_IGNORE'})) {
      $gstat_hosts_ignore{$host} = 1;
      $gstat_hosts_ignore{$host.$domainname} = 1;
    }

    foreach my $host (@gstat_hosts) {
      push (@hosts, $host) unless $gstat_hosts_ignore{$host};
    }
  } else {
    push(@hosts, @gstat_hosts);
  }

  if ($#hosts+1 < $np) {
    printf("gexec can only find %d from the $np requested.: @hosts\n", 
        $#hosts+1);
    exit(0);
  }

  splice(@hosts, $np);
  $ENV{'GEXEC_SVRS'} = join(" ", @hosts);
}

if (!$dry_run) {
  $pid_socket = fork;
  if ($pid_socket == 0) {
    # Gather the information from all remote processes via sockets.
    $SIG{'ALRM'} = 'cleanup_TIMEOUT';
    alarm ($timeout);

    $index = $np;
    while ($index > 0) {
      accept (INCOMING_SOCKET, FIRST_SOCKET);
      recv (INCOMING_SOCKET, $incoming_data, MAX_RECV_LEN, 0);

      if ($incoming_data !~ /^<<<(\d+):(\d+):(\d+):(\d+):(\d+):(\d+):(\d+)::(\d+)>>>$/) {
        warn ("Received invalid data format !\n");
        close (INCOMING_SOCKET);
        next;
      }

      # Check the magic number.
      if ($1 != $magic) {
	warn ("Received bad magic number !\n");
	close (INCOMING_SOCKET);
	next;
      }

      if ($2 > $np) {
	clean_up;
	die "MPI Id received is out of range ($2 over $np)\n";
      }

      if ($3 == 0) {
	clean_up;
	die "MPI Id $2 was unable to open a GM port.)\n";
      }
	
      if (defined ($port_ids[$2])) {
	warn ("Ignoring message from the MPI Id $2 ($_) !\n");
	close (INCOMING_SOCKET);
	next;
      }

      $port_ids[$2] = $3;
      $board_ids[$2] = $4;
      $node_ids[$2] = $5;
      $numanodes[$2] = $6;
      $remote_pids[$2] = $7;
      $remote_ports[$2] = $8;
      $index--;
      close (INCOMING_SOCKET);

      if ($verbose) {
	print ("MPI Id $2 is using GM port $3, board $4, GM_id $5.\n");
      }
    }

    print ("Received data from all $np MPI processes.\n") if $verbose;

	
    # Build the Port ID/Board ID mapping.
    $global_mapping = '[[[';
    for ($i=0; $i<$np; $i++) {
      $global_mapping .= '<'.$port_ids[$i].':'.$board_ids[$i].':'.$node_ids[$i].':'.$numanodes[$i].'>';
    }
    $global_mapping .= '|||';


    # Send the Port ID/Board ID mapping to all remote processes.
    $index = 0;
    while ($index < $np) {
      unless ($remote_addr = inet_aton($hosts[$index])) {
	clean_up;
	die "Cannot get address of host: $hosts[$index]";
      }
      $net_addr = sockaddr_in($remote_ports[$index], $remote_addr);

      unless (socket (SECOND_SOCKET, AF_INET, SOCK_STREAM, 
		      getprotobyname ('tcp'))) {
	clean_up;
	die ("Second socket creation failed: $!\n");
      }
      setsockopt (SECOND_SOCKET, SOL_SOCKET, SO_REUSEADDR, 1)
	or warn ("Error setting second socket option: $!\n");
      unless (connect(SECOND_SOCKET, $net_addr)) {
	clean_up;
	die "Cannot connect to $hosts[$index] on port $remote_ports[$index]: $!";
      }

      print ("Sending mapping to MPI Id $index.\n") if $verbose;

      $local_mapping = '';
      for ($i=0; $i<$np; $i++) {
	if (($hosts[$index] eq $hosts[$i]) 
	    && ($numanodes[$index] eq $numanodes[$i])) {
	  $local_mapping .= '<'.$i.'>';
	}
      }
      $local_mapping .= ']]]';

      send (SECOND_SOCKET, "$global_mapping$local_mapping", 0);
      close (SECOND_SOCKET);

      $port_ids[$index] = 0;
      $index++;
    }
    alarm (0);
    print ("Data sent to all processes.\n") if $verbose;

    if ($rexec_type eq "gexec") { exit(0); }

    # Keep the first socket opened for abort messages.
    while (1) {
      accept (ABORT_SOCKET, FIRST_SOCKET);
      recv (ABORT_SOCKET, $incoming_data, MAX_RECV_LEN, 0);

      if ($incoming_data !~ /^<<<ABORT_(\d+)_ABORT>>>$/) {
        print ("Received spurious abort message, keep listening...\n");
        close (ABORT_SOCKET);
        next;
      }

      if ($1 != $magic) {
	print ("Received bad magic number in abort message!\n");
	close (ABORT_SOCKET);
	next;
      }

      close (ABORT_SOCKET);
      close (FIRST_SOCKET);
      print ("Received valid abort message !\n") if $verbose;
      clean_up;
      exit (0);
    }
  }
}


if ($rexec_type eq "gexec") {

  $runcmd = "$np " . $app_cmd . " ";
  for ($k=0; $k<scalar(@app_flags);$k++) {
    $runcmd .= $app_flags[$k] . ' ';
  }

  if ($totalview) {
  	printf("$0 doesn't support gexec and totalview");
	exit(1);
  }
  elsif ($ddt) {
  	printf("$0 doesn't support gexec and ddt");
	exit(1);
  }

  if ($dry_run) {
    print ("@rexec_flags -n $runcmd\n");
  }
  else {
    print ("GEXEC_SVRS set to $ENV{'GEXEC_SVRS'}\n") if $verbose;
    print ("@rexec_flags -n $runcmd\n") if $verbose;
    exec ("@rexec_flags -n $runcmd\n");
  }
  exit(0);
}

# Spawn remote processes.
for ($i=0; $i<$np; $i++) {
  $pid_rexec = fork;
  if ($pid_rexec == 0) {
    $SIG{'INT'} = 'DEFAULT';
    $SIG{'TERM'} = 'DEFAULT';
    $SIG{'KILL'} = 'DEFAULT';
    $SIG{'QUIT'} = 'DEFAULT';

    if (defined($ENV{LD_LIBRARY_PATH})) {
      $varenv .= " LD_LIBRARY_PATH=$ENV{LD_LIBRARY_PATH}";
    }
    $varenv .= " DISPLAY=$display" if $display;
    $varenv .= " GMPI_MAGIC=$magic";
    $varenv .= " GMPI_ID=$i";
    $varenv .= " GMPI_NP=$np";
    $varenv .= " GMPI_BOARD=$boards[$i]";

    @envlist = undef;
    $envv = '';
    foreach $e (keys %ENV) {
	if (($e =~ m/(TI_)|(UPC_)|(GASNET_)/) || defined $envlist_cmdline{$e} ) {
		@sp = split(/\s+/, $ENV{$e});
		if ($#sp > 0 && $ENV{$e} !~ m/^\".*\"$/ && 
		                $ENV{$e} !~ m/^'.*'$/) {
		    $envv = "$e=\"$ENV{$e}\"";
		}
		else {
		    $envv = "$e=$ENV{$e}";
		}
		push(@envlist, $envv);
	}
    }

    $slave_ip = inet_ntoa(inet_aton("$hosts[$i]"));
    $varenv .= " GMPI_SLAVE=$slave_ip";
    $varenv .= " " . join (" ", @envlist);

    if (defined ($logins[$i])) {
      $login = 1;
      $login_id = $logins[$i];
    } else {
      $login = 0;
    }

    if ($totalview) {
      if ($i == 0) {
	$cmdline = "cd $wdir ; $envprog $varenv $totalview_cmd $apps_cmd[$i] -a $apps_flags[$i] -mpichtv";
      } else {
	$cmdline = "cd $wdir ; $envprog $varenv $apps_cmd[$i] $apps_flags[$i] -mpichtv";
      }
    } elsif ($ddt) {
      if ($i == 0) {
	$cmdline = "cd $wdir ; $envprog $varenv $ddt_cmd $apps_cmd[$i] $apps_flags[$i] -mpichtv";
      } else {
	$cmdline = "cd $wdir ; $envprog $varenv $apps_cmd[$i] $apps_flags[$i] -mpichtv";
      }
    } else {
      $cmdline = "cd $wdir ; $envprog $varenv $apps_cmd[$i] $apps_flags[$i]";
    }

    if ($dry_run) {
      if (($i == 0) && (!$close_stdin)) {
	if ($login) {
	  print ("@rexec_flags $hosts[$i] -l $login_id $cmdline\n");
	} else {
	  print ("@rexec_flags $hosts[$i] $cmdline\n");
	}
      } else {
	if ($login) {
	  print ("@rexec_flags $hosts[$i] -n -l $login_id $cmdline\n");
	} else {
	  print ("@rexec_flags $hosts[$i] -n $cmdline\n");
	}
      }
      exit (0);
    } else {
      if (($i == 0) && (!$close_stdin)) {
	if ($login) {
	  print ("@rexec_flags $hosts[$i] -l $login_id $cmdline\n") if ($verbose);
	  exec (@rexec_flags, $hosts[$i], '-l', $login_id, $cmdline);
	} else {
	  print ("@rexec_flags $hosts[$i] $cmdline\n") if ($verbose);
	  exec (@rexec_flags, $hosts[$i], $cmdline);
	}
      } else {
	if ($login) {
	  print ("@rexec_flags $hosts[$i] -n -l $login_id $cmdline\n") if ($verbose);
	  exec (@rexec_flags, $hosts[$i], '-n', '-l', $login_id, $cmdline);
	} else {
	  print ("@rexec_flags $hosts[$i] -n $cmdline\n") if ($verbose);
	  exec (@rexec_flags, $hosts[$i], '-n', $cmdline);
	}
      }
    }
    exit (0);
  } else {
    $pids[$i] = $pid_rexec;
    sleep ($delay_rexec) if ($delay_rexec);
  }
}

# If dry_run, there is nothing more to do.
if ($dry_run) {
  while (wait != -1) {
    ;
  }
  exit (0);
}
$index = $np;

# Wait and eventually kill remaining processes;
if ($kill_time) {
  $first_pid = wait;
  $index--;
  if ($first_pid == -1) {
    clean_up;
    exit $exit_code;
  }

  if ($first_pid == $pid_socket) {
    clean_up;
    exit $exit_code;
  }

  $exit_code = ($? << 8);

  if ($verbose) {
    for ($i=0; $i<$np; $i++) {
      if ($first_pid == $pids[$i]) {
	print ("GASNet Process $i has exited, wait $kill_time seconds and kill all remaining processes...\n") if $verbose;
	last;
      }
    }
  }

  $SIG{'ALRM'} = 'cleanup_ALARM';
  alarm ($kill_time + 1);
}

while (1) {
  $next_pid = wait;
  if ($next_pid == -1) {
    print ("All processes have exited.\n") if $verbose;
    clean_up;
    exit $exit_code;
  }

  if ($next_pid != $pid_socket) {
    print ("Remote GASNet exited with status " . ($? >> 8) . ".\n") if $verbose;
    $exit_code = ($? >> 8) unless ($exit_code != 0);	# Save first non-zero exit
    $index--;
    if ($index == 0) {
      print ("All remote GASNet processes have exited.\n") if $verbose;
      clean_up;
      exit $exit_code;
    }
  } else {
    # the process waiting for an Abort has exited, so let's aborting
    print ("Abort in progress...\n") if $verbose;
    clean_up;
    exit $exit_code;
  }
}
exit $exit_code;
