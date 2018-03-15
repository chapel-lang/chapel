pragma "safe"
module borrowescapes {

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
  proc get(): MyClass {
    return c.borrow();
  }
}

record MyCollection {
  var a: R;
  var b: R;
  proc init() {
  }
  proc this(i:int): MyClass {
    if i == 1 then
      return a.get();
    else
      return b.get();
  }
  iter these(): MyClass {
    yield a.get();
    yield b.get();
  }
  proc returnsNil() {
    return nil:MyClass;
  }
}

var global:MyClass;

proc bad1() {
  var r:R;
  r.c.retain(new MyClass(1));
  global = r.get();
  // r.c deleted here
}

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

proc bad3() {
  var group:MyCollection;
  group.a.c.retain(new MyClass(1));
  group.b.c.retain(new MyClass(2));
  global = group.this(1);
  global = group.this(2);
  // group.{a.b}.c deleted here
}


proc bad10() : MyClass {
  //var r = new R(1);
  //var tmp = new Owned(new MyClass(1));
  //r.c = tmp;
  var r:R;
  r.c.retain(new MyClass(1));
  return r.get();
  // r.c deleted here
}

proc bad21() {
  var outer:MyClass = nil;
  {
    var r:R;
    r.c.retain(new MyClass(1));
    outer = r.get();
    // r.c deleted here
  }
  writeln(outer);
}

proc bad22() {
  var outer:MyClass = new MyClass(1);
  {
    var r:R;
    r.c.retain(new MyClass(1));
    delete outer;
    outer = r.get();
    // r.c deleted here
  }
  writeln(outer);
}

proc bad23() {
  var outer:MyClass;
  {
    var r:R;
    r.c.retain(new MyClass(1));
    outer = r.get();
    // r.c deleted here
  }
  writeln(outer);
  outer = new MyClass(1);
  delete outer;
}


proc ok1() {
  global = new MyClass(10);
  var a:MyClass = global; // OK: lifetime global > lifetime a
  a = global;
  {
    var x = a; // OK: x has shorter lifetime than a
  }
  delete global;
}

proc ok2() {
  var group:MyCollection;
  group.a.c.retain(new MyClass(1));
  group.b.c.retain(new MyClass(2));
  var x = group.returnsNil();
}

proc ok3() {
  var x:MyClass = nil;

  var r:R;
  r.c.retain(new MyClass(1));
 
  x = r.get();
}

proc ok4() {
  var group:MyCollection;
  group.a.c.retain(new MyClass(1));
  group.b.c.retain(new MyClass(2));

  var first:MyClass = nil;

  for i in 1..2 {
    var cur = group[i];
    if first == nil then
      first = cur;
  }
}

proc ok5() {
  var group:MyCollection;
  group.a.c.retain(new MyClass(1));
  group.b.c.retain(new MyClass(2));

  var first:MyClass = nil;

  for i in group {
    if first == nil then
      first = i;
  }
}

proc test() {
  bad1();
  bad2();
  bad3();

  {
    var tmp = bad10();
    writeln(tmp);
  }

  bad21();
  bad22();
  bad23();

  ok1();
  ok2();
  ok3();
  ok4();
  ok5();
}

test();

}
