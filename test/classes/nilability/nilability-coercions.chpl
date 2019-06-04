module test {
  class Parent { var p: int; }

  class Child : Parent { var c: int; }

  proc acceptsParent(arg:borrowed Parent) { }
  proc acceptsParentQ(arg:borrowed Parent?) { }
  proc acceptsChild(arg:borrowed Child) { }
  proc acceptsChildQ(arg:borrowed Child?) { }

  proc main() {
    var c = (new owned Child(1, 2)).borrow();

    // Check some coercions
    var cu = c:unmanaged;
    var cuq = c:unmanaged?;
    var cb = c:borrowed;
    var cbq = c:borrowed?;
    writeln();
    writeln("cu:", cu.type:string);
    writeln("cuq:", cuq.type:string);
    writeln("cb:", cb.type:string);
    writeln("cbq:", cbq.type:string);
    writeln();

    // unmanaged -> borrowed?
    {
      writeln("cu:borrowed Child?");
      var x = cu:borrowed Child?;
      writeln(x.type:string, " ", x);
      var y:borrowed Child? = cu;
      writeln(y.type:string, " ", y);
      var z:borrowed? = cu;
      writeln(z.type:string, " ", z);
    }
    // unmanaged -> unmanaged?
    {
      writeln("cu:unmanaged Child?");
      var x = cu:unmanaged Child?;
      writeln(x.type:string, " ", x);
      var y:unmanaged Child? = cu;
      writeln(y.type:string, " ", y);
      var z:unmanaged? = cu;
      writeln(z.type:string, " ", z);
    }
    // unmanaged -> borrowed
    {
      writeln("cu:borrowed Child");
      var x = cu:borrowed Child;
      writeln(x.type:string, " ", x);
      var y:borrowed Child = cu;
      writeln(y.type:string, " ", y);
      var z:borrowed = cu;
      writeln(z.type:string, " ", z);
    }
    // method dispatch with unmanaged
    {
      acceptsParent(cu);
      acceptsParentQ(cu);
      acceptsChild(cu);
      acceptsChildQ(cu);
    }


    // unmanaged ? -> borrowed ?
    {
      writeln("cuq:borrowed Child?");
      var x = cuq:borrowed Child?;
      writeln(x.type:string, " ", x);
      var y:borrowed Child? = cuq;
      writeln(y.type:string, " ", y);
      var z:borrowed? = cuq;
      writeln(z.type:string, " ", z);
    }
    // method dispatch with unmanaged ?
    {
      acceptsParentQ(cuq);
      acceptsChildQ(cuq);
    }

    // borrowed -> borrowed ?
    {
      writeln("cb:borrowed Child?");
      var x = cb:borrowed Child?;
      writeln(x.type:string, " ", x);
      var y:borrowed Child? = cb;
      writeln(y.type:string, " ", y);
      var z:borrowed? = cb;
      writeln(z.type:string, " ", z);
    }
    // method dispatch with borrowed
    {
      acceptsParent(cb);
      acceptsParentQ(cb);
      acceptsChild(cb);
      acceptsChildQ(cb);
    }

    // method dispatch with borrowed?
    {
      acceptsParentQ(cbq);
      acceptsChildQ(cbq);
    }
  }
}


