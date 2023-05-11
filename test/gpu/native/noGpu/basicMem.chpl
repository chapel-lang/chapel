use MemDiagnostics;

startVerboseMem();
on here.gpus[0] {
  var A: [1..10] int;
}
stopVerboseMem();
