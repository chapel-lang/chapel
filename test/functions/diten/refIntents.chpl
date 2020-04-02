proc swap(ref l: ?t, ref r: t) {
  l <=> r;
  writeln("End of swap(", t:string, ")");
}

proc main {
  { writeln("Class fields");
    class C {
      var a: int;
      var b: int;
    }
    var c = new unmanaged C(1,2);
    writeln(c);
    swap(c.a, c.b);
    writeln(c);
    delete c;
  }
  { writeln("Tuple components");
    var t = (1,2,3);
    writeln(t);
    swap(t(0), t(2));
    writeln(t);
  }
  { writeln("Array elements");
    var A: [1..10] int = [i in 1..10] i;
    writeln((A(3), A(7)));
    swap(A(3), A(7));
    writeln((A(3), A(7)));
  }
  { writeln("Array sections");
    var A: [1..10] int = [i in 1..10] i;
    writeln(A);
    swap(A(1..3), A(4..6));
    writeln(A);
  }
  { writeln("Whole tuples");
    var t1 = (1,2,3), t2 = (4,5,6);
    writeln((t1, t2));
    swap(t1, t2);
    writeln((t1, t2));
  }
  { writeln("Records");
    record R {
      var a, b: int;
    }
    var r1 = new R(1,2);
    var r2 = new R(3,4);
    writeln(r1, r2);
    swap(r1, r2);
    writeln(r1, r2);
  }
  { writeln("Classes");
    class C {
      var a, b: int;
    }
    var c1 = new C(1,2);
    var c2 = new C(3,4);
    writeln(c1, c2);
    swap(c1, c2);
    writeln(c1, c2);
    delete c2;
    delete c1;
  }
  { writeln("Domains");
    var D1 = {1..10};
    var D2 = {3..13};
    writeln((D1, D2));
    swap(D1, D2);
    writeln((D1, D2));
  }
/* Arrays are not working for the right reason.  They just use normal array
 * pass by reference, not the ref intent.
 */
  { writeln("Arrays");
    var A1: [1..10] int;
    var A2: [1..10] int;
    for i in 1..10 { A1(i) = i; A2(i) = 10 - i + 1; }
    writeln((A1, A2));
    swap(A1, A2);
    writeln((A1, A2));
  }
  { writeln("Ranges");
    var r1 = 1..20 by 2;
    var r2 = 1..10 by -1;
    writeln((r1, r2));
    swap(r1, r2);
    writeln((r1, r2));
  }
  { writeln("Differently sized ranges");
    var r1 = 1..20;
    var r2 = 1..10;
    writeln((r1, r2));
    swap(r1, r2);
    writeln((r1, r2));
  }
  { writeln("Enum values");
    enum E { a, b, c, d };
    var e1 = E.a;
    var e2 = E.d;
    writeln((e1, e2));
    swap(e1, e2);
    writeln((e1, e2));
  }
  { writeln("Strings");
    var s1 = "string number one";
    var s2 = "differently sized string number two";
    writeln((s1, s2));
    swap(s1, s2);
    writeln((s1, s2));
  }
  { writeln("sync ints");
    var s1$: sync int = 1;
    var s2$: sync int = 2;
    writeln((s1$.readFF(), s2$.readFF()));
    swap(s1$, s2$);
    writeln((s1$.readFF(), s2$.readFF()));
  }
}
