
var globalTup = (1, "hello");


proc byblank( arg: (int, string) ) {
  globalTup(0) = 3;
  globalTup(1) = "byblank";
  writeln("in byblank ", arg);
}

proc byref( ref arg: (int, string) ) {
  globalTup(0) = 2;
  globalTup(1) = "byref";
  writeln("in byref ", arg);
  byblank(arg);
}

proc run() {
  writeln("in run ", globalTup);
  byref( globalTup );
}

run();
