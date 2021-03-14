class Domain {
}
class MyDomain : Domain {
  var x;
}
class Array {
  proc foo(in arg:owned Domain) {
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
    var arr = new owned MyArray(1);
    var dom = new owned MyDomain(1);
    arr.foo(dom); // empties dom
  }
  {
    var arr = new owned MyArray(1);
    var dom = new owned MyDomain(1);
    arr.bar(dom);
    baz(dom);
  }
}
test();
