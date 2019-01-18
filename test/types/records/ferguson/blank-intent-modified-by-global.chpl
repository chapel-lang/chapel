record R {
  var x:int;
}

var globalInt = 1;
var globalR = new R(100);

proc byblank( arg1:int, arg2:R ) {
  globalInt = 2;
  globalR = new R(200);
  writeln("in byblank ", (arg1, arg2));

  var a = arg1;
  var b = arg2;

  globalInt = 3;
  globalR = new R(300);
  writeln("in byblank a=", a, " b=", b);

}

proc run() {
  writeln("in run ", (globalInt, globalR));
  byblank( globalInt, globalR );
}

run();
