module M {

  module L {
    module K {
      var x:int;
      proc foo() { return 1; }
    }
    module J {
      import super.K.x; 
      import super.K.foo;
      proc bar() {
        writeln(x);
        writeln(foo());
      }
    }

    public import this.K;
    public import this.J;
  }

  import this.L.K.x;
  import this.L.K.foo;
  import this.L.J.bar;

  proc main() {
    writeln(x);
    writeln(foo());

    bar();
  }
}
