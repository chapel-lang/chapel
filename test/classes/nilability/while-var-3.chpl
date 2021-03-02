// Test various successful scenarios of iterating over a linked list (shared)

// a concrete class
class clist {
  var elm: int;
  var next: shared clist?;
}

var globalCL = new shared clist?(30,
                 new shared clist(31, new shared clist(32)));

// a generic class
class glist {
  var elm;
  var next: shared glist(elm.type)?;
}

proc showU1(name: string, list) {
  write("showU1 ", name, ":");
  pragma "unsafe" // one workaround for lifetime of 'cur'
  var curr = list: borrowed class?;
  while const cur = curr {
    write(" ", cur.elm);
    curr = cur.next;
  }
  writeln();
}

proc showU2(name: string, list) {
  write("showU2 ", name, ":");
  var curr = list: unmanaged class?;  // another workaround
  while const cur = curr {
    write(" ", cur.elm);
    curr = cur.next: unmanaged class?;
  }
  writeln();
}

proc consume(name: string, in list) {
  write("consume ", name, ":");
  var curr = list: class?;
  while const cur = curr {
    write(" ", cur.elm);
    curr = cur.next;
  }
  writeln();
}

proc main {
  var cl = new shared clist(100,
             new shared clist(101, new shared clist(102)));
  var c0: shared clist? = nil;

  var gl = new shared glist("hi",
             new shared glist("there", new shared glist("world")));
  var g0 = nil: shared glist(locale)?;

  showU1("cl", cl);
  showU1("c0", c0);
  showU1("gl", gl);
  showU1("g0", g0);

  showU2("cl", cl);
  showU2("c0", c0);
  showU2("gl", gl);
  showU2("g0", g0);

  writeln("cl = ", cl);
  writeln("c0 = ", c0);
  writeln("gl = ", gl);
  writeln("g0 = ", g0);

  consume("cl", cl);
  consume("c0", c0);
  consume("gl", gl);
  consume("g0", g0);

  consume("globalCL", globalCL);
  writeln(globalCL);
}
