module Spam {
  use BlockDist;
  use CTypes;

  type region_desc = c_ptr(void);

  class sam {
    proc lookup_region(regionName:string): region_desc throws {
      return nil;
    }
  }

  var sammy = new sam();

  private proc regionLookups(ref regionDescs, regionName: string) throws {
    // Won't break if the try isn't there.
    try {
      // The "with in" seems to be required as well
      forall r in regionDescs with (in regionName) {
        r = sammy.lookup_region(regionName);
      }
    }
  }

  private proc lookup(regionName: string) throws {
    var regionDescs = blockDist.createArray(0..#numLocales, region_desc);
    regionLookups(regionDescs, regionName);
  }

  proc main() : int {
    try! lookup("a");

    return 0;
  }
}
