class C {
  proc shortMethod return 3;
}

class D {

}

var a = new D();
writeln(a.shortMethod); // unresolved call
