proc f(type t) {
  proc f0() {
    {
      enum E { zero=-1, one=-2, two=-4, three=-8 };
      const s: t = "three";
      const x = s:E;
      writeln(s);
    }
  }
  f0();

  proc f1() {
    enum E { zero=-1, one=-2, two=-4, three=-8 };
    {
      const x = E.three;
      const s = x:t;
      writeln(s);
    }
  }
  f1();
}

f(string);


