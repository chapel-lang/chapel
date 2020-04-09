class Domain {
}
class MyDomain : Domain {
  var x;
}
class Array {
  proc foo(in arg:shared Domain) {
    writeln("In parent foo");
  }
  proc bar(arg:borrowed Domain) {
    writeln("In parent bar");
  }
}
class MyArray : Array {
  var x;
}

proc baz(arg:borrowed Domain) {
  writeln("In bar arg:Domain");
}

proc test() {
  {
    var arr = new shared MyArray(1);
    var dom = new shared MyDomain(1);
    arr.foo(dom);
  }
  {
    var arr = new shared MyArray(1);
    var dom = new shared MyDomain(1);
    arr.bar(dom);
    baz(dom);
  }
}
test();
