use driver_domains;
private use CommDiagnostics;

config const doVerboseComm = false;

const bigDom4D = Dom4D.expand((1,1,1,1));
var A: [bigDom4D] 4*int = {(...bigDom4D.dims())};
var B: [Dom4D] 4*int;

startTrial();
ref AS = A[Dom4D];
stopTrial(B, "create slice (module-scope)", skipVerify=true);

startTrial();
B = AS;
stopTrial(B, "use slice via assignment (module-scope)");

startTrial();
forall i in Dom4D do
  B[i] = AS[i];
stopTrial(B, "use slice via random access (module-scope)");

startTrial();
forall a in AS do
  var x = a;
stopTrial(B, "use slice as standalone (module-scope)", skipVerify=true);

startTrial();
forall (a,b) in zip(AS,B) do
  b = a;
stopTrial(B, "use slice as leader (module-scope)");

startTrial();
forall (b,a) in zip(B,AS) do
  b = a;
stopTrial(B, "use slice as follower (module-scope)");

startTrial();
foo(AS);
stopTrial(B, "1-arg promotion (module-scope)");

startTrial();
bar(B, AS);
stopTrial(B, "2-arg promotion (module-scope)");

proc main() {
  const bigDom4D = Dom4D.expand((1,1,1,1));
  var A: [bigDom4D] 4*int = {(...bigDom4D.dims())};
  var B: [Dom4D] 4*int;

  startTrial();
  ref AS = A[Dom4D];
  stopTrial(B, "create slice (local-scope)", skipVerify=true);

  startTrial();
  B = AS;
  stopTrial(B, "use slice via assignment (local-scope)");

  startTrial();
  forall i in Dom4D do
    B[i] = AS[i];
  stopTrial(B, "use slice via random access (local-scope)");

  startTrial();
  forall a in AS do
    var x = a;
  stopTrial(B, "use slice as standalone (local-scope)", skipVerify=true);

  startTrial();
  forall (a,b) in zip(AS,B) do
    b = a;
  stopTrial(B, "use slice as leader (local-scope)");

  startTrial();
  forall (b,a) in zip(B,AS) do
    b = a;
  stopTrial(B, "use slice as follower (local-scope)");

  startTrial();
  foo(AS);
  stopTrial(B, "1-arg promotion (local-scope)");

  startTrial();
  bar(B, AS);
  stopTrial(B, "2-arg promotion (local-scope)");
}

proc startTrial() {
  resetCommDiagnostics();
  startCommDiagnostics();
  if doVerboseComm then startVerboseComm();
}

proc stopTrial(B, str, skipVerify=true) {
  if doVerboseComm then stopVerboseComm();
  stopCommDiagnostics();
  writeln(str, ":\n", getCommDiagnostics());
  if !skipVerify then
    for i in Dom4D do if B[i]!=i then writeln("ERROR: B[", i, "]==", B[i]);
  B = (0,0,0,0);
}  

proc foo(a: A.eltType) {
}

proc bar(ref b: B.eltType, a: A.eltType) {
  b = a;
}
