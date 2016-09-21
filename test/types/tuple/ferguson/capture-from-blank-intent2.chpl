
var globalInt = 1;
var globalString = "hello";

proc byblank( arg: (int, string, (int, string)) ) {
  globalInt = 2;
  globalString = "byblank";
  writeln("in byblank ", arg);

  var (a, b, c) = arg;

  globalInt = 3;
  globalString = "byblank2";
  writeln("in byblank a=", a, " b=", b, " c=", c);
}

proc run() {
  writeln("in run ", (globalInt, globalString, (globalInt, globalString)));
  byblank( (globalInt, globalString, (globalInt, globalString)) );
}

run();
