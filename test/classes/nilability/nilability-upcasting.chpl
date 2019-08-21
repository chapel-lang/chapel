module test {
  class Parent { var p: int; }

  class Child : Parent { var c: int; }

  proc main() {
    var cb:borrowed Child = (new owned Child(1, 2)).borrow();
    var cbq = cb:borrowed Child?;
    var cu = cb:unmanaged Child;
    var cuq = cb:unmanaged Child?;
    var nbq:borrowed Child? = nil;
    var nuq:unmanaged Child? = nil;

    // casting to generic Child
    {
      writeln();
      writeln("casts to generic Child");
      // cb
      {
        writeln("cb:Child");
        var x = cb:Child;
        writeln(x.type:string, " ", x);
      }
      // cbq
      {
        writeln("cbq:Child");
        try {
          var x = cbq:Child;
          writeln(x.type:string, " ", x);
        } catch e {
          writeln(e);
          halt("fail");
        }
      }
      // cu
      {
        writeln("cu:Child");
        var x = cu:Child;
        writeln(x.type:string, " ", x);
      }
      // cuq
      {
        writeln("cuq:Child");
        try {
          var x = cuq:Child;
          writeln(x.type:string, " ", x);
        } catch e {
          writeln(e);
          halt("fail");
        }
      }
      // nbq
      {
        writeln("nbq:Child");
        try {
          var x = nbq:Child;
          writeln(x.type:string, " ", x);
          halt("fail");
        } catch e {
          writeln(e);
        }
      }
      // nuq
      {
        writeln("nuq:Child");
        try {
          var x = nuq:Child;
          writeln(x.type:string, " ", x);
          halt("fail");
        } catch e {
          writeln(e);
        }
      }
    }

    // casting to generic Child?
    {
      writeln();
      writeln("casts to generic Child?");
      // cb
      {
        writeln("cb:Child?");
        var x = cb:Child?;
        writeln(x.type:string, " ", x);
      }
      // cbq
      {
        writeln("cbq:Child?");
        var x = cbq:Child?;
        writeln(x.type:string, " ", x);
      }
      // cu
      {
        writeln("cu:Child?");
        var x = cu:Child?;
        writeln(x.type:string, " ", x);
      }
      // cuq
      {
        writeln("cuq:Child?");
        var x = cuq:Child?;
        writeln(x.type:string, " ", x);
      }
      // nbq
      {
        writeln("nbq:Child?");
        var x = nbq:Child?;
        writeln(x.type:string, " ", x);
        assert(x == nil);
      }
      // nuq
      {
        writeln("nuq:Child?");
        var x = nuq:Child?;
        writeln(x.type:string, " ", x);
        assert(x == nil);
      }
    }

    // casting to borrowed Child
    {
      writeln();
      writeln("casts to borrowed Child");
      // cb
      {
        writeln("cb:borrowed Child");
        var x = cb:borrowed Child;
        writeln(x.type:string, " ", x);
        var y = cb:borrowed;
        writeln(y.type:string, " ", y);
        var z = cb:borrowed class;
        writeln(z.type:string, " ", z);
      }
      // cbq
      {
        writeln("cbq:borrowed Child");
        try {
          var x = cbq:borrowed Child;
          writeln(x.type:string, " ", x);
        } catch e {
          writeln(e);
          halt("fail");
        }
        try {
          var y = cbq:borrowed class;
          writeln(y.type:string, " ", y);
        } catch e {
          writeln(e);
          halt("fail");
        }
      }
      // cu
      {
        writeln("cu:borrowed Child");
        var x = cu:borrowed Child;
        writeln(x.type:string, " ", x);
        var y = cu:borrowed class;
        writeln(y.type:string, " ", y);
        var z = cu:borrowed;
        writeln(z.type:string, " ", z);
      }
      // cuq
      {
        writeln("cuq:borrowed Child");
        try {
          var x = cuq:borrowed Child;
          writeln(x.type:string, " ", x);
        } catch e {
          writeln(e);
          halt("fail");
        }
        try {
          var y = cbq:borrowed class;
          writeln(y.type:string, " ", y);
        } catch e {
          writeln(e);
          halt("fail");
        }
      }
      // nbq
      {
        writeln("nbq:borrowed Child");
        try {
          var x = nbq:borrowed Child;
          writeln(x.type:string, " ", x);
          halt("fail");
        } catch e {
          writeln(e);
        }
        try {
          var y = nbq:borrowed class;
          writeln(y.type:string, " ", y);
          halt("fail");
        } catch e {
          writeln(e);
        }
      }
      // nuq
      {
        writeln("nuq:borrowed Child");
        try {
          var x = nuq:borrowed Child;
          writeln(x.type:string, " ", x);
          halt("fail");
        } catch e {
          writeln(e);
        }
        try {
          var y = nuq:borrowed class;
          writeln(y.type:string, " ", y);
          halt("fail");
        } catch e {
          writeln(e);
        }
      }
    }

    // casting to borrowed Child?
    {
      writeln();
      writeln("casts to borrowed Child?");
      // cb
      {
        writeln("cb:borrowed Child?");
        var x = cb:borrowed Child?;
        writeln(x.type:string, " ", x);
        var y = cb:borrowed class?;
        writeln(y.type:string, " ", y);
      }
      // cbq
      {
        writeln("cbq:borrowed Child?");
        var x = cbq:borrowed Child?;
        writeln(x.type:string, " ", x);
        var y = cbq:borrowed class?;
        writeln(y.type:string, " ", y);
        var z = cbq:borrowed;
        writeln(z.type:string, " ", z);
      }
      // cu
      {
        writeln("cu:borrowed Child?");
        var x = cu:borrowed Child?;
        writeln(x.type:string, " ", x);
        var y = cu:borrowed class?;
        writeln(y.type:string, " ", y);
      }
      // cuq
      {
        writeln("cuq:borrowed Child?");
        var x = cuq:borrowed Child?;
        writeln(x.type:string, " ", x);
        var y = cbq:borrowed class?;
        writeln(y.type:string, " ", y);
        var z = cbq:borrowed;
        writeln(z.type:string, " ", z);
      }
      // nbq
      {
        writeln("nbq:borrowed Child?");
        var x = nbq:borrowed Child?;
        writeln(x.type:string, " ", x);
        assert(x == nil);
        var y = nbq:borrowed class?;
        writeln(y.type:string, " ", y);
        assert(y == nil);
        var z = nbq:borrowed;
        writeln(z.type:string, " ", z);
        assert(z == nil);
      }
      // nuq
      {
        writeln("nuq:borrowed Child?");
        var x = nuq:borrowed Child?;
        writeln(x.type:string, " ", x);
        assert(x == nil);
        var y = nuq:borrowed class?;
        writeln(y.type:string, " ", y);
        assert(y == nil);
        var z = nuq:borrowed;
        writeln(z.type:string, " ", z);
        assert(z == nil);
      }
    }

    // casting to unmanaged Child
    {
      writeln();
      writeln("casts to unmanaged Child");
      // cb
      {
        writeln("cb:unmanaged Child");
        var x = cb:unmanaged Child;
        writeln(x.type:string, " ", x);
        var y = cb:unmanaged class;
        writeln(y.type:string, " ", y);
        var z = cb:unmanaged;
        writeln(z.type:string, " ", z);
      }
      // cbq
      {
        writeln("cbq:unmanaged Child");
        try {
          var x = cbq:unmanaged Child;
          writeln(x.type:string, " ", x);
        } catch e {
          writeln(e);
          halt("fail");
        }
        try {
          var y = cbq:unmanaged class;
          writeln(y.type:string, " ", y);
        } catch e {
          writeln(e);
          halt("fail");
        }
      }
      // cu
      {
        writeln("cu:unmanaged Child");
        var x = cu:unmanaged Child;
        writeln(x.type:string, " ", x);
        var y = cu:unmanaged class;
        writeln(y.type:string, " ", y);
        var z = cu:unmanaged;
        writeln(z.type:string, " ", z);
      }
      // cuq
      {
        writeln("cuq:unmanaged Child");
        try {
          var x = cuq:unmanaged Child;
          writeln(x.type:string, " ", x);
        } catch e {
          writeln(e);
          halt("fail");
        }
        try {
          var y = cbq:unmanaged class;
          writeln(y.type:string, " ", y);
        } catch e {
          writeln(e);
          halt("fail");
        }
      }
      // nbq
      {
        writeln("nbq:unmanaged Child");
        try {
          var x = nbq:unmanaged Child;
          writeln(x.type:string, " ", x);
          halt("fail");
        } catch e {
          writeln(e);
        }
        try {
          var y = nbq:unmanaged class;
          writeln(y.type:string, " ", y);
          halt("fail");
        } catch e {
          writeln(e);
        }
      }
      // nuq
      {
        writeln("nuq:unmanaged Child");
        try {
          var x = nuq:unmanaged Child;
          writeln(x.type:string, " ", x);
          halt("fail");
        } catch e {
          writeln(e);
        }
        try {
          var y = nuq:unmanaged class;
          writeln(y.type:string, " ", y);
          halt("fail");
        } catch e {
          writeln(e);
        }
      }
    }

    // casting to unmanaged Child?
    {
      writeln();
      writeln("casts to unmanaged Child?");
      // cb
      {
        writeln("cb:unmanaged Child?");
        var x = cb:unmanaged Child?;
        writeln(x.type:string, " ", x);
        var y = cb:unmanaged class?;
        writeln(y.type:string, " ", y);
      }
      // cbq
      {
        writeln("cbq:unmanaged Child?");
        var x = cbq:unmanaged Child?;
        writeln(x.type:string, " ", x);
        var y = cbq:unmanaged class?;
        writeln(y.type:string, " ", y);
        var z = cbq:unmanaged;
        writeln(z.type:string, " ", z);
      }
      // cu
      {
        writeln("cu:unmanaged Child?");
        var x = cu:unmanaged Child?;
        writeln(x.type:string, " ", x);
        var y = cu:unmanaged class?;
        writeln(y.type:string, " ", y);
      }
      // cuq
      {
        writeln("cuq:unmanaged Child?");
        var x = cuq:unmanaged Child?;
        writeln(x.type:string, " ", x);
        var y = cbq:unmanaged class?;
        writeln(y.type:string, " ", y);
        var z = cbq:unmanaged;
        writeln(z.type:string, " ", z);
      }
      // nbq
      {
        writeln("nbq:unmanaged Child?");
        var x = nbq:unmanaged Child?;
        writeln(x.type:string, " ", x);
        assert(x == nil);
        var y = nbq:unmanaged class?;
        writeln(y.type:string, " ", y);
        assert(y == nil);
        var z = nbq:unmanaged;
        writeln(z.type:string, " ", z);
        assert(z == nil);
      }
      // nuq
      {
        writeln("nuq:unmanaged Child?");
        var x = nuq:unmanaged Child?;
        writeln(x.type:string, " ", x);
        assert(x == nil);
        var y = nuq:unmanaged class?;
        writeln(y.type:string, " ", y);
        assert(y == nil);
        var z = nuq:unmanaged;
        writeln(z.type:string, " ", z);
        assert(z == nil);
      }
    }

    // casting to generic Parent
    {
      writeln();
      writeln("casts to generic Parent");
      // cb
      {
        writeln("cb:Parent");
        var x = cb:Parent;
        writeln(x.type:string, " ", x);
      }
      // cbq
      {
        writeln("cbq:Parent");
        try {
          var x = cbq:Parent;
          writeln(x.type:string, " ", x);
        } catch e {
          writeln(e);
          halt("fail");
        }
      }
      // cu
      {
        writeln("cu:Parent");
        var x = cu:Parent;
        writeln(x.type:string, " ", x);
      }
      // cuq
      {
        writeln("cuq:Parent");
        try {
          var x = cuq:Parent;
          writeln(x.type:string, " ", x);
        } catch e {
          writeln(e);
          halt("fail");
        }
      }
      // nbq
      {
        writeln("nbq:Parent");
        try {
          var x = nbq:Parent;
          writeln(x.type:string, " ", x);
          halt("fail");
        } catch e {
          writeln(e);
        }
      }
      // nuq
      {
        writeln("nuq:Parent");
        try {
          var x = nuq:Parent;
          writeln(x.type:string, " ", x);
          halt("fail");
        } catch e {
          writeln(e);
        }
      }
    }

    // casting to generic Parent?
    {
      writeln();
      writeln("casts to generic Parent?");
      // cb
      {
        writeln("cb:Parent?");
        var x = cb:Parent?;
        writeln(x.type:string, " ", x);
      }
      // cbq
      {
        writeln("cbq:Parent?");
        var x = cbq:Parent?;
        writeln(x.type:string, " ", x);
      }
      // cu
      {
        writeln("cu:Parent?");
        var x = cu:Parent?;
        writeln(x.type:string, " ", x);
      }
      // cuq
      {
        writeln("cuq:Parent?");
        var x = cuq:Parent?;
        writeln(x.type:string, " ", x);
      }
      // nbq
      {
        writeln("nbq:Parent?");
        var x = nbq:Parent?;
        writeln(x.type:string, " ", x);
        assert(x == nil);
      }
      // nuq
      {
        writeln("nuq:Parent?");
        var x = nuq:Parent?;
        writeln(x.type:string, " ", x);
        assert(x == nil);
      }
    }

    // casting to borrowed Parent
    {
      writeln();
      writeln("casts to borrowed Parent");
      // cb
      {
        writeln("cb:borrowed Parent");
        var x = cb:borrowed Parent;
        writeln(x.type:string, " ", x);
      }
      // cbq
      {
        writeln("cbq:borrowed Parent");
        try {
          var x = cbq:borrowed Parent;
          writeln(x.type:string, " ", x);
        } catch e {
          writeln(e);
          halt("fail");
        }
      }
      // cu
      {
        writeln("cu:borrowed Parent");
        var x = cu:borrowed Parent;
        writeln(x.type:string, " ", x);
      }
      // cuq
      {
        writeln("cuq:borrowed Parent");
        try {
          var x = cuq:borrowed Parent;
          writeln(x.type:string, " ", x);
        } catch e {
          writeln(e);
          halt("fail");
        }
      }
      // nbq
      {
        writeln("nbq:borrowed Parent");
        try {
          var x = nbq:borrowed Parent;
          writeln(x.type:string, " ", x);
          halt("fail");
        } catch e {
          writeln(e);
        }
      }
      // nuq
      {
        writeln("nuq:borrowed Parent");
        try {
          var x = nuq:borrowed Parent;
          writeln(x.type:string, " ", x);
          halt("fail");
        } catch e {
          writeln(e);
        }
      }
    }

    // casting to borrowed Parent?
    {
      writeln();
      writeln("casts to borrowed Parent?");
      // cb
      {
        writeln("cb:borrowed Parent?");
        var x = cb:borrowed Parent?;
        writeln(x.type:string, " ", x);
      }
      // cbq
      {
        writeln("cbq:borrowed Parent?");
        var x = cbq:borrowed Parent?;
        writeln(x.type:string, " ", x);
      }
      // cu
      {
        writeln("cu:borrowed Parent?");
        var x = cu:borrowed Parent?;
        writeln(x.type:string, " ", x);
      }
      // cuq
      {
        writeln("cuq:borrowed Parent?");
        var x = cuq:borrowed Parent?;
        writeln(x.type:string, " ", x);
      }
      // nbq
      {
        writeln("nbq:borrowed Parent?");
        var x = nbq:borrowed Parent?;
        writeln(x.type:string, " ", x);
        assert(x == nil);
      }
      // nuq
      {
        writeln("nuq:borrowed Parent?");
        var x = nuq:borrowed Parent?;
        writeln(x.type:string, " ", x);
        assert(x == nil);
      }
    }

    // casting to unmanaged Parent
    {
      writeln();
      writeln("casts to unmanaged Parent");
      // cb
      {
        writeln("cb:unmanaged Parent");
        var x = cb:unmanaged Parent;
        writeln(x.type:string, " ", x);
      }
      // cbq
      {
        writeln("cbq:unmanaged Parent");
        try {
          var x = cbq:unmanaged Parent;
          writeln(x.type:string, " ", x);
        } catch e {
          writeln(e);
          halt("fail");
        }
      }
      // cu
      {
        writeln("cu:unmanaged Parent");
        var x = cu:unmanaged Parent;
        writeln(x.type:string, " ", x);
      }
      // cuq
      {
        writeln("cuq:unmanaged Parent");
        try {
          var x = cuq:unmanaged Parent;
          writeln(x.type:string, " ", x);
        } catch e {
          writeln(e);
          halt("fail");
        }
      }
      // nbq
      {
        writeln("nbq:unmanaged Parent");
        try {
          var x = nbq:unmanaged Parent;
          writeln(x.type:string, " ", x);
          halt("fail");
        } catch e {
          writeln(e);
        }
      }
      // nuq
      {
        writeln("nuq:unmanaged Parent");
        try {
          var x = nuq:unmanaged Parent;
          writeln(x.type:string, " ", x);
          halt("fail");
        } catch e {
          writeln(e);
        }
      }
    }

    // casting to unmanaged Parent?
    {
      writeln();
      writeln("casts to unmanaged Parent?");
      // cb
      {
        writeln("cb:unmanaged Parent?");
        var x = cb:unmanaged Parent?;
        writeln(x.type:string, " ", x);
      }
      // cbq
      {
        writeln("cbq:unmanaged Parent?");
        var x = cbq:unmanaged Parent?;
        writeln(x.type:string, " ", x);
      }
      // cu
      {
        writeln("cu:unmanaged Parent?");
        var x = cu:unmanaged Parent?;
        writeln(x.type:string, " ", x);
      }
      // cuq
      {
        writeln("cuq:unmanaged Parent?");
        var x = cuq:unmanaged Parent?;
        writeln(x.type:string, " ", x);
      }
      // nbq
      {
        writeln("nbq:unmanaged Parent?");
        var x = nbq:unmanaged Parent?;
        writeln(x.type:string, " ", x);
        assert(x == nil);
      }
      // nuq
      {
        writeln("nuq:unmanaged Parent?");
        var x = nuq:unmanaged Child?;
        writeln(x.type:string, " ", x);
        assert(x == nil);
      }
    }
  }
}
