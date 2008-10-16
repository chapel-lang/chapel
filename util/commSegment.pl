#!/usr/bin/perl
use File::Basename;

$utildirname = dirname($0);

$comm = `$utildirname/comm.pl`;
chomp($comm);

if ($comm eq "gasnet") {
  $preset_segment = $ENV{'CHPL_GASNET_SEGMENT'};
  if ($preset_segment eq "") {
    $substrate = `$utildirname/commSubstrate.pl`;
    chomp($substrate);

    if ($substrate eq "portals") {
      $segment = "fast";
    } else {
      $segment = "everything";
    }
  } else {
    $segment = $preset_segment;
  }
} else {
  # if ($comm eq "armci") ...
  $segment = "none";
}

print "$segment\n";
exit(0);
