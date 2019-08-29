module test {
  class Parent { var p: int; }

  class Child : Parent { var c: int; }

  proc acceptsParent(arg:borrowed Parent) { }
  proc acceptsParentQ(arg:borrowed Parent?) { }
  proc acceptsChild(arg:borrowed Child) { }
  proc acceptsChildQ(arg:borrowed Child?) { }

  config param useowned = true;

  proc makeNil() {
    if useowned {
      var ret: owned Child?;
      return ret;
    } else {
      var ret: shared Child?;
      return ret;
    }
  }
  proc main() {
    var c = if useowned then new owned Child(1, 2)
                        else new shared Child(1, 2);

    var cq = makeNil();

    writeln();
    //writeln("c:", c.type:string);
    writeln("c.borrow():", c.borrow().type:string);
    //writeln("cq:", cq.type:string);
    writeln("cq.borrow():", cq.borrow().type:string);
    writeln();

    // owned Child -> borrowed Child?
    {
      writeln("c:borrowed Child?");
      var x = c:borrowed Child?;
      writeln(x.type:string, " ", x);
      var y:borrowed Child? = c;
      writeln(y.type:string, " ", y);
      var z:borrowed class? = c;
      writeln(z.type:string, " ", z);
    }
    // owned Child -> borrowed Child
    {
      writeln("c:borrowed Child");
      var x = c:borrowed Child;
      writeln(x.type:string, " ", x);
      var y:borrowed Child = c;
      writeln(y.type:string, " ", y);
      var z:borrowed = c;
      writeln(z.type:string, " ", z);
    }
    // method dispatch with owned Child
    {
      acceptsChild(c);
      acceptsChildQ(c);
      acceptsParent(c);
      acceptsParentQ(c);
    }

    // owned Child? -> borrowed Child?
    {
      writeln("cq:borrowed Child?");
      var x = cq:borrowed Child?;
      writeln(x.type:string, " ", x);
      var y:borrowed Child? = cq;
      writeln(y.type:string, " ", y);
      var z:borrowed class? = cq;
      writeln(z.type:string, " ", z);
    }
    // method dispatch with owned Child?
    {
      acceptsChildQ(cq);
      acceptsParentQ(cq);
    }
  }
}


