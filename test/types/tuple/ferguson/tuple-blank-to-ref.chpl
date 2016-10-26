
var globalInt = 1;
var globalString = "hello";

proc byref( ref arg: (int, string) ) {
  globalInt = 3;
  globalString = "byref";
  writeln("in byref ", arg);
}

proc byblank( arg: (int, string) ) {
  globalInt = 2;
  globalString = "byblank";
  writeln("in byblank ", arg);
  byref(arg);
}

proc run() {
  writeln("in run ", (globalInt, globalString));
  byblank( (globalInt, globalString) );
}

run();
