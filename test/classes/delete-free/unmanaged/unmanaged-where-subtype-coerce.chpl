class Class {
}

proc f(x) where isSubtype(x, Class) {
  writeln("f");
}

var x = new unmanaged Class();

f(x);
