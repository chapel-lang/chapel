#!/usr/bin/perl

$flag = $ARGV[0];

if ($flag eq "" || $flag eq "--host") {
    $preset_platform=$ENV{'CHPL_HOST_PLATFORM'};
} elsif ($flag eq "--target") {
    $preset_platform=$ENV{'CHPL_TARGET_PLATFORM'};
    if ($preset_platform eq "") {
        $preset_platfrom=$ENV{'CHPL_HOST_PLATFORM'};
    }
}


if ($preset_platform eq "") {
    $platform = `uname -s`;
    chomp($platform);
    $platform = lc($platform);
    $underscore = index($platform, "_");
    if ($underscore != -1) {
        substr($platform, $underscore) = "";
    }
    if ($platform eq "linux") {
        $machine = `uname -m`;
        chomp($machine);
        if ($machine eq "x86_64") {
            $build64_32 = $ENV{'CHPL_BUILD_X86_64_AS_32'};
            if ($build64_32 eq "1") {
                $platform = "linux64_32";
            } else {
                $platform = "linux64";
            }
        }
    }
} else {
    $platform = $preset_platform;
}


print "$platform\n";
exit(0);


