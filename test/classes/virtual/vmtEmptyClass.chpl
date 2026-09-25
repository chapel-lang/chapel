// Exercises classes with empty per-class vtables (vmt == NULL rows):
//  - a leaf that adds no virtual methods
//  - a middle class with no virtual methods whose child introduces some
// plus downcasts across the tree and an Error subclass printing its
// class name via "class name by id".

class Base {
  proc who() { writeln("Base.who"); }
}

// leaf adding nothing
class Leaf : Base { }

// middle class with no virtual methods of its own (Base has one, so this
// class only inherits; Quiet's own new methods are non-virtual until
// overridden)
class Quiet : Base { }

class Loud : Quiet {
  override proc who() { writeln("Loud.who"); }
  proc shout() { writeln("Loud.shout"); }
}

class Louder : Loud {
  override proc shout() { writeln("Louder.shout"); }
}

// a hierarchy rooted in a class with no methods at all
class NoMethods { var x: int; }
class NoMethodsChild : NoMethods { }
class NoMethodsGrandchild : NoMethodsChild {
  proc hello() { writeln("NoMethodsGrandchild.hello"); }
}
class NoMethodsGreat : NoMethodsGrandchild {
  override proc hello() { writeln("NoMethodsGreat.hello"); }
}

class MyError : Error {
  override proc message() { return "my message"; }
}
class MyDeeperError : MyError {
  override proc message() { return "my deeper message"; }
}

proc tryCasts(b: borrowed Base) {
  write("  Leaf? ", (b : borrowed Leaf?) != nil);
  write(" Quiet? ", (b : borrowed Quiet?) != nil);
  write(" Loud? ", (b : borrowed Loud?) != nil);
  writeln(" Louder? ", (b : borrowed Louder?) != nil);
}

var objs: [0..4] owned Base?;
objs[0] = new Base();
objs[1] = new Leaf();
objs[2] = new Quiet();
objs[3] = new Loud();
objs[4] = new Louder();

for (o, i) in zip(objs, 0..) {
  writeln("object ", i);
  var b = o!.borrow();
  b.who();
  tryCasts(b);
  if var l = b : borrowed Loud? then l.shout();
}

var nm: [0..3] owned NoMethods?;
nm[0] = new NoMethods();
nm[1] = new NoMethodsChild();
nm[2] = new NoMethodsGrandchild();
nm[3] = new NoMethodsGreat();

for (o, i) in zip(nm, 0..) {
  writeln("nomethods object ", i);
  var b = o!.borrow();
  writeln("  Child? ", (b : borrowed NoMethodsChild?) != nil,
          " Grandchild? ", (b : borrowed NoMethodsGrandchild?) != nil,
          " Great? ", (b : borrowed NoMethodsGreat?) != nil);
  if var g = b : borrowed NoMethodsGrandchild? then g.hello();
}

proc throwIt(which: int) throws {
  select which {
    when 0 do throw new Error("plain");
    when 1 do throw new MyError();
    otherwise do throw new MyDeeperError();
  }
}

for which in 0..2 {
  try {
    throwIt(which);
  } catch e {
    writeln("caught ", e);
  }
}
