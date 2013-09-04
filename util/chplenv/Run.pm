package Run;

use strict;
use Exporter;
use File::Basename;
use vars qw($VERSION @ISA @EXPORT @EXPORT_OK %EXPORT_TAGS);

$VERSION     = 1.00;
@ISA         = qw(Exporter);
@EXPORT      = ();
@EXPORT_OK   = qw(&run &ret);
%EXPORT_TAGS = ( DEFAULT => [qw(&run &ret)] );

# Run a program in the chplenv directory.
# This subroutine can take arguments; they will be
# supplied to the program. If it is a Perl program,
# we assume that it returns a result with ret($value).
sub run  {
  my $prog = shift;
  my @args = @_;

  my $utildirname = dirname(__FILE__);
  $prog = "$utildirname/$prog";

  # open $prog to check to see if it's a perl script.
  my $isperl = 0;
  open(my $fh, '<', $prog) or die "Could not find $prog";
  my $line = <$fh>;
  if ($line =~ /^#/ and $line =~ /perl/) {
    $isperl = 1;
  }
  close($fh);
  # If it's a Perl script, we assume that it returns
  # its value with ret(some-value).
  # Typically this will be done in code like
  # ret($atomics);
  # print "$atomics\n" unless caller;
  #
  if( $isperl ) {
    local @ARGV = @args;
    our $return = undef;
    do $prog;
    return $return;
  } else {
    my $got = `$prog @args`;
    chomp $got;
    return $got;
  }
}

# Return a result to a calling Perl program.
sub ret {
  my $value = shift;
  our $return = $value;
}

1;
