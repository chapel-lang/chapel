module M1 {
          config const aaa = 1;
  private config const bbb = 1;
  private config const ccc = 1;
  private config const ddd = 1;
  private config const eee = 1;
          config const fff = 1;
  private config const ggg = 1;

  writeln("M1: ", (aaa, bbb, ccc, ddd, eee, fff, ggg));
}

module M2 {
  private config const aaa = 2;
          config const bbb = 2;
  private config const ccc = 2;
  private config const ddd = 2;
  private config const eee = 2;
  private config const fff = 2;
          config const ggg = 2;

  writeln("M2: ", (aaa, bbb, ccc, ddd, eee, fff, ggg));
}

module M3 {
  private config const aaa = 3;
  private config const bbb = 3;
          config const ccc = 3;
  private config const ddd = 3;
  private config const eee = 3;
          config const fff = 3;
  private config const ggg = 3;

  writeln("M3: ", (aaa, bbb, ccc, ddd, eee, fff, ggg));
}

module M4 {
  private config const aaa = 4;
  private config const bbb = 4;
  private config const ccc = 4;
          config const ddd = 4;
  private config const eee = 4;
  private config const fff = 4;
          config const ggg = 4;

  writeln("M4: ", (aaa, bbb, ccc, ddd, eee, fff, ggg));

  proc main() {
    use M1, M2, M3;
    writeln("In main()");
  }
}
