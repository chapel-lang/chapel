var HostArr: [1..5] int;  // allocated on the host
HostArr = 1;              // executes on [multicore] CPU

on here.gpus[0] {
  var DevArr: [1..5] int;  // allocated on the device
  DevArr += 1;             // executes on GPU as a kernel
  writeln(DevArr);         // prints "1 1 1 1 1"
}

writeln(HostArr);  // prints "1 1 1 1 1"
