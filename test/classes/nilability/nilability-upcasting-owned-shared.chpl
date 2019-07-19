module test {
  class Parent { var p: int; }

  class Child : Parent { var c: int; }

  proc main() {
    var co:owned Child = new owned Child(1,2);
    var coq:owned Child? = new owned Child(1,2);
    var cs:shared Child = new shared Child(1,2);
    var csq:shared Child? = new shared Child(1,2);

    var noq:owned Child? = nil;
    var nsq:shared Child? = nil;

    // casting to generic Child
    {
      writeln();
      writeln("casts to generic Child");
      // co
      {
        writeln("co:Child");
        var x = co:Child;
        writeln(x.type:string, " ", x);
        co = new owned Child(1,2); // because of ownership transfer
      }
      // coq
      {
        writeln("coq:Child");
        try {
          var x = coq:Child;
          writeln(x.type:string, " ", x);
          coq = new owned Child(1,2); // because of ownership transfer
        } catch e {
          writeln(e);
          halt("fail");
        }
      }
      // cs
      {
        writeln("cs:Child");
        var x = cs:Child;
        writeln(x.type:string, " ", x);
      }
      // csq
      {
        writeln("csq:Child");
        try {
          var x = csq:Child;
          writeln(x.type:string, " ", x);
        } catch e {
          writeln(e);
          halt("fail");
        }
      }
      // noq
      {
        writeln("noq:Child");
        try {
          var x = noq:Child;
          writeln(x.type:string, " ", x);
          halt("fail");
        } catch e {
          writeln(e);
        }
      }
      // nsq
      {
        writeln("nsq:Child");
        try {
          var x = nsq:Child;
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
      // co
      {
        writeln("co:Child?");
        var x = co:Child?;
        writeln(x.type:string, " ", x);
        co = new owned Child(1,2); // because of ownership transfer
      }
      // coq
      {
        writeln("coq:Child?");
        var x = coq:Child?;
        writeln(x.type:string, " ", x);
        coq = new owned Child(1,2); // because of ownership transfer
      }
      // cs
      {
        writeln("cs:Child?");
        var x = cs:Child?;
        writeln(x.type:string, " ", x);
      }
      // csq
      {
        writeln("csq:Child?");
        var x = csq:Child?;
        writeln(x.type:string, " ", x);
      }
      // noq
      {
        writeln("noq:Child?");
        var x = noq:Child?;
        writeln(x.type:string, " ", x);
        assert(x == nil);
      }
      // nsq
      {
        writeln("nsq:Child?");
        var x = nsq:Child?;
        writeln(x.type:string, " ", x);
        assert(x == nil);
      }
    }


    // casting to borrowed Child
    {
      writeln();
      writeln("casts to borrowed Child");
      // co
      {
        writeln("co:borrowed Child");
        var x = co:borrowed Child;
        writeln(x.type:string, " ", x);
        var y = co:(borrowed!);
        writeln(y.type:string, " ", y);
      }
      // coq
      {
        writeln("coq:borrowed Child");
        try {
          var x = coq:borrowed Child;
          writeln(x.type:string, " ", x);
        } catch e {
          writeln(e);
          halt("fail");
        }
        try {
          var y = coq:(borrowed!);
          writeln(y.type:string, " ", y);
        } catch e {
          writeln(e);
          halt("fail");
        }
      }
      // cs
      {
        writeln("cs:borrowed Child");
        var x = cs:borrowed Child;
        writeln(x.type:string, " ", x);
        var y = cs:(borrowed!);
        writeln(y.type:string, " ", y);
      }
      // csq
      {
        writeln("csq:borrowed Child");
        try {
          var x = csq:borrowed Child;
          writeln(x.type:string, " ", x);
        } catch e {
          writeln(e);
          halt("fail");
        }
        try {
          var y = csq:(borrowed!);
          writeln(y.type:string, " ", y);
        } catch e {
          writeln(e);
          halt("fail");
        }
      }
      // noq
      {
        writeln("noq:borrowed Child");
        try {
          var x = noq:borrowed Child;
          writeln(x.type:string, " ", x);
          halt("fail");
        } catch e {
          writeln(e);
        }
        try {
          var y = noq:(borrowed!);
          writeln(y.type:string, " ", y);
          halt("fail");
        } catch e {
          writeln(e);
        }
      }
      // nsq
      {
        writeln("nsq:borrowed Child");
        try {
          var x = nsq:borrowed Child;
          writeln(x.type:string, " ", x);
          halt("fail");
        } catch e {
          writeln(e);
        }
        try {
          var y = nsq:(borrowed!);
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
      // co
      {
        writeln("co:borrowed Child?");
        var x = co:borrowed Child?;
        writeln(x.type:string, " ", x);
        var y = co:borrowed?;
        writeln(y.type:string, " ", y);
      }
      // coq
      {
        writeln("coq:borrowed Child?");
        var x = coq:borrowed Child?;
        writeln(x.type:string, " ", x);
        var y = coq:borrowed?;
        writeln(y.type:string, " ", y);
      }
      // cs
      {
        writeln("cs:borrowed Child?");
        var x = cs:borrowed Child?;
        writeln(x.type:string, " ", x);
        var y = cs:borrowed?;
        writeln(y.type:string, " ", y);
      }
      // csq
      {
        writeln("csq:borrowed Child?");
        var x = csq:borrowed Child?;
        writeln(x.type:string, " ", x);
        var y = csq:borrowed?;
        writeln(y.type:string, " ", y);
      }
      // noq
      {
        writeln("noq:borrowed Child?");
        var x = noq:borrowed Child?;
        writeln(x.type:string, " ", x);
        assert(x == nil);
        var y = noq:borrowed?;
        writeln(y.type:string, " ", y);
        assert(y == nil);
      }
      // nsq
      {
        writeln("nsq:borrowed Child?");
        var x = nsq:borrowed Child?;
        writeln(x.type:string, " ", x);
        assert(x == nil);
        var y = nsq:borrowed?;
        writeln(y.type:string, " ", y);
        assert(y == nil);
      }
    }

    // casting to owned Child
    {
      writeln();
      writeln("casts to owned Child");
      // co
      {
        writeln("co:owned Child");
        var x = co:owned Child;
        writeln(x.type:string, " ", x);
        co = new owned Child(1,2); // because of ownership transfer
        //var y = co:owned;
        //writeln(y.type:string, " ", y);
        //co = new owned Child(1,2); // because of ownership transfer
      }
      // coq
      {
        writeln("coq:owned Child");
        try {
          var x = coq:owned Child;
          writeln(x.type:string, " ", x);
          coq = new owned Child(1,2); // because of ownership transfer
        } catch e {
          writeln(e);
          halt("fail");
        }
        //try {
        //  var y = coq:owned;
        //  writeln(y.type:string, " ", y);
        //  coq = new owned Child(1,2); // because of ownership transfer
        //} catch e {
        //  writeln(e);
        //  halt("fail");
        //}
      }
      // noq
      {
        writeln("noq:owned Child");
        try {
          var x = noq:owned Child;
          writeln(x.type:string, " ", x);
          halt("fail");
        } catch e {
          writeln(e);
        }
        //try {
        //  var y = noq:owned;
        //  writeln(y.type:string, " ", y);
        //  halt("fail");
        //} catch e {
        //  writeln(e);
        //}
      }
    }

    // casting to owned Child?
    {
      writeln();
      writeln("casts to owned Child?");
      // co
      {
        writeln("co:owned Child?");
        var x = co:owned Child?;
        writeln(x.type:string, " ", x);
	co = new owned Child(1,2); // because of ownership transfer
        //var y = co:owned?;
        //writeln(y.type:string, " ", y);
	//co = new owned Child(1,2); // because of ownership transfer
      }
      // coq
      {
        writeln("coq:owned Child?");
        var x = coq:owned Child?;
        writeln(x.type:string, " ", x);
	coq = new owned Child(1,2); // because of ownership transfer
        //var y = coq:owned?;
        //writeln(y.type:string, " ", y);
	//coq = new owned Child(1,2); // because of ownership transfer
      }
      // noq
      {
        writeln("noq:owned Child?");
        var x = noq:owned Child?;
        writeln(x.type:string, " ", x);
        assert(x == nil);
        //var y = noq:owned?;
        //writeln(y.type:string, " ", y);
        //assert(y == nil);
      }
    }

    // casting to shared Child
    {
      writeln();
      writeln("casts to shared Child");
      // cs
      {
        writeln("cs:shared Child");
        var x = cs:shared Child;
        writeln(x.type:string, " ", x);
        //var y = cs:shared;
        //writeln(y.type:string, " ", y);
      }
      // csq
      {
        writeln("csq:shared Child");
        try {
          var x = csq:shared Child;
          writeln(x.type:string, " ", x);
        } catch e {
          writeln(e);
          halt("fail");
        }
        //try {
        //  var y = csq:shared;
        //  writeln(y.type:string, " ", y);
        //} catch e {
        //  writeln(e);
        //  halt("fail");
        //}
      }
      // nsq
      {
        writeln("nsq:shared Child");
        try {
          var x = nsq:shared Child;
          writeln(x.type:string, " ", x);
          halt("fail");
        } catch e {
          writeln(e);
        }
        //try {
        //  var y = nsq:shared;
        //  writeln(y.type:string, " ", y);
        //  halt("fail");
        //} catch e {
        //  writeln(e);
        //}
      }
    }

    // casting to shared Child?
    {
      writeln();
      writeln("casts to shared Child?");
      // cs
      {
        writeln("cs:shared Child?");
        var x = cs:shared Child?;
        writeln(x.type:string, " ", x);
        //var y = cs:shared?;
        //writeln(y.type:string, " ", y);
      }
      // csq
      {
        writeln("csq:shared Child?");
        var x = csq:shared Child?;
        writeln(x.type:string, " ", x);
      }
      // nsq
      {
        writeln("nsq:shared Child?");
        var x = nsq:shared Child?;
        writeln(x.type:string, " ", x);
        assert(x == nil);
      }
    }

    // casting to generic Parent
    {
      writeln();
      writeln("casts to generic Parent");
      // co
      {
        writeln("co:Parent");
        var x = co:Parent;
        writeln(x.type:string, " ", x);
        co = new owned Child(1,2); // because of ownership transfer
      }
      // coq
      {
        writeln("coq:Parent");
        try {
          var x = coq:Parent;
          writeln(x.type:string, " ", x);
          coq = new owned Child(1,2); // because of ownership transfer
        } catch e {
          writeln(e);
          halt("fail");
        }
      }
      // cs
      {
        writeln("cs:Parent");
        var x = cs:Parent;
        writeln(x.type:string, " ", x);
      }
      // csq
      {
        writeln("csq:Parent");
        try {
          var x = csq:Parent;
          writeln(x.type:string, " ", x);
        } catch e {
          writeln(e);
          halt("fail");
        }
      }
      // noq
      {
        writeln("noq:Parent");
        try {
          var x = noq:Parent;
          writeln(x.type:string, " ", x);
          halt("fail");
        } catch e {
          writeln(e);
        }
      }
      // nsq
      {
        writeln("nsq:Parent");
        try {
          var x = nsq:Parent;
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
      // co
      {
        writeln("co:Parent?");
        var x = co:Parent?;
        writeln(x.type:string, " ", x);
        co = new owned Child(1,2); // because of ownership transfer
      }
      // coq
      {
        writeln("coq:Parent?");
        var x = coq:Parent?;
        writeln(x.type:string, " ", x);
        coq = new owned Child(1,2); // because of ownership transfer
      }
      // cs
      {
        writeln("cs:Parent?");
        var x = cs:Parent?;
        writeln(x.type:string, " ", x);
      }
      // csq
      {
        writeln("csq:Parent?");
        var x = csq:Parent?;
        writeln(x.type:string, " ", x);
      }
      // noq
      {
        writeln("noq:Parent?");
        var x = noq:Parent?;
        writeln(x.type:string, " ", x);
        assert(x == nil);
      }
      // nsq
      {
        writeln("nsq:Parent?");
        var x = nsq:Parent?;
        writeln(x.type:string, " ", x);
        assert(x == nil);
      }
    }

    // casting to borrowed Parent
    {
      writeln();
      writeln("casts to borrowed Parent");
      // co
      {
        writeln("co:borrowed Parent");
        var x = co:borrowed Parent;
        writeln(x.type:string, " ", x);
      }
      // coq
      {
        writeln("coq:borrowed Parent");
        try {
          var x = coq:borrowed Parent;
          writeln(x.type:string, " ", x);
        } catch e {
          writeln(e);
          halt("fail");
        }
      }
      // cs
      {
        writeln("cs:borrowed Parent");
        var x = cs:borrowed Parent;
        writeln(x.type:string, " ", x);
      }
      // csq
      {
        writeln("csq:borrowed Parent");
        try {
          var x = csq:borrowed Parent;
          writeln(x.type:string, " ", x);
        } catch e {
          writeln(e);
          halt("fail");
        }
      }
      // noq
      {
        writeln("noq:borrowed Parent");
        try {
          var x = noq:borrowed Parent;
          writeln(x.type:string, " ", x);
          halt("fail");
        } catch e {
          writeln(e);
        }
      }
      // nsq
      {
        writeln("nsq:borrowed Parent");
        try {
          var x = nsq:borrowed Parent;
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
      // co
      {
        writeln("co:borrowed Parent?");
        var x = co:borrowed Parent?;
        writeln(x.type:string, " ", x);
      }
      // coq
      {
        writeln("coq:borrowed Parent?");
        var x = coq:borrowed Parent?;
        writeln(x.type:string, " ", x);
      }
      // cs
      {
        writeln("cs:borrowed Parent?");
        var x = cs:borrowed Parent?;
        writeln(x.type:string, " ", x);
      }
      // csq
      {
        writeln("csq:borrowed Parent?");
        var x = csq:borrowed Parent?;
        writeln(x.type:string, " ", x);
      }
      // noq
      {
        writeln("noq:borrowed Parent?");
        var x = noq:borrowed Parent?;
        writeln(x.type:string, " ", x);
        assert(x == nil);
      }
      // nsq
      {
        writeln("nsq:borrowed Parent?");
        var x = nsq:borrowed Parent?;
        writeln(x.type:string, " ", x);
        assert(x == nil);
      }
    }

    // casting to owned Parent
    {
      writeln();
      writeln("casts to owned Parent");
      // co
      {
        writeln("co:owned Parent");
        var x = co:owned Parent;
        writeln(x.type:string, " ", x);
        co = new owned Child(1,2); // because of ownership transfer
      }
      // coq
      {
        writeln("coq:owned Parent");
        try {
          var x = coq:owned Parent;
          writeln(x.type:string, " ", x);
	  coq = new owned Child(1,2); // because of ownership transfer
        } catch e {
          writeln(e);
          halt("fail");
        }
      }
      // noq
      {
        writeln("noq:owned Parent");
        try {
          var x = noq:owned Parent;
          writeln(x.type:string, " ", x);
          halt("fail");
        } catch e {
          writeln(e);
        }
      }
    }

    // casting to owned Parent?
    {
      writeln();
      writeln("casts to owned Parent?");
      // co
      {
        writeln("co:owned Parent?");
        var x = co:owned Parent?;
        writeln(x.type:string, " ", x);
      }
      // coq
      {
        writeln("coq:owned Parent?");
        var x = coq:owned Parent?;
        writeln(x.type:string, " ", x);
      }
      // noq
      {
        writeln("noq:owned Parent?");
        var x = noq:owned Parent?;
        writeln(x.type:string, " ", x);
        assert(x == nil);
      }
    }

    // casting to shared Parent
    {
      writeln();
      writeln("casts to shared Parent");
      // cs
      {
        writeln("cs:shared Parent");
        var x = cs:shared Parent;
        writeln(x.type:string, " ", x);
      }
      // csq
      {
        writeln("csq:shared Parent");
        try {
          var x = csq:shared Parent;
          writeln(x.type:string, " ", x);
        } catch e {
          writeln(e);
          halt("fail");
        }
      }
      // nsq
      {
        writeln("nsq:shared Parent");
        try {
          var x = nsq:shared Parent;
          writeln(x.type:string, " ", x);
          halt("fail");
        } catch e {
          writeln(e);
        }
      }
    }

    // casting to shared Parent?
    {
      writeln();
      writeln("casts to shared Parent?");
      // cs
      {
        writeln("cs:shared Parent?");
        var x = cs:shared Parent?;
        writeln(x.type:string, " ", x);
      }
      // csq
      {
        writeln("csq:shared Parent?");
        var x = csq:shared Parent?;
        writeln(x.type:string, " ", x);
      }
      // nsq
      {
        writeln("nsq:shared Parent?");
        var x = nsq:shared Parent?;
        writeln(x.type:string, " ", x);
        assert(x == nil);
      }
    }
  }
}
