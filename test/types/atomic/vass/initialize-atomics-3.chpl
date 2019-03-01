
proc main() {
  var A : atomic int = 1;
  var B : atomic uint = 2;
  var C : atomic int(8) = 3;
  var D : atomic int = (4:int(16));

  var E : atomic real = 1.0;
  var F : atomic real(32) = 2.0;
  var G : atomic real = (3.0:real(32));

  var H : atomic bool = true;

  writeln(A.type:string);
  writeln(A.read());
  writeln(B.type:string);
  writeln(B.read());
  writeln(C.type:string);
  writeln(C.read());
  writeln(D.type:string);
  writeln(D.read());

  writeln(E.type:string);
  writeln(E.read());
  writeln(F.type:string);
  writeln(F.read());
  writeln(G.type:string);
  writeln(G.read());

  writeln(H.type:string);
  writeln(H.read());
}
