var globalInt = 0;

proc someFunction() {
  return 22;
}

proc returnRef2() ref {
  return globalInt;
}

proc returnRef2() {
  var x : int = someFunction();

  return x;
}


returnRef2() = 1;      // resolves to 'ref' return intent version
var y = returnRef2(); // resolves to default/const ref return intent

writeln(globalInt);
writeln(y);

