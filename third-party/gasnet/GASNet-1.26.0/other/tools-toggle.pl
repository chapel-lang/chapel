#!/usr/bin/env perl

require 5.005;
use strict;

my $toolsonly;

if ($ARGV[0] eq "-y") { $toolsonly = 1; }
elsif ($ARGV[0] eq "-n") { $toolsonly = 0; }
else {
  die "Usage: $0 [-y|-n] files...\n";
}
shift;

my $insec=0;
my $show=0;
my $cc;
while (@ARGV) {
  my $filename = $ARGV[0];
  print " + Processing $filename...\n";     

  open(FIN, "<$filename");                                              
  open(FOUT, ">$filename.tmp");                                                                 
  while (defined(my $line = <FIN>)) {
    if ($line =~ m/^(.+)GASNET_TOOLSONLY_(TRUE|FALSE)_BEGIN/) { 
      $cc = $1;
      $show = ($2 eq ($toolsonly?"TRUE":"FALSE"));
      $insec = 1; 
    } elsif ($line =~ m/GASNET_TOOLSONLY_(TRUE|FALSE)_END/) { 
      $insec = 0; 
    } elsif ($insec) { 
      if ($show) {
        $line =~ s/^(?:$cc)*(.*)$/$1/;
      } else {
        $line =~ s/^(?:$cc)*(.*)$/$cc$1/;
      } 
    }
    print FOUT $line;
  }
  close(FIN);
  close(FOUT);
  rename("$filename.tmp", $filename);                              
  shift;
}
