import RangeChunk.chunks;

config const n = 10;         // now, our application has `--n` to set this!
config const sliceSize = 5;  // number of elements per slice

const numSlices = n/sliceSize; // assume divisibility for simplicity

var HostArr: [1..n] int;  // allocated on the host
HostArr = 1;              // executes on [multicore] CPU

on here.gpus[0] {
  coforall chunk in chunks(1..n, numSlices) {
    var DevArr: [chunk] int;  // allocated on the device *per task*

    DevArr = HostArr[chunk];  // copy a slice from host to device
    DevArr += 1;              // executes on GPU as a kernel
    HostArr[chunk] = DevArr;  // copy from device to a slice on host
  }
}

writeln(HostArr);  // prints "2 2 2 2 2 ..."
