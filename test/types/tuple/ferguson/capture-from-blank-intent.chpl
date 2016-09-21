
var globalInt = 1;
var globalString = "hello";

proc byblank( arg: (int, string) ) {
  globalInt = 2;
  globalString = "byblank";
  writeln("in byblank ", arg);

  var (a, b) = arg;

  globalInt = 3;
  globalString = "byblank2";
  writeln("in byblank a=", a, " b=", b);
}

proc run() {
  writeln("in run ", (globalInt, globalString));
  byblank( (globalInt, globalString) );
}

run();
