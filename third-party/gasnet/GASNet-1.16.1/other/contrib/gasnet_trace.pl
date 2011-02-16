#! /usr/bin/env perl

#############################################################
#   $Source: /var/local/cvs/gasnet/other/contrib/gasnet_trace.pl,v $
#     $Date: 2008/10/11 22:41:47 $
# $Revision: 1.37 $
#
# All files in this directory (except where otherwise noted) are subject to the
#following licensing terms:
#
#---------------------------------------------------------------------------
#Copyright (c) 2003, The Regents of the University of California, through
#Lawrence Berkeley National Laboratory (subject to receipt of any required
#approvals from U.S. Dept. of Energy)
#
#All rights reserved.
#
#Redistribution and use in source and binary forms with its documentation, with
#or without modification, are permitted for any purpose, without fee, provided
#that the following conditions are met:
#
#(1) Redistributions of source code must retain the above copyright notice, this
#list of conditions and the following disclaimer.
#(2) Redistributions in binary form must reproduce the above copyright notice,
#this list of conditions and the following disclaimer in the documentation and/or
#other materials provided with the distribution.
#(3) Neither the name of Lawrence Berkeley National Laboratory, U.S. Dept. of
#Energy nor the names of its contributors may be used to endorse or promote
#products derived from this software without specific prior written permission.
#
#THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
#ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
#WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
#DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR CONTRIBUTORS BE LIABLE FOR
#ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
#(INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
#LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON
#ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
#(INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
#SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
#
#---------------------------------------------------------------------------
#
#author:  Wei Tu
#email:   weitu@ocf.berkeley.edu
#
#############################################################


use strict;
use FileHandle;
use Getopt::Long;
    
# Global Variables
########################

my $version = "2.0";

my ($opt_sort, $opt_output, $opt_help, $opt_report);
my ($opt_internal, $opt_full, $opt_thread, $opt_filter);

my (%data, %report, %heapstats);
my (%threads); # maps thread pidstring => global thread num
my (%nodes); # maps thread pidstring => node num
my (%node_threads); # maps node num => the number of threads on that node
my (%job_nodes); # maps job idstring => num nodes
my (%job_seen); # maps job idstring => boolean job encountered before
my (%job_uniq); 
my $memtable; # textual table of memory info
my $heapstats_report;
my @heapstat_keys; # sorted desc keys into heapstats
my $tool_prefix = $ENV{'TOOL_PREFIX'} || 'gasnet';
my $tool_prefix_mc = ucfirst($tool_prefix);
$tool_prefix_mc =~ s/Gasnet/GASNet/;
$tool_prefix_mc =~ s/Upc/UPC/;
my $gasnet_version = $ENV{'VERSION'} || '?.?';
my $opt_debug = 0;
my $lang_mode = "GASNET";

#%nodes, %threads are identifier->thread(node)num

# Getting the Options
########################
    # hack: if we're being called by help2man as part of generating the upcrun
    # man page, we use a different 'version' option (we can't use the regular
    # one, since we'll have no valid conf file to read)
    my (@v) = grep {s/^-h2mversion=([0-9.]+)$/$1/} @ARGV;
    if (@v) {
        # help2man format
        print $tool_prefix."_trace @v\n";
        exit(0);
    }
    # same thing, but for help2man's -h2mhelp call
    if (grep { /-h2mhelp/ } @ARGV) {
        usage(undef, 1);
    }

GetOptions (
    'h|?|help'		=> \$opt_help,
    'd'			=> \$opt_debug,
    'sort=s'		=> \$opt_sort,
    'o=s'		=> \$opt_output,
    'report=s'		=> \$opt_report,
    't'			=> \$opt_thread,
    'thread!'		=> \$opt_thread,
    'i'			=> \$opt_internal,
    'internal!'		=> \$opt_internal,
    'f'			=> \$opt_full,
    'full!'		=> \$opt_full,
    'filter=s'		=> \$opt_filter
);

# The main routine
########################

usage() if $opt_help;

if (!@ARGV) {
    die "no tracefile(s) specified!\n";
}

if ($opt_output) {
    open(STDOUT, ">$opt_output") or die "Could not write to $opt_output: $!\n";
}

if (!$opt_report) {
    $opt_report="GET,PUT,BARRIER,TI_ARRAY_COPY,MEMORY";
} 

my ($got_memreport, $got_tracefile);

ARG: while (@ARGV) {
    my $arg = pop @ARGV;
    if ($arg =~ /%/) {
      for (my $i=0; ; $i++) {
	my $targ = $arg;
	$targ =~ s/%/$i/g;
	if (-f $targ) {
	  unshift @ARGV, $targ;
	} else {
	  die "No tracefiles found matching pattern '$arg'\n" if ($i == 0);
	  next ARG;
	}
      }
    }
    if (is_memreport($arg)) {
      parse_memreport($arg);
      $got_memreport = 1;
    } else {
      parse_threadinfo($arg);
      parse_tracefile($arg);
      $got_tracefile = 1;
    }
}
foreach my $job (keys %job_nodes) {
    my ($want, $have) = ($job_nodes{$job}, $job_seen{$job});
    if ($have < $want) {
	print STDERR "WARNING: only have traces for $have out of $want nodes of job $job\n";
    }
}

convert_report();
sort_report();
trace_output(*STDOUT, "GET")           if ($got_tracefile && $opt_report =~ /GET/);
trace_output(*STDOUT, "PUT")           if ($got_tracefile && $opt_report =~ /PUT/);
trace_output(*STDOUT, "BARRIER")       if ($got_tracefile && $opt_report =~ /BARRIER/);
trace_output(*STDOUT, "TI_ARRAY_COPY") if ($got_tracefile && $opt_report =~ /TI_ARRAY_COPY/ && $lang_mode eq "TITANIUM");
trace_output(*STDOUT, "MEMORY")        if ($got_memreport && $opt_report =~ /MEMORY/);
# Show program usage
########################
sub usage 
{
    my ($errormsg, $h2mhelp) = @_;

    print "${tool_prefix_mc} trace file summarization script, v${version} (GASNet v${gasnet_version})\n";
    print "Usage:  ${tool_prefix}_trace [options] trace-file(s)\n";
    print "  trace-file(s) may include any mix of ${tool_prefix_mc} trace files and local memory reports\n";
    if ($tool_prefix eq "upc" && !$h2mhelp) {
        print <<EOF;
For detailed documentation, please see man upc_trace(1) or http://upc.lbl.gov/docs/
EOF
    }
    print <<EOF;

Options:
    -h -? -help         See this message.
    -o [filename]       Output results to file. Default is STDOUT.
    -report [r1][r2]..  Indicate which reports to generate: 
    			PUT, GET, BARRIER, MEMORY, and/or TI_ARRAY_COPY.
                        Default: all reports.
    -sort [f1],[f2]...  Sort output by one or more fields: TOTAL, AVG, MIN, MAX,
                        CALLS, TYPE, or SRC. (for GET/PUT/MEMORY, TOTAL, AVG, MIN,
                        and MAX refer to size in bytes: for BARRIERS, to time
                        spent in barrier).  Default: sort by SRC 
    -filter [t1],[t2].. Filter out output by one or more types:
    			LOCAL, GLOBAL, WAIT, WAITNOTIFY.  
    -t -[no]thread      Output detailed information for each thread.
    -i -[no]internal    Show internal events (such as the initial and final
                        barriers) which do not correspond to user source code. 
    -f -[no]full        Show the full source file name.
    -d                  Enable debugging output for the parsing script.
EOF
    exit(-1);
}

# returns true if a given file is a memory report file
# args : the filename to be read.
########################
sub is_memreport
{
    open (TRACEFILE, $_[0]) or die "Could not open $_[0]: $!\n";
    my $line = <TRACEFILE> ;
    return ($line =~ m/GASNet Debug Mallocator Report/);
}

# subroutine to read a memory report file and dump the useful information into a 
# data-structure, namely an array of hashes and return the array.
# args : the filename to be read.
########################
sub parse_memreport
{
    my $filename = $_[0];
    my ($node, $numnodes);
    my $in_memtable = 0;
    open (TRACEFILE, $filename) or die "Could not open $filename: $!\n";
    print STDERR "Parsing memory report file for $filename\n";

    LINE:
    while (<TRACEFILE>) {
        if (m/^#\s*node:\s*(\d+)\s*\/\s*(\d+)/) {
          $node = $1; 
          $numnodes = $2;
	} elsif (m/^# Object size/) {
	  $in_memtable = 0;
        } elsif (m/^#\s+(Private memory utilization)/) {
	  if (!$got_memreport) {
	    $memtable .= "$1\n"; 
	    my $next = <TRACEFILE>;
	    $next =~ s/^#\s*//; 
	    $memtable .= $next;
	    $memtable .= "\nMEMORY_TABLE_SUMMARY"; 
	  }
	  $in_memtable = 1;
	} elsif ($in_memtable && m/^#\s+(.*)$/) { # memory table
	  my $line = $1;
	  if ($line =~ m/(malloc.*):.*?(\d+) bytes[^\d]*(\d+)?/) {
	    my ($desc, $sz, $cnt) = ($1,$2,$3);
	    # my $szstr = sprintf '%*s', length($sz)+6, shorten($1,"MEMORY");
	    # $line =~ s/$sz bytes/$szstr/;
	    push @heapstat_keys, $desc unless ($heapstats{$desc});
	    if ($desc =~ m/peak/) { 
	      if ($sz > $heapstats{$desc}{"SZ"}) {
	        $heapstats{$desc}{"SZ"} = $sz;
		$heapstats{$desc}{"CNT"} = $cnt;
              }
	    } else {
	      $heapstats{$desc}{"CNT"} += $cnt;
	      $heapstats{$desc}{"SZ"} += $sz;
	    }
	      
	  } elsif (!$got_memreport) {
	    $memtable .= "$line\n"; 
	  }
        } elsif (/\s*(\d+)\s+(\S+)/) {
          my ($sz, $src) = ($1, $2);
          push @{$data{"MEMORY"}{$src}{""}{$node}}, $sz; 
        }
    }
}

# subroutine to read the tracefile and dump the useful information into a 
# data-structure, namely an array of hashes and return the array.
# args : the filename to be read.
########################
sub parse_threadinfo
{
    my $filename = $_[0];
    open (TRACEFILE, $filename) or die "Could not open $filename: $!\n";
    print STDERR "Parsing thread info for $filename..\n";
    my ($nodes_complete);
    my (%thread_magic_seen, %node_seen, %node_magic_seen, %node_done, %node_threads_seen);
 
    LINE:
    while (<TRACEFILE>) {
        if (/GASNET_TRACEMASK: (\S+)/) {
 	  my $initial_mask = $1; # note that mask may change during execution
          unless ($initial_mask =~ m/[N]/) {
            print STDERR "WARNING: '$filename' was created with the N tracing category disabled.\n".
                         "WARNING: Line number information may be missing or incomplete.\n";
          }
          #unless ($initial_mask =~ m/[H]/) {
          #  die "ERROR: '$filename' was created with the H tracing category disabled.\n" .
          #      "ERROR: ${tool_prefix}_trace currently requires GASNET_TRACEMASK to include 'H'.\n";
          #}
        } elsif (/GASNET_TRACE_MAGIC/ && 
            m/^(\S+).*?I am thread (\d+) of (\d+).*?on node (\d+) of (\d+).*?in job <([^>]+)>.*$/) {
  	    my $job_id = $6;
            $threads{$1} = $2;
            $nodes{$1} = $4;
	    $node_magic_seen{$4} = 1;   # remember we saw magic from some thread on this node
            $thread_magic_seen{$1} = 1; # remember we saw this thread's magic
            $node_threads_seen{$4}++;   # track number of threads on this node
            # for error checking of total nodes/threads
            $job_nodes{$job_id} = $5;
            $job_seen{$job_id}++;
            if ($job_uniq{$job_id,$2}++) {
                print STDERR "WARNING: duplicate tracing data for thread $2 of job $job_id\n";
            }
            $lang_mode = "TITANIUM" if ($job_id =~ /^Ti:/);
            $lang_mode = "UPC" if ($job_id =~ /^UPC:/);
        } 
        elsif (/^(\d+).*?\(B\) BARRIER_WAIT/) {
		my $barrier_node = $1;
  		# ensure we got them all - esp tricky if multiple nodes are in same file
                # stop parsing for each node at the second barrier after magic lines for that node
                $node_done{$barrier_node}++ if ($node_magic_seen{$barrier_node});	
	        $nodes_complete++ if ($node_done{$barrier_node} == 2);
                next if ($nodes_complete == 0 || $nodes_complete < (scalar keys %node_seen));
                # By now magic lines of every thread seen have been processed
		if ($opt_debug) {
		  foreach my $node (keys %node_seen) {
		    print " - Node $node has threads: ";
		    foreach my $threadid (keys %nodes) {
			print $threads{$threadid}."[$threadid]  " if ($nodes{$threadid} == $node);
		    }
		    print "\n";
		  }
		}
		last;
        } elsif (/^(\d+)> Program/) {
	    $node_seen{$1} = 1;   # remember we saw this node's trace output in this file
	}
    }

    die "ERROR: '$filename' does not appear to be a valid GASNet trace file!\n" 
       unless (keys %node_magic_seen);
	
    # remember the number of threads per node
    foreach my $key (keys %node_threads_seen) {
      $node_threads{$key} = $node_threads_seen{$key};
    }
}

sub parse_tracefile 
{
    
    open (TRACEFILE, $_[0]) or die "Could not open $_[0]: $!\n";
    print STDERR "Parsing tracefile for $_[0]..  0%";
    
    my $file_size = (stat($_[0]))[7];
    # FILTERS for reports and types
    my %filters, my %reports;
    foreach my $filter (split /,/, $opt_filter) {
    	$filters{$filter}++;
    }
    foreach my $report (split /,/, $opt_report) {
    	$reports{$report}++;
    }
    
    # Counter for progress indication
    my $counter;
    
    # Flag for internal region
    my $inRegion;
    my $sawmagic;
    my %arraycopy_not_nb; # flag for nb copy failure detection
    while (<TRACEFILE>) {
    	unless ($opt_internal) {
    	    # If in region, skip unless we have a leaveregion
	    if (/GASNET_TRACE_LEAVEREGION/) {
	        $inRegion = 0;
	        next;
	    }
	    next if $inRegion;
            # Set the flag for entering a region
	    if (/GASNET_TRACE_ENTERREGION/) {
	        $inRegion = 1;
	        next;
	    }
	}
	$sawmagic = 1 if (/GASNET_TRACE_MAGIC/);
	    
	# Actual info
	my ($thread, $src, $pgb, $type, $sz);
	$counter++;
	if ($counter > 100000) {
	    my $percentage = int (tell(TRACEFILE) * 100 / $file_size);
	    if ($percentage >= 10) {
	    	print STDERR "\b";
	    } 
	    print STDERR "\b\b$percentage%";
	    $counter = 0;
	}
        if (/^(\S+) \S+ (\[[^\]]+\] )?\([HPGB]\) (PUT|GET|BARRIER|TI_ARRAY_COPY)([^:]*):\D+(\d+(\.\d+)?)?/) { 
            ($thread, $src, $pgb, $type, $sz) = ($1, $2, $3, $4, $5);
            # filter out lines that are not going to be in the report
            next unless $reports{$pgb};
            if ($pgb =~ /^(?:PUT|GET)/) {
	        $type = ($type =~ /_LOCAL$/) ? "LOCAL" : "GLOBAL";
            	# filter by type to increase performance
            	next if $filters{$type}; 
            } elsif ($pgb =~ /^BARRIER/) {
	        $type =~ s/^_//;
                next unless ($type =~ /^(?:NOTIFYWAIT|WAIT)/);	# discard unknowns
                next if $filters{$type};
                $thread = $nodes{$thread};
            } elsif ($pgb =~ /^TI_ARRAY_COPY/) {
		my $desc;
		if (/(issued as non-blocking.*)$/) {
		  $arraycopy_not_nb{$thread} = $1;
		  next;
                } else {
	          /TI_ARRAY_COPY: \(.*?\) \S+ (.*)$/;
                  $desc = $1; 
		  $desc .= "|NOTE: " . $arraycopy_not_nb{$thread} if ($arraycopy_not_nb{$thread});
		  $arraycopy_not_nb{$thread} = undef;
                }
		#$desc =~ s/ copy(?:ing)?//g;
		$desc =~ s/ region//g;
		$desc =~ s/\(local <- local\)//g;
		$desc =~ s/contiguous/contig/g;
		$desc =~ s/direction/dir/g;
		$desc =~ s/scatter-gather AM-based copy/strided /g;
		$desc =~ s/\(put:.*?\)/put/g;
		$desc =~ s/\(get:.*?\)/get/g;
		$type = "LOCAL";
		$type = "GLOBAL" if (m/remote/);
                next if $filters{$type};
		$type .= "|$desc";
            }

            if ($src =~ m/\[([^\]]+)\]/) { $src = $1; }
            elsif (!$sawmagic) { $src = "_STARTUP:0"; } # anything before the first magic is internal
            else { $src = "UNKNOWN:0"; } # a user operation with missing line number

            push @{$data{$pgb}{$src}{$type}{$thread}}, $sz;	
	}
    }
    
    print STDERR "\b\b\bdone\n";
}


# subroutine to canonicalize the msg size
# e.g -> 14336->14K, 2516582->2.4M
# args: the msg size to be canonicalized
########################
sub shorten
{
    my ($msg_sz, $type) = @_;
    if ($type =~ /GET|PUT|TI_ARRAY_COPY|MEMORY/) {
    	if ($msg_sz < 1024) {
    	    return sprintf("%.0f B", $msg_sz);
    	} elsif ($msg_sz < 1024 * 1024) {
    	    return sprintf("%.2f K", $msg_sz / 1024.0);
    	} elsif ($msg_sz < 1024 * 1024 * 1024) {
    	    return sprintf("%.2f M", $msg_sz / (1024.0 * 1024.0));
    	} elsif ($msg_sz < 1024 * 1024 * 1024 * 1024) {
    	    return sprintf("%.2f G", $msg_sz / (1024.0 * 1024.0 * 1024.0));
    	} else {
    	    return sprintf("%.2f T", $msg_sz / (1024.0 * 1024.0 * 1024.0 * 1024.0));
    	}
    } else {
    	if ($msg_sz < 1000) {
    	    return sprintf("%.1f us", $msg_sz);
    	} elsif ($msg_sz < 1000 * 1000) {
    	    return sprintf("%.1f ms", $msg_sz / 1000.0);
    	} elsif ($msg_sz < 1000 * 1000 * 60) {
    	    return sprintf("%.1f  s", $msg_sz / (1000.0 * 1000.0));
    	} else {
    	    return sprintf("%.1fmin", $msg_sz / (1000.0 * 1000.0 * 60.0));
    	}
    }
}

# subroutine to separate the source file name 
# and the line number
# args: the source line to be separated
#######################
sub src_line
{
    my ($line) = @_;
	
    $line =~ /(.*):(\d+)$/;    
    return ($1, $2);
}

# transfer the raw data structure into report -- a hash of arrays 
#######################
sub convert_report 
{
    print STDERR "Generating reports..\n";
    foreach my $pgb (keys %data) {
    	foreach my $line (keys %{$data{$pgb}}) {
    	    foreach my $type (keys %{$data{$pgb}{$line}}) {

    	    	my ($max, $min, $avg, $total, $totalc);
    	    	foreach my $thread (keys %{$data{$pgb}{$line}{$type}}) {
    	    	    # change the raw sizes to max, min, avg, total, totalc;
    	    	    @{$data{$pgb}{$line}{$type}{$thread}} 
    	    	        = get_minmax(@{$data{$pgb}{$line}{$type}{$thread}}); 
    	    	    
    	    	    # For Barrier $thread is actually the node number
    	    	    my ($tmax, $tmin, $tavg, $ttotal, $ttotalc) 
			= @{$data{$pgb}{$line}{$type}{$thread}};
    	    	    $max = $max > $tmax ? $max : $tmax;
    	    	    $min = ($min > $tmin || !$min) ? $tmin : $min;
    	    	    if ($pgb =~ /BARRIER/) {
    	    	        $total += $ttotal * $node_threads{$thread};
    	    	        $totalc += $ttotalc * $node_threads{$thread};
    	    	    } else { 
    	    	        $total += $ttotal;
    	    	        $totalc += $ttotalc;
                    }		
                    #print "pgb=$pgb line=$line type=$type thread=$thread ".
                    #      "node_threads{$thread}=".$node_threads{$thread}." total=$total totalc=$totalc\n";
    	    	}
		die "INTERNAL ERROR" unless $totalc;
    	    	$avg = $total / $totalc;
    	    	my @entry = ($line, $type, $max, $min, $avg, $total, $totalc);
		push @{$report{$pgb}}, \@entry; 
    	    }
    	}
    }
    if ($got_memreport) {
	  foreach my $desc (@heapstat_keys) {
	    my ($totsz,$totcnt) = ($heapstats{$desc}{"SZ"}, $heapstats{$desc}{"CNT"});
	    my $line = sprintf('  %35s %15s', $desc, shorten($totsz,"MEMORY"));
	    $line .= sprintf(' in %6i objects', $totcnt) if ($totcnt);
	    $heapstats_report .= "$line\n";
	  }
	  $memtable =~ s/MEMORY_TABLE_SUMMARY/$heapstats_report/;
    }
}

# get an array of raw msg sizes, return an array of max, min, avg, total and totalc
sub get_minmax 
{
    my @msgs = @_;
    my ($max, $min, $avg, $total, $totalc);
    $max = $msgs[0];
    $min = $msgs[0];
    foreach my $sz (@msgs) {
        if ($sz > $max) {
            $max = $sz;
        } 
        if ($sz < $min) {
            $min = $sz;
        }
        $total += $sz;
    }
    $totalc = scalar @msgs;
    $avg = $total / $totalc;
    return ($max, $min, $avg, $total, $totalc);         
}

# report_sorting criterion
#######################
sub criterion
{
    my @mtd = @_;
    my $result;
    my $sort_mtd = shift @mtd;
    # Breaking ties using the less important fields.
    while (!$result && $sort_mtd) {
    	if ($sort_mtd eq "CALLS") {
            $result = ${$b}[6] <=> ${$a}[6];;
    	} 
        if ($sort_mtd eq "TOTAL") {
            $result = ${$b}[5] <=> ${$a}[5];
        }
        if ($sort_mtd eq "AVG") {
            $result = ${$b}[4] <=> ${$a}[4];
        }
        if ($sort_mtd eq "MIN") {
            $result = ${$b}[3] <=> ${$a}[3];
        }
        if ($sort_mtd eq "MAX") {
            $result = ${$b}[2] <=> ${$a}[2];
        }
        if ($sort_mtd eq "TYPE") {
            $result = (${$a}[1] cmp ${$b}[1]);
        }
        if ($sort_mtd eq "SRC") {
            my ($a_src, $a_line) = src_line${$a}[0];
            my ($b_src, $b_line) = src_line${$b}[0];
            $result = ($a_src cmp $b_src) ||
                      ($a_line <=> $b_line);
        }
    	
    	$sort_mtd = shift @mtd;
    }
    return $result;
}

# sorting the report
########################
sub sort_report 
{

    my @sortmtd = split /,/, $opt_sort;
    # Checking for valid input
    foreach my $mtd (@sortmtd) {
        $mtd =~ /^(CALLS|AVG|MAX|MIN|TOTAL|SRC|TYPE)$/
        or die "Could not recognize $mtd\n"; 
    }
    
    foreach my $pgb (keys %report) {
	if ($opt_sort) {
	    @{$report{$pgb}} = sort {criterion(@sortmtd)} @{$report{$pgb}};
	} else {
	    @{$report{$pgb}} = sort {criterion("SRC")} @{$report{$pgb}};
    	}
    }
	 
}

sub get_threads 
{
    my ($node) = @_;
    my @threads;
    foreach my $identifier (keys %nodes) {
        if ($nodes{$identifier} == $node) {
            push @threads, $threads{$identifier};
        }
    }
    @threads = sort @threads;
    return $threads[0] . ".." . $threads[-1];
}       

# subroutine to process the data structure produced by the parse_tracefile 
# subroutine and print out in a format that the caller specifies.
# args:	-filehandler -- specifying where the output should go
########################
sub trace_output 
{
    my ($handle, $pgb) = @_;
    

    # Print out 
    print "\n$pgb REPORT:\n";
    

if ($pgb eq "MEMORY") {
    print <<EOF;

$memtable
SOURCE                     LINE   SZ:( min       max       avg     total) COUNT  
===============================================================================    	
EOF
} else {
    print <<EOF;
SOURCE         LINE    TYPE        MSG:(min    max     avg     total)     CALLS  
===============================================================================    	
EOF
}

    # Setting up variables;
    my ($src_num, $source, $lnum, $type, $min, $max, $avg, $total, $calls, $extra);
    my ($threadlabel, $threadnum, $tmin, $tmax, $tavg, $ttotal, $tcalls, $got_one);

    foreach my $entry (@{$report{$pgb}}) { 
        ($src_num, $type, $max, $min, $avg, $total, $calls) = @{$entry};
        ($source, $lnum) = src_line($src_num);
        # Skip internal events (having lnum==0) if not specified.
        next unless ($lnum || $opt_internal || $source eq "UNKNOWN");
        $got_one = 1;
        
        
        $max = shorten($max, $pgb);
        $min = shorten($min, $pgb);
        $avg = shorten($avg, $pgb);
        $total = shorten($total, $pgb);
        
        # Options for showing the full file name
        if ($pgb eq "MEMORY") {
	  next if (($source =~ m@/(gasnet|upcr)_[^/]*@) && !$opt_internal);
          if ($opt_full) {
	    printf "%s\n", $source;
	    $handle->format_name("MEMFULL");             
          } else {
            $source = substr $source, -25, 25;
            $handle->format_name("MEMDEFAULT");
          }
	} else {
          if ($opt_full) {
	    printf "%s\n", $source;
	    $handle->format_name("FULL");             
          } else {
            $source = substr $source, -14, 14;
            $handle->format_name("DEFAULT");
          }
	}
	if ($pgb =~ /BARRIER/) {
	  # bug 762 - don't display any cross-thread barrier call total if
	  # we're showing call totals for each thread, because it's confusing
	  if ($opt_thread) { $calls = ""; }
	  else { # and divide barrier call counts by thread count in non-thread
                 # view to represent the number of global calls. This could result
		 # in a fractional call count for non-single barriers, but that's
                 # an uncommon case, and this way the sum of the global barrier call
                 # count across source lines always represents the total number of global barriers
		$calls /= (scalar keys %nodes);
	  }
        }
        my $oldtype = $type;
        $type =~ m/^(.*?)(\|.*)?$/;
	($type, $extra) = ($1, $2);
        write($handle);
	$extra =~ s/^\|//; 
	for my $line (split(/\|/,$extra)) {
	  $extra = $line;
          $handle->format_name("EXTRA");
          write($handle);
        }
	$type = $oldtype;
        
        if ($opt_thread) {
            foreach my $thread (sort keys %{$data{$pgb}{$src_num}{$type}}) {
		$threadlabel = "Thread";
            	if ($pgb =~ /PUT|GET|TI_ARRAY_COPY/) {
            	    $threadnum = $threads{$thread};
            	} elsif ($pgb =~ /MEMORY/) {
		    $threadnum = $thread;
		    $threadlabel = " Node";
            	} else {
            	    $threadnum = get_threads($thread);
                }
            	($tmax, $tmin, $tavg, $ttotal, $tcalls) = 
            	    @{$data{$pgb}{$src_num}{$type}{$thread}};
    		$tmax = shorten($tmax, $pgb);
		$tmin = shorten($tmin, $pgb);
    		$tavg = shorten($tavg, $pgb);
		$ttotal = shorten($ttotal, $pgb);

    		$handle->format_name("THREAD");
    		write($handle);
    	    }
    	}
        
    }
    if (!$got_one) {
        print "NONE\n";
    }
    
# formats
########################
#@<<<<<<<<<<<<< @>>>> @>>>>>>>>>  @>>>>>>>> @>>>>>>>> @>>>>>>>> @>>>>>>>> @>>>>>@*

    format DEFAULT = 
@<<<<<<<<<<<<< @>>>> @>>>>>>>>>  @>>>>>>>> @>>>>>>>> @>>>>>>>> @>>>>>>>> @>>>>>
$source, $lnum, $type, $min, $max, $avg, $total, $calls
.

    format FULL = 
               @>>>> @>>>>>>>>>  @>>>>>>>> @>>>>>>>> @>>>>>>>> @>>>>>>>> @>>>>>
               $lnum, $type, $min, $max, $avg, $total, $calls
.
	    
    format EXTRA = 
~~  ^<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<
                  $extra
.

    format THREAD =
    @<<<<< @<<<<<<<<<<<<         @>>>>>>>> @>>>>>>>> @>>>>>>>> @>>>>>>>> @>>>>>
$threadlabel, $threadnum, $tmin, $tmax, $tavg, $ttotal, $tcalls
.

    format MEMDEFAULT = 
@<<<<<<<<<<<<<<<<<<<<<<<< @>>>>  @>>>>>>>> @>>>>>>>> @>>>>>>>> @>>>>>>>> @>>>>>
$source, $lnum, $min, $max, $avg, $total, $calls
.

    format MEMFULL = 
                          @>>>>  @>>>>>>>> @>>>>>>>> @>>>>>>>> @>>>>>>>> @>>>>>
                          $lnum, $min, $max, $avg, $total, $calls
.
}

        

   
