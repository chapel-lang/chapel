module test {
  class Parent { }

  class Child : Parent { }

  proc main() {
    var cp:Parent = (new owned Child()).borrow();
    var p:Parent  = (new owned Parent()).borrow();

    try {
      writeln("cp:Child");
      var x = cp:Child;
      writeln(x.type:string, " ", x);
    } catch e {
      writeln(e);
    }

    {
      writeln("cp:Child?");
      var x = cp:Child?;
      writeln(x.type:string, " ", x);
    }

    try {
      writeln("p:Child");
      var x = p:Child;
      writeln(x.type:string, " ", x);
    } catch e {
      writeln(e);
    }

    {
      writeln("p:Child?");
      var x = p:Child?;
      writeln(x.type:string, " ", x);
    }
  }
}
