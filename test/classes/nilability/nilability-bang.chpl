module test {
  class Parent { var p:int; }

  class Child : Parent { var c:int; }

  proc main() {
    var cb:borrowed Child = (new owned Child(1, 2)).borrow();
    var cbq = cb:borrowed Child?;
    var cu = cb:unmanaged Child;
    var cuq = cb:unmanaged Child?;

    var nbq:borrowed Child? = nil;
    var nuq:unmanaged Child? = nil;

    {
      writeln("cb!");
      var x = cb!;
      writeln(x.type:string, " ", x);
    }
    {
      writeln("cbq!");
      var x = cbq!;
      writeln(x.type:string, " ", x);
    }
    {
      writeln("cu!");
      var x = cu!;
      writeln(x.type:string, " ", x);
    }
    {
      writeln("cuq!");
      var x = cuq!;
      writeln(x.type:string, " ", x);
    }
  }
}
