
config var confighelp = true;

record F1 { proc deinit() { writeln("F1.deinit()"); } }
record F2 { proc deinit() { writeln("F2.deinit()"); } }
record F3 { proc deinit() { writeln("F3.deinit()"); } }
record F4 { proc deinit() { writeln("F4.deinit()"); } }
{
  writeln("checking helpers F1-F4");
  var f1: F1;
  var f2: F2;
  var f3: F3;
  var f4: F4;
}

record RR { var f1: F1; var f2: F2; }
{
  writeln("checking field order in RR");
  var rr: RR;
}

class CC { var f1: F1; var f2: F2; }
{
  writeln("checking field order in CC");
  var cc = new borrowed CC();
}

class DD: CC { var f3: F3; var f4: F4; }
{
  writeln("checking field order in DD, child of CC");
  writeln(" dd1");
  var dd1 = new unmanaged DD();
  delete dd1;
  writeln(" dd2");
  var dd2: unmanaged CC = if confighelp then new unmanaged DD() else new unmanaged CC();
  delete dd2;
  anotherCheckDD(true);
  anotherCheckDD(false);
}
proc anotherCheckDD(arg:bool) {
  writeln(" dd3 - ", arg);
  var dd3: unmanaged CC = if arg then new unmanaged DD() else new unmanaged CC();
  delete dd3;
}

class EE {
  var f1: F1;
  var f2: F2;
  proc deinit() { writeln("EE.deinit()"); }
}
{
  writeln("checking field and deinit order in EE");
  var ee = new unmanaged EE();
  delete ee;
}

class FF: EE {
  var f3: F3;
  var f4: F4;
  // As of Jan 2017, it is undefined whether EE.deinit() is invoked
  // before or after or in the middle of f3.deinit() and f4.deinit().
  proc deinit() { writeln("FF.deinit()"); }
}
{
  writeln("checking field and deinit order in FF, child of EE");
  var ff = new unmanaged FF();
  delete ff;
}
