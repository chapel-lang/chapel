record R {
  var x:int;
}

var globalInt = 1;
var globalR = new R(100);

proc byblank( arg: (int, R, (int, R)) ) {
  globalInt = 2;
  globalR = new R(200);
  writeln("in byblank ", arg);

  var (a, b, c) = arg;

  globalInt = 3;
  globalR = new R(300);
  writeln("in byblank a=", a, " b=", b, " c=", c);
}

proc run() {
  writeln("in run ", (globalInt, globalR, (globalInt, globalR)));
  byblank( (globalInt, globalR, (globalInt, globalR)) );
}

run();
