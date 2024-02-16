var HostArr: [1..5] int;  // allocated on the host
HostArr += 1;             // executes on [multicore] CPU

on here.gpus[0] {
  var DevArr: [1..5] int;  // allocated on the device

  DevArr = HostArr;  // copy from host to device
  DevArr += 1;       // executes on GPU as a kernel
  HostArr = DevArr;  // copy from device to host
}

writeln(HostArr);  // prints "2 2 2 2 2"
