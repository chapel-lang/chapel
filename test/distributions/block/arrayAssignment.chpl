// test from https://github.com/chapel-lang/chapel/issues/18747
module Spam {
    config param testVersion: int = 1;
    use BlockDist;
    use CTypes;
    use IO;
    use IO.FormattedIO;
    use Reflection;

    type region_desc = c_ptr(void);

    class Sam {
        proc lookup_region(regionName:string): region_desc throws {
            return nil;
        }
    }

    class Region {
        const regionName: string;
        var regionDescs = blockDist.createArray({0..#numLocales}, region_desc);
        const size: uint(64);
        var iAmDestroyed = false;

        proc init(regionName: string, size: uint(64)) {
            this.regionName = regionName;
            this.size = size;
        }

    }
    proc newSharedRegion(regionName: string, size: uint(64),
                         ref regionDescs: [] region_desc) throws {
        try! stderr.writeln("%s %i".format(getRoutineName(), getLineNumber()));
        var region = new shared Region(regionName, size);
        try! stderr.writeln("%s %i".format(getRoutineName(), getLineNumber()));
        if testVersion == 1 {
          region.regionDescs = regionDescs;
        } else {
          forall (d, s) in zip(region.regionDescs, regionDescs) do
            d = s;
        }
        try! stderr.writeln("%s %i".format(getRoutineName(), getLineNumber()));
        return region;
    }

    var sammy = new Sam();

    private proc regionLookups(ref regionDescs, regionName: string) throws {
        try {
            forall r in regionDescs {
                var rName = regionName;
                r = sammy.lookup_region(rName);
            }
        }
    }

    private proc lookup(regionName: string) throws {
        var regionDescs = blockDist.createArray(0..#numLocales, region_desc);
        regionLookups(regionDescs, regionName);

        return newSharedRegion(regionName, 1024, regionDescs);
    }

    proc main() : int {
        var a = try! lookup("a");

        return 0;
    }
}

