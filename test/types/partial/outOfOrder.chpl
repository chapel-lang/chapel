
record R {
  type T;
  type U;
  param x : int;
}

record D {
  type T;
  type U;
  param p : T;
}

proc main() {
  {
    writeln("----- middle-first-last -----");
    type R_S_ = R(U=string);
    writeln(R_S_:string);
    type RIS_ = R_S_(int);
    writeln(RIS_:string);
    type RIS5 = RIS_(5);
    writeln(RIS5:string);
  }

  {
    writeln("----- middle-last-first -----");
    type R_S_ = R(U=string);
    type R_S5 = R_S_(x=5);
    writeln(R_S5:string);
    type RIS5 = R_S5(int);
    writeln(RIS5:string);
  }

  {
    writeln("----- first-last-middle -----");
    type RI_5 = R(int, x=5);
    writeln(RI_5:string);
    type RIS5 = RI_5(string);
    writeln(RIS5:string);
  }

  {
    writeln("----- first-last-dependent -----");
    type DU_5 = D(uint(8), p=55);
    writeln(DU_5:string);
  }
}
