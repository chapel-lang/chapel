// deprecated by Jade in 1.32, should be moved to test/types/ after deprecation period as compile failures
config param testnum: int = 1;
config type retType = sync int;

var myGlobalValue: retType;

if testnum == 1 {
  proc foo(): retType { // warn
    return myGlobalValue;
  }
  var a: retType = foo();
}
else if testnum == 2 {
   proc foo() const : retType { // warn
    return myGlobalValue;
  }
  var a: retType = foo();
}
else if testnum == 3 {
  proc foo1() const ref {
    return myGlobalValue;
  }
  const ref a1: retType = foo1();
   proc foo2() ref  {
    return myGlobalValue;
  }
  ref a2: retType = foo2();
}
