
record R {
  param q = "default";
  param p : int = 0;
}

record S {
  param q = "default";
  param p : int = 1;
}

record T {
  param p : int = 1;
  param q = "default";
}

record U {
  param p : int = 1;
  param q : int = 2;
  param r : int = 3;
  param s : int = 4;
}

record Z {
  param p : int = 1;
  param q : int = 2;
  param r : int = 3;
}

proc main() {
  const a : R;
  writeln(typeToString(a.type));

  const b : R(p=0);
  writeln(typeToString(b.type));

  const c : R(p=1);
  writeln(typeToString(c.type));


  const d : S;
  writeln(typeToString(d.type));

  const e : S(p=0);
  writeln(typeToString(e.type));

  const f : S(p=1);
  writeln(typeToString(f.type));

  const g : T();
  writeln(typeToString(g.type));

  const h : T(q="foo");
  writeln(typeToString(h.type));

  const i : U;
  writeln(typeToString(i.type));

  const j : U(r=10);
  writeln(typeToString(j.type));

  writeln(typeToString( Z          )); // Z(1)(2)(3)  
  writeln(typeToString( Z(p=0)     )); // Z(0)(2)(3) 
  writeln(typeToString( Z(q=0)     )); // Z(1,0)(3)  
  writeln(typeToString( Z(q=2)     )); // Z(1)(2)(3)  
  writeln(typeToString( Z(r=0)     )); // Z(1)(2,0)  
  writeln(typeToString( Z(q=0,r=0) )); // Z(1,0,0)   
}
