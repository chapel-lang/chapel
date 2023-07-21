use IO;

proc test() {
  var s: string;

  var A:[1..2] int = 1..2;

  for a in A {
    try! {
      s += "num:%i\n".format(a);
    }
  }

  writeln(s);
}

test();
