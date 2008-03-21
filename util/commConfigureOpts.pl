#!/usr/bin/perl

#
# Build up options to pass to the comm-layer's configure step
# Use environment variables to help if they are set.
# CHPL_GASNET_SEGMENT can be set to: fast, large, or everything
# CHPL_GASNET_FORCE_ALIGNED is true if set to any string but 'false'
# Takes one argument that can be set to GASNet conduit names
#

$comm = $ENV{'CHPL_COMM'};
if ($comm eq "gasnet") {
  $conduit = $ARGV[0];
  $segment = $ENV{'CHPL_GASNET_SEGMENT'};
  $alignment = $ENV{'CHPL_GASNET_FORCE_ALIGNED'};
  $devel = $ENV{'CHPL_DEVELOPER'};

  if ($segment eq "") {
    if ($conduit eq "portals") {
      $cfgOpts = "--enable-segment-fast";
      $segment = "fast";
    } else {
      $cfgOpts = "--enable-segment-everything";
      $segment = "everything";
    }
  }

  if ($segment eq "everything" ||
      $segment eq "fast" ||
      $segment eq "large")  {
    $cfgOpts = "--enable-segment-$segment";
  } else {
    $cfgOpts = ""; # error?
  }

  if ($alignment eq "") {
    if ($segment eq "fast" || $segment eq "large") {
      $cfgOpts = "$cfgOpts --disable-aligned-segments";
    }
  }

  if ($devel ne "" && $devel ne "false") {
    $cfgOpts = "$cfgOpts --enable-debug"
  }
}

print "$cfgOpts\n";
exit(0);


