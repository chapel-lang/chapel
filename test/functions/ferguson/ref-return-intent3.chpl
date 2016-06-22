var globalInt = 0;

proc someFunction() {
  return 22;
}

proc returnRef2(a) ref {
  writeln(a);
  return globalInt;
}

proc returnRef2(a) {
  writeln(a);
  var x : int = someFunction();

  return x;
}


{
  var a = 0;

  returnRef2(a) = 1;      // resolves to 'ref' return intent version
  var y = returnRef2(a); // resolves to default/const ref return intent

  writeln(globalInt);
  writeln(y);
}

{
  var a = "a";

  returnRef2(a) = 1;      // resolves to 'ref' return intent version
  var y = returnRef2(a); // resolves to default/const ref return intent

  writeln(globalInt);
  writeln(y);
}

