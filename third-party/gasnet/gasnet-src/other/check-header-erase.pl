#!/usr/bin/env perl

# check-header-erase by Dan Bonachea
# Checks that all #defines in a header file are guaranteed to be preceded by an #undef.
# The check is conservative in that the preceding #undef must appear in a scope that
# equals or encloses the scope containing the #define.
# Additionally checks that any 'public' #defines (those that do not begin with '_')
# have a preceding #undef at the global scope level (the "always parsed" scope).
#
# This condition is sufficient to ensure that a header file is "self-erasing", 
# specifically that it can be safely #included multiple times in succession without
# encountering any errors concerning redefinition, and furthermore that only the
# public #defines from the 'last' #include will remain.

require 5.005;
use strict;

my $debug;
if ($ARGV[0] eq "-d") {
  $debug = 1;
  shift;
}

my $filename = $ARGV[0];
my $globallevel = $ARGV[1];
$globallevel = 0 if !defined($globallevel);

if (!($globallevel =~ m/^\d+$/) or ! -f $filename) {
  die "Usage: $0 [-d] <header_filename> [<global_level>]\n";
}

my $level=0;
my $linenum;
my $cur_linenum=0;
my $errs=0;
my %first_undef = {}; # symbol => line of first #undef
my %symstate = {};    # symbol => (+) line of last #define or (-) last of last #undef
my @undef_stack = (); # [ scope level ] : symbol => line of first #undef in current scope
push @undef_stack, {};

sub error {
  my ($err) = @_;
  print "$filename:$linenum ERROR: $err\n";
  $errs++;
}
#use Data::Dumper;

open(FIN, "<$filename");
while (defined(my $line = <FIN>)) {
  $cur_linenum++;
  $linenum = $cur_linenum;
  $line =~ s@/\*.*\*/@@g; # strip inline C comments
  while ($line =~ s/\\$// or   # concatenate split lines
         $line =~ m@/\*.*$@) { # concatenate lines ending with a multi-line C comment
    $line =~ s@\n$@@;
    $line .= <FIN>;
    $cur_linenum++;
    $line =~ s@/\*.*\*/@@g; # strip inline C comments
  }
  $line =~ s@//.*$@@; # strip C++ comments
  #print Dumper(\%symstate);
  if ($line =~ m/^\s*#\s*define\s+(\w+)(?:\s+(.*))?$/) { 
    print "$linenum \tD:"." "x$level."$1=$2\n" if $debug;
    my $sym = $1;
    if (not $first_undef{$sym}) {
      error("No prior #undef $sym");
    } elsif ($sym =~ m/^[^_]/ and not $undef_stack[$globallevel]{$sym}) {
      error("No prior #undef $sym of public symbol at global scope $globallevel");
    } else {
      #print Dumper(\@undef_stack);
      my $found;
      for (my $scope = $#undef_stack; $scope >= 0; $scope--) {
        my $lookup = $undef_stack[$scope]{$sym};
	if ($lookup) {
	  $found = $lookup;
	  last;
	}
      }
      error("No prior #undef $sym in current or enclosing scope") unless $found;
    }
    if ($symstate{$sym} > 0) { # currently too imprecise to be useful
      # error("Possible redefinition of $sym. Prior definition at line ".$symstate{$sym});
    }
    $symstate{$sym} = $linenum;
  } elsif ($line =~ m/^\s*#\s*undef\s+(\w+)\s*$/) {
    print "$linenum \tU:"." "x$level."$1\n" if $debug;
    my $sym = $1;
    $first_undef{$sym} = $linenum unless $first_undef{$sym};
    $undef_stack[$level]{$sym} = $linenum;
    $symstate{$sym} = - $linenum;
  } elsif ($line =~ m/^\s*#\s*if(?:n?def)?\b/) {
    $level++;
    push @undef_stack, {};
  } elsif ($line =~ m/^\s*#\s*else\b/ or $line =~ m/^\s*#\s*elif\b/) {
    pop @undef_stack;
    push @undef_stack, {};
  } elsif ($line =~ m/^\s*#\s*endif\b/) {
    pop @undef_stack;
    $level--;
  }
}

exit not $errs==0;

