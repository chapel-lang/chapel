
var globalTup = (1, "hello");


proc byblank( arg: (int, string) ) {
  globalTup(1) = 3;
  globalTup(2) = "byblank";
  writeln("in byblank ", arg);
}

proc byref( ref arg: (int, string) ) {
  globalTup(1) = 2;
  globalTup(2) = "byref";
  writeln("in byref ", arg);
  byblank(arg);
}

proc run() {
  writeln("in run ", globalTup);
  byref( globalTup );
}

run();
