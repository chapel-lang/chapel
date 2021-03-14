module OuterModule {
  use ReplicatedVar, ReplicatedDist;
  use Utils;

  module Utils {

    record R {
      /* Note that the issue appears to be related to the domain.... if
         we comment this line out, it works again...

         Although defining R in the main module also works
      */
      var D: domain(1);
      var x : int;

      // New style initializer
      proc init() {
      }

      // No checks
      inline proc this(x: real): real {
        return 0.0;
      }
    }
  }

  proc setup() {
    return new R();
  }

  proc main() {
    var a : [rcDomain] R;
    rcReplicate(a, setup());
  }
}
