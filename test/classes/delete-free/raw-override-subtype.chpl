class Domain {
}
class MyDomain : Domain {
  var x;
}
class Array {
  proc foo() {
    writeln("In parent foo");
    return nil: unmanaged Domain;
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
  var dom = new unmanaged MyDomain(1);
  var arr:unmanaged Array = new unmanaged MyArray(dom);
  var x = arr.foo();
}
test();
