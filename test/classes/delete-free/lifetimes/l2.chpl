use OwnedObject;

class MyClass {
  var data:int;
}

record R {
  // TODO - get init with owned fields working
  var c:Owned(MyClass);// = new Owned(nil:MyClass);
  proc init() {
    //var tmp = new Owned(new MyClass(data));
    //c = tmp;
    //c.retain(new MyClass(data));
  }
  pragma "safe"
  proc get(): MyClass {
    return c.borrow();
  }
}

record MyCollection {
  var a: R;
  var b: R;
  proc init() {
  }
  pragma "safe"
  proc this(i:int): MyClass {
    if i == 1 then
      return a.get();
    else
      return b.get();
  }
}

var global:MyClass;

pragma "safe"
proc bad1() {
  var r:R;
  r.c.retain(new MyClass(1));
  global = r.get();
  // r.c deleted here
}

pragma "safe"
proc bad2() {
  var outer:MyClass;
  {
    var r:R;
    r.c.retain(new MyClass(1));
    outer = r.get();
    // r.c deleted here
  }
  writeln(outer);
}

pragma "safe"
proc bad3() {
  var group:MyCollection;
  group.a.c.retain(new MyClass(1));
  group.b.c.retain(new MyClass(2));
  global = group.this(1);
  global = group.this(2);
  // group.{a.b}.c deleted here
}


pragma "safe"
proc bad10() : MyClass {
  //var r = new R(1);
  //var tmp = new Owned(new MyClass(1));
  //r.c = tmp;
  var r:R;
  r.c.retain(new MyClass(1));
  return r.get();
  // r.c deleted here
}

pragma "safe"
proc ok1() {
  global = new MyClass(10);
  var a:MyClass = global; // OK: lifetime global > lifetime a
  a = global;
  {
    var x = a; // OK: x has shorter lifetime than a
  }
  delete global;
}

pragma "safe"
proc ok2() {
}

proc test() {
  ok1();
  bad1();
  bad2();
  bad3();

  {
    var tmp = bad10();
    writeln(tmp);
  }
}

test();
