var HostArr: [1..10, 1..10] int = 5;

on here.gpus[0] {
  var DevArr: [1..10, 1..2] int;

  DevArr = HostArr[1..10, 1..2]; // copy the first 2 elements of each row
  writeln(DevArr);

  @assertOnGpu
  foreach d in DevArr do d = 1;

  HostArr[1..10, 1..2] = DevArr;

}

writeln(HostArr);
