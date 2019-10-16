module test {
  class Parent { var p:int; }

  class Child : Parent { var c:int; }

  proc main() {
    var co:owned Parent = new owned Child(1,2);
    var coq:owned Parent? = new owned Child(1,2);
    var cs:shared Parent = new shared Child(1,2);
    var csq:shared Parent? = new shared Child(1,2);

    var po:owned Parent = new owned Parent(3);
    var poq:owned Parent? = new owned Parent(3);
    var ps:shared Parent = new shared Parent(3);
    var psq:shared Parent? = new shared Parent(3);

    var noq:owned Parent? = nil;
    var nsq:shared Parent? = nil;

    // casting to borrowed Child
    {
      writeln();
      writeln("casts to borrowed Child");

      // from co
      try {
        writeln("co:borrowed Child");
        var x = co:borrowed Child;
        writeln(x.type:string, " ", x);
        assert(co != nil);
      } catch e {
        writeln(e);
        halt("fail");
      }
      // from coq
      try {
        writeln("coq:borrowed Child");
        var x = coq:borrowed Child;
        writeln(x.type:string, " ", x);
        assert(coq != nil);
      } catch e {
        writeln(e);
        halt("fail");
      }
      // from cs
      try {
        writeln("cs:borrowed Child");
        var x = cs:borrowed Child;
        writeln(x.type:string, " ", x);
        assert(cs != nil);
      } catch e {
        writeln(e);
        halt("fail");
      }
      // from csq
      try {
        writeln("csq:borrowed Child");
        var x = csq:borrowed Child;
        writeln(x.type:string, " ", x);
      } catch e {
        writeln(e);
        halt("fail");
      }
      // from po
      try {
        writeln("po:borrowed Child");
        var x = po:borrowed Child;
        writeln(x.type:string, " ", x);
        halt("fail");
      } catch e {
        writeln(e);
        assert(po != nil);
      }
      // from poq
      try {
        writeln("poq:borrowed Child");
        var x = poq:borrowed Child;
        writeln(x.type:string, " ", x);
        halt("fail");
      } catch e {
        writeln(e);
        assert(poq != nil);
      }
      // from ps
      try {
        writeln("ps:borrowed Child");
        var x = ps:borrowed Child;
        writeln(x.type:string, " ", x);
        halt("fail");
      } catch e {
        writeln(e);
        assert(ps != nil);
      }
      // from psq
      try {
        writeln("psq:borrowed Child");
        var x = psq:borrowed Child;
        writeln(x.type:string, " ", x);
        halt("fail");
      } catch e {
        writeln(e);
        assert(psq != nil);
      }
      // from noq
      try {
        writeln("noq:borrowed Child");
        var x = noq:borrowed Child;
        writeln(x.type:string, " ", x);
        halt("fail");
      } catch e {
        writeln(e);
      }
      // from nsq
      try {
        writeln("nsq:borrowed Child");
        var x = nsq:borrowed Child;
        writeln(x.type:string, " ", x);
        halt("fail");
      } catch e {
        writeln(e);
      }
    }

    // casting to borrowed Child?
    {
      writeln();
      writeln("casts to borrowed Child?");

      // from co
      {
        writeln("co:borrowed Child?");
        var x = co:borrowed Child?;
        writeln(x.type:string, " ", x);
        assert(x != nil);
        assert(co != nil);
      }
      // from coq
      {
        writeln("coq:borrowed Child?");
        var x = coq:borrowed Child?;
        writeln(x.type:string, " ", x);
        assert(x != nil);
        assert(coq != nil);
      }
      // from cs
      {
        writeln("cs:borrowed Child?");
        var x = cs:borrowed Child?;
        writeln(x.type:string, " ", x);
        assert(x != nil);
        assert(cs != nil);
      }
      // from csq
      {
        writeln("csq:borrowed Child?");
        var x = csq:borrowed Child?;
        writeln(x.type:string, " ", x);
        assert(x != nil);
        assert(csq != nil);
      }
      // from po
      {
        writeln("po:borrowed Child?");
        var x = po:borrowed Child?;
        writeln(x.type:string, " ", x);
        assert(x == nil);
        assert(po != nil);
      }
      // from poq
      {
        writeln("poq:borrowed Child?");
        var x = poq:borrowed Child?;
        writeln(x.type:string, " ", x);
        assert(x == nil);
        assert(poq != nil);
      }
      // from ps
      {
        writeln("ps:borrowed Child?");
        var x = ps:borrowed Child?;
        writeln(x.type:string, " ", x);
        assert(x == nil);
        assert(ps != nil);
      }
      // from psq
      {
        writeln("psq:borrowed Child?");
        var x = psq:borrowed Child?;
        writeln(x.type:string, " ", x);
        assert(x == nil);
        assert(psq != nil);
      }
      // from noq
      {
        writeln("noq:borrowed Child?");
        var x = noq:borrowed Child?;
        writeln(x.type:string, " ", x);
        assert(x == nil);
      }
      // from nsq
      {
        writeln("nsq:borrowed Child?");
        var x = nsq:borrowed Child?;
        writeln(x.type:string, " ", x);
        assert(x == nil);
      }
    }

    // casting to owned Child
    {
      writeln();
      writeln("casts to owned Child");

      // from co
      try {
        writeln("co:owned Child");
        var x = co:owned Child;
        writeln(x.type:string, " ", x);
        co = new owned Child(1,2); // since we did ownership transfer
      } catch e {
        writeln(e);
        halt("fail");
      }
      // from coq
      try {
        writeln("coq:owned Child");
        var x = coq:owned Child;
        writeln(x.type:string, " ", x);
        coq = new owned Child(1,2); // since we did ownership transfer
      } catch e {
        writeln(e);
        halt("fail");
      }
      // from po
      try {
        writeln("po:owned Child");
        var x = po:owned Child;
        writeln(x.type:string, " ", x);
        halt("fail");
      } catch e {
        writeln(e);
        assert(po != nil); // should not transfer if cast failed
        po = new owned Parent(3);
      }
      // from poq
      try {
        writeln("poq:owned Child");
        var x = poq:owned Child;
        writeln(x.type:string, " ", x);
        halt("fail");
      } catch e {
        writeln(e);
        assert(poq != nil); // should not transfer if cast failed
        poq = new owned Parent(3);
      }
      // from noq
      try {
        writeln("noq:owned Child");
        var x = noq:owned Child;
        writeln(x.type:string, " ", x);
        halt("fail");
      } catch e {
        writeln(e);
      }
    }


    // casting to owned Child?
    {
      writeln();
      writeln("casts to owned Child?");

      // from co
      {
        writeln("co:owned Child?");
        var x = co:owned Child?;
        writeln(x.type:string, " ", x);
        assert(x != nil);
        co = new owned Child(1,2); // since we did ownership transfer
      }
      // from coq
      {
        writeln("coq:owned Child?");
        var x = coq:owned Child?;
        writeln(x.type:string, " ", x);
        assert(x != nil);
        coq = new owned Child(1,2); // since we did ownership transfer
      }
      // from po
      {
        writeln("po:owned Child?");
        var x = po:owned Child?;
        writeln(x.type:string, " ", x);
        assert(x == nil);
        assert(po != nil); // should not transfer if cast failed
        po = new owned Parent(3);
      }
      // from poq
      {
        writeln("poq:owned Child?");
        var x = poq:owned Child?;
        writeln(x.type:string, " ", x);
        assert(x == nil);
        assert(poq != nil); // should not transfer if cast failed
        poq = new owned Parent(3);
      }
      // from noq
      {
        writeln("noq:owned Child?");
        var x = noq:owned Child?;
        writeln(x.type:string, " ", x);
        assert(x == nil);
      }
    }

    // casting to shared Child
    {
      writeln();
      writeln("casts to shared Child");

      // from cs
      try {
        writeln("cs:shared Child");
        var x = cs:shared Child;
        writeln(x.type:string, " ", x);
        assert(cs != nil);
      } catch e {
        writeln(e);
        halt("fail");
      }
      // from csq
      try {
        writeln("csq:shared Child");
        var x = csq:shared Child;
        writeln(x.type:string, " ", x);
        assert(csq != nil);
      } catch e {
        writeln(e);
        halt("fail");
      }
      // from ps
      try {
        writeln("ps:shared Child");
        var x = ps:shared Child;
        writeln(x.type:string, " ", x);
        halt("fail");
      } catch e {
        writeln(e);
        assert(ps != nil);
      }
      // from psq
      try {
        writeln("psq:shared Child");
        var x = psq:shared Child;
        writeln(x.type:string, " ", x);
        halt("fail");
      } catch e {
        writeln(e);
        assert(psq != nil);
      }
      // from nsq
      try {
        writeln("nsq:shared Child");
        var x = nsq:shared Child;
        writeln(x.type:string, " ", x);
        halt("fail");
      } catch e {
        writeln(e);
      }
    }

    // casting to shared Child?
    {
      writeln();
      writeln("casts to shared Child?");

      // from cs
      {
        writeln("cs:shared Child?");
        var x = cs:shared Child?;
        writeln(x.type:string, " ", x);
        assert(x != nil);
      }
      // from csq
      {
        writeln("csq:shared Child?");
        var x = csq:shared Child?;
        writeln(x.type:string, " ", x);
        assert(x != nil);
      }

      // from ps
      {
        writeln("ps:shared Child?");
        var x = ps:shared Child?;
        writeln(x.type:string, " ", x);
        assert(x == nil);
        assert(ps != nil);
      }
      // from psq
      {
        writeln("psq:shared Child?");
        var x = psq:shared Child?;
        writeln(x.type:string, " ", x);
        assert(x == nil);
        assert(psq != nil);
      }
      // from nsq
      {
        writeln("nsq:shared Child?");
        var x = nsq:shared Child?;
        writeln(x.type:string, " ", x);
        assert(x == nil);
      }
    }
  }
}
