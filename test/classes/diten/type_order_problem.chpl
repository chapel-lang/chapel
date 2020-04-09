var c = new C(); // workaround: move this line below "type T = real;"
type T = real;

class C {
  var t: T;
}

writeln(c);
