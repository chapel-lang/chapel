class Class {
}

proc f(x) where isSubtype(x.type, Class) {
  writeln("f");
}

var x = new unmanaged Class();

f(x);

delete x;
