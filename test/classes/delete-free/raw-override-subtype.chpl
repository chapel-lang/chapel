class Domain {
}
class MyDomain : Domain {
  var x;
}
class Array {
  proc foo() {
    writeln("In parent foo");
    return nil: raw Domain;
  }
}
class MyArray : Array {
  var dom;
  proc foo() {
    writeln("In child foo");
    return dom;
  }
}

proc test() {
  var dom = new raw MyDomain(1);
  var arr:raw Array = new raw MyArray(dom);
  var x = arr.foo();
}
test();
