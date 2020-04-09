
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
  writeln(a.type:string);

  const b : R(p=0);
  writeln(b.type:string);

  const c : R(p=1);
  writeln(c.type:string);


  const d : S;
  writeln(d.type:string);

  const e : S(p=0);
  writeln(e.type:string);

  const f : S(p=1);
  writeln(f.type:string);

  const g : T();
  writeln(g.type:string);

  const h : T(q="foo");
  writeln(h.type:string);

  const i : U;
  writeln(i.type:string);

  const j : U(r=10);
  writeln(j.type:string);

  writeln( Z         :string ); // Z
  writeln( Z(p=0)    :string ); // Z(0)(2)(3)
  writeln( Z(q=0)    :string ); // Z(1,0)(3)
  writeln( Z(q=2)    :string ); // Z(1)(2)(3)
  writeln( Z(r=0)    :string ); // Z(1)(2,0)
  writeln( Z(q=0,r=0):string ); // Z(1,0,0)
}
