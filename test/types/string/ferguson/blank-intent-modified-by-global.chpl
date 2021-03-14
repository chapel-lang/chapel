var globalInt = 1;
var globalString = "hello";

proc byblank( arg1:int, arg2:string ) {
  globalInt = 2;
  globalString = "byblank";
  writeln("in byblank ", (arg1, arg2));

  var a = arg1;
  var b = arg2;

  globalInt = 3;
  globalString = "byblank2";
  writeln("in byblank a=", a, " b=", b);

}

proc run() {
  writeln("in run ", (globalInt, globalString));
  byblank( globalInt, globalString );
}

run();
