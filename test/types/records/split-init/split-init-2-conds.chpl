class C { var x: int; }

config const option = true;

proc main() {
  var a;
  var b;
  if option then
    a = new C(1);
  else
    a = new C(2);
  if !option then
    b = new C(3);
  else
    b = new C(4);

  writeln(a.type:string, " ", a);
  writeln(b.type:string, " ", b);
}
