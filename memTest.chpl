use MemDiagnostics;
use CTypes;
use Image;

config var niter = 100;
config var arrSize = 1000;

class C {
  var size: int;
  var Arr: [1..size] int;

  proc init(size) { this.size = size; }
}

config const height = 100;
config const width = 1024;
config const deleteFreq = 2;
config var expandRate = 1.05;
config var pixelVal = 255;

const physicalMemory = here.physicalMemory();

var pipe = new mediaPipe("mem.mp4", imageType.bmp);

proc main() {
  writeln(physicalMemory);
  var totalMemoryFreed = 0;
  var prev: unmanaged C? = nil;
  for i in 0..#niter {
    var c = new unmanaged C(arrSize);
    prev = c;

    if i%deleteFreq == 1 {
      delete prev;
      recordMemoryMap();
      arrSize = (arrSize * expandRate):int;
      totalMemoryFreed += arrSize*numBytes(c.Arr.eltType);
      writeln("Total Memory Freed: ", totalMemoryFreed);
    }
    else {
      assert(c.Arr[1] == 0);
    }
  }

  arrSize /= 4;
  niter = 50;
  expandRate = 1.0;

  for i in 0..#niter {
    var c = new unmanaged C(arrSize);
    prev = c;

    if i%deleteFreq == 0 {
      delete prev;
      recordMemoryMap();
      arrSize = (arrSize * expandRate):int;
      totalMemoryFreed += arrSize*numBytes(c.Arr.eltType);
      writeln("Total Memory Freed: ", totalMemoryFreed);
    }
    else {
      assert(c.Arr[1] == 0);
    }
  }
}

iter allocsToRender(granularity=0:uint) {
  const hashSize = get_hashsize();
  for i in 0..<hashSize {
    var entry = get_memtable_entry(i);
    while entry != nil {
      var addr = get_memtable_entry_addr(entry);
      var size = get_memtable_entry_size(entry);

      if size >= granularity then yield (addr,size);

      entry = get_next_memtable_entry(entry);
    }
  }
}

config const memSpaceInGigs = 64;


proc recordMemoryMap() {
  var smallestAddr = max(uint);
  var largestAddr = min(uint);

  var color: [0..<height, 0..<width] int;

  for (addr, size) in MemDiagnostics.allocations() {
    if addr<smallestAddr then smallestAddr = addr;
    if addr>largestAddr then largestAddr = addr;
  }

  /*writeln(smallestAddr);*/
  /*writeln(largestAddr);*/

  const granularity = (memSpaceInGigs*(1024**3)/width);

  /*writeln(granularity);*/


  for (addr, size) in MemDiagnostics.allocations(granularity) {
    const start = (largestAddr-addr)/granularity;
    const numLines = size/granularity;

    for i in start..#numLines {
      color[.., width-i:int] = pixelVal;
    }
  }


  /*var format = (rgbColor.blue, rgbColor.green, rgbColor.red);*/
  /*var arr = colorToPixel(color, format=format);*/

  /*writeImage("pixels2.bmp", imageType.bmp, scale(arr, 2));*/

  printGrid(color);
}

/* Print helpers to write an image of the automaton */
proc printGrid(Grid) {
  pipe.writeFrame(Grid);
}

