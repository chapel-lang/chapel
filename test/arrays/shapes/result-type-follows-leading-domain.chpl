use BlockDist;

var D = { 1..2, 1..3 };
var BD = D dmapped Block(D);

{
  var B: [D] int = 1;
  var C: [BD] int;
  var A = B + C;

  compilerWarning("A.domain #1 : ", A.domain._value.type:string);
  writeln(A.domain);
  writeln(A);
  writeln();
}

{
  var B: [BD] int = 2;
  var C: [D] int;
  var A = B + C;

  compilerWarning("A.domain #2 : ", A.domain._value.type:string);
  writeln(A.domain);
  writeln(A);
  writeln();
}

{
  var F = [d in D] d(0)*100 + d(1);

  compilerWarning("F.domain #1 : ", F.domain._value.type:string);
  writeln(F.domain);
  writeln(F);
  writeln();

  var G = [f in F] f * 0.1;
  compilerWarning("G.domain #1 : ", G.domain._value.type:string);
  writeln(G.domain);
  writeln(G);
  writeln();
}

{
  var F = [d in BD] d(0)*200 + d(1);

  compilerWarning("F.domain #2 : ", F.domain._value.type:string);
  writeln(F.domain);
  writeln(F);
  writeln();

  var G = [f in F] f * 0.1;
  compilerWarning("G.domain #2 : ", G.domain._value.type:string);
  writeln(G.domain);
  writeln(G);
  writeln();
}
