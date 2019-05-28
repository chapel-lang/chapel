module test {
  class Parent { var p: int; }

  class Child : Parent { var c: int; }

  proc abd(arg:borrowed)  { writeln("abd ", arg.type:string); }
  proc abq(arg:borrowed?) { writeln("abq ", arg.type:string); }
  proc abb(arg:borrowed!) { writeln("abb ", arg.type:string); }
  
  proc aod(arg:owned)  { writeln("aod ", arg.type:string); }
  proc aoq(arg:owned?) { writeln("aoq ", arg.type:string); }
  proc aob(arg:owned!) { writeln("aob ", arg.type:string); }

  proc main() {
    var co = new owned Child(1, 2);
    var coq:owned Child? = new owned Child(1, 2);

    writeln("Passing co");
    abd(co);
    abq(co);
    abb(co);
    aod(co);
    aoq(co);
    aob(co);

    writeln("Passing coq");
    abd(coq);
    abq(coq);
    aod(coq);
    aoq(coq);
  }
}
