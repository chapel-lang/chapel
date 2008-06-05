class C {
  type t;
  var a, b: t;
}

def main {
  var c = new C(int, 1, 2);
  var o: object = c;
  writeln((o:c.type).a);
}
