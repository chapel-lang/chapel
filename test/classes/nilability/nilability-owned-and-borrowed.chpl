module test {
  class Parent { var p: int; }

  class Child : Parent { var c: int; }

  proc abd(arg:borrowed)  { writeln("abd ", arg.type:string); }
  proc abq(arg:borrowed class?) { writeln("abq ", arg.type:string); }
  proc abb(arg:borrowed class) { writeln("abb ", arg.type:string); }

  proc aod(arg:owned)  { writeln("aod ", arg.type:string); }
  proc aoq(arg:owned class?) { writeln("aoq ", arg.type:string); }
  proc aob(arg:owned class) { writeln("aob ", arg.type:string); }

  proc main() {
    var co = new owned Child(1, 2);
    var coq:owned Child? = new owned Child(1, 2);

    writeln("Passing co");
    abd(co);
    //abq(co); not supported -- coercion + generic
    abb(co);

    aod(co);
    co = new owned Child(1, 2); // b/c of ownership transfer
    //aoq(co); not supported - coercion + generic
    //co = new owned Child(1, 2); // b/c of ownership transfer
    aob(co);
    co = new owned Child(1, 2); // b/c of ownership transfer

    writeln("Passing coq");
    abd(coq);
    abq(coq);
    aod(coq);
    coq = new owned Child(1, 2); // b/c of ownership transfer
    aoq(coq);
    coq = new owned Child(1, 2); // b/c of ownership transfer
  }
}
