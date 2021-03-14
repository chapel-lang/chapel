proc main() {
  //these worked before adding otherwise do to the parser
  writeln(whenIntAdd(3));
  writeln(whenIntAdd("Cats are cool"));

  whenStringWrite(3);
  whenStringWrite("Cats are cool");

  //these didn't work before adding otherwise do to the parser
  writeln(whenIntAddDo(3));
  writeln(whenIntAddDo("Cats are cool"));

  whenStringWriteDo(3);
  whenStringWriteDo("Cats are cool");
}

//test otherwise with curly braces and without a do
proc whenIntAdd(x) {
  select x.type {
    when int {
      return (x + 1);
    }
    otherwise {
      write("Not an int ");
      return 0;
    }
  }
}

//test otherwise with curly braces and with a do
proc whenIntAddDo(x) {
  select x.type {
    when int do {
      return (x + 1);
    }
    otherwise do {
      write("Not an int ");
      return 0;
    }
  }
}

//test otherwise in one line without a do
proc whenStringWrite(x) {
  select x.type {
    when string do //when needs a do for oneline things
      writeln(x);
    otherwise
      writeln("not a string");
  }
}

//test otherwise in a single line with a do
proc whenStringWriteDo(x) {
  select x.type {
    when string do
      writeln(x);
    otherwise do
      writeln("not a string");
  }
}
