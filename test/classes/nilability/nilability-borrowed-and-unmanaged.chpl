module test {
  class Parent { var p: int; }

  class Child : Parent { var c: int; }

  proc abd(arg:borrowed)  { writeln("abd ", arg.type:string); }
  proc abq(arg:borrowed class?) { writeln("abq ", arg.type:string); }
  proc abb(arg:borrowed class) { writeln("abb ", arg.type:string); }
  
  proc aud(arg:unmanaged)  { writeln("aud ", arg.type:string); }
  proc auq(arg:unmanaged class?) { writeln("auq ", arg.type:string); }
  proc aub(arg:unmanaged class) { writeln("aub ", arg.type:string); }

  proc main() {
    var c = (new owned Child(1, 2)).borrow();

    var cu = c:unmanaged;
    var cuq = c:unmanaged class?;
    var cb = c:borrowed;
    var cbq = c:borrowed class?;
    writeln();
    writeln("cu:", cu.type:string);
    writeln("cuq:", cuq.type:string);
    writeln("cb:", cb.type:string);
    writeln("cbq:", cbq.type:string);
    writeln();

    // Passing in cu
    writeln("Passing cu");
    abd(cu);
    abq(cu);
    abb(cu);
    aud(cu);
    auq(cu);
    aub(cu);

    writeln("Passing cuq");
    abd(cuq);
    abq(cuq);
    aud(cuq);
    auq(cuq);

    writeln("Passing cb");
    abd(cb);
    abq(cb);
    abb(cb);

    writeln("Passing cbq");
    abd(cbq);
    abq(cbq);
  }
}
