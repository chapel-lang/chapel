// This tests the deprecation warning for : in
// where clauses. After the 1.18 release, the :
// in a where clause should mean a cast.

class Parent {
}

class Child : Parent {
}

proc myfunc(x) where x.type : Parent {
  writeln("in myfunc, type is ", x.type:string);
}

proc main() {
  var x = new borrowed Child();
  var y = new borrowed Parent();

  myfunc(x);
  myfunc(y);
}
