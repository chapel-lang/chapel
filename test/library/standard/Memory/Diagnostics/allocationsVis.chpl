use MemDiagnostics;
use CTypes;
use Image;
use List;

config var niter = 100;
config var arrSize = 1000;
config const deleteFreq = 2;
config var expandRate = 1.05;

config const memSpaceInGigs = 64;

// for the video
config const height = 100;
config const width = 1024;
config var pixelVal = 255;

class C {
  var size: int;
  var Arr: [1..size] int;

  proc init(size) { this.size = size; }
}

record heapVis {
  var pipe: mediaPipe;
  var height, width: int;

  var Frame: [0..<height, 0..<width] int;

  proc init(filename, height, width) {
    this.pipe = new mediaPipe(filename, imageType.bmp);
    this.height = height;
    this.width = width;
  }

  proc ref recordHeapAsFrame() {
    var largestAddr = min(uint);

    for (addr, size) in MemDiagnostics.allocations() {
      if addr>largestAddr then largestAddr = addr;
    }

    const granularity = (memSpaceInGigs*(1024**3)/width);

    for (addr, size) in MemDiagnostics.allocations(granularity) {
      const start = (largestAddr-addr)/granularity;
      const numLines = size/granularity;

      for i in start..#numLines {
        Frame[.., width-i:int] = pixelVal;
      }
    }

    pipe.writeFrame(Frame);
    Frame = 0;
  }
}

proc main() {
  var heapVisualizer = new heapVis("mem.mp4", height, width);
  var prev: unmanaged C? = nil;
  var cleanup: list(unmanaged C?);

  test(heapVisualizer, cleanup);

  arrSize /= 4;
  niter = 50;
  expandRate = 1.0;

  test(heapVisualizer, cleanup);

  for item in cleanup do delete item;
}

proc test(ref vis, ref cleanup) {
  var prev: unmanaged C? = nil;
  for i in 0..#niter {
    var c = new unmanaged C?(arrSize);

    if i%deleteFreq == 1 {
      delete prev;
      vis.recordHeapAsFrame();
      arrSize = (arrSize * expandRate):int;
    }
    else {
      assert(c!.Arr[1] == 0);
      cleanup.pushBack(c);
    }

    prev = c;
  }
}
