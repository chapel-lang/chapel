

use BlockDist;
proc main() {
  var arr = blockDist.createArray(0..#10, int);
  arr = arr.domain;
  coforall loc in Locales do on loc {
    const myVar = loc.id;
    writeln("Hello from locale ", myVar);
    const mySlice = arr[arr.localSubdomain()];
    writeln("My slice is: ", mySlice);
    import Debugger; Debugger.breakpoint;
  }
}
