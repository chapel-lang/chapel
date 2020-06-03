pragma "safe"
module borrowescapes {



class MyClass {
  var data:int;
}

record R {

  var c = new owned MyClass(234);
  proc init() {
    // A whole lot of nothing.
    // To preserve the line numbers
    // in the .good file for this test
  }
  proc get(): borrowed MyClass {
    return c.borrow();
  }
}

record MyCollection {
  var a: R;
  var b: R;
  proc init() {
  }
  proc this(i:int): borrowed MyClass {
    if i == 1 then
      return a.get();
    else
      return b.get();
  }
  iter these(): borrowed MyClass {
    yield a.get();
    yield b.get();
  }
  proc returnsNil() {
    return nil:borrowed MyClass?;
  }
}

var global:borrowed MyClass?;

proc bad1() {
  var r:R;
  r.c.retain(new unmanaged MyClass(1));
  global = r.get();
  // r.c deleted here
}

proc bad2() {
  var outer:borrowed MyClass?;
  {
    var r:R;
    r.c.retain(new unmanaged MyClass(1));
    outer = r.get();
    // r.c deleted here
  }
  writeln(outer);
}

proc bad3() {
  var group:MyCollection;
  group.a.c.retain(new unmanaged MyClass(1));
  group.b.c.retain(new unmanaged MyClass(2));
  global = group.this(1);
  global = group.this(2);
  // group.{a.b}.c deleted here
}


proc bad10() : borrowed MyClass {
  //var r = new R(1);
  //var tmp = new Owned(new MyClass(1));
  //r.c = tmp;
  var r:R;
  r.c.retain(new unmanaged MyClass(1));
  return r.get();
  // r.c deleted here
}

proc bad21() {
  var outer:borrowed MyClass? = nil;
  {
    var r:R;
    r.c.retain(new unmanaged MyClass(1));
    outer = r.get();
    // r.c deleted here
  }
  writeln(outer);
}

proc bad22() {
  var outer:borrowed MyClass = new borrowed MyClass(1);
  {
    var r:R;
    r.c.retain(new unmanaged MyClass(1));

    outer = r.get();
    // r.c deleted here
  }
  writeln(outer);
}

proc bad23() {
  var outer:borrowed MyClass?;
  {
    var r:R;
    r.c.retain(new unmanaged MyClass(1));
    outer = r.get();
    // r.c deleted here
  }
  writeln(outer);
  outer = new borrowed MyClass(1);
  writeln(outer);
}


proc ok1() {
  var unm = new unmanaged MyClass(10);
  global = unm;
  var a:borrowed MyClass? = global; // OK: lifetime global > lifetime a
  a = global;
  {
    var x = a; // OK: x has shorter lifetime than a
  }
  delete unm;
}

proc ok2() {
  var group:MyCollection;
  group.a.c.retain(new unmanaged MyClass(1));
  group.b.c.retain(new unmanaged MyClass(2));
  var x = group.returnsNil();
}

proc ok3() {
  var x:borrowed MyClass? = nil;

  var r:R;
  r.c.retain(new unmanaged MyClass(1));
 
  x = r.get();
}

proc ok4() {
  var group:MyCollection;
  group.a.c.retain(new unmanaged MyClass(1));
  group.b.c.retain(new unmanaged MyClass(2));

  var first:borrowed MyClass? = nil;

  for i in 1..2 {
    var cur = group[i];
    if first == nil then
      first = cur;
  }
}

proc ok5() {
  var group:MyCollection;
  group.a.c.retain(new unmanaged MyClass(1));
  group.b.c.retain(new unmanaged MyClass(2));

  var first:borrowed MyClass? = nil;

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
