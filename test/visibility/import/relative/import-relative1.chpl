module M {

  module L {
    module K {
      var x:int;
      proc foo() { return 1; }
    }
    module J {
      public import super.K; 
      proc bar() {
        writeln(K.x);
        writeln(K.foo());
      }
    }

    public import this.K;
    public import this.J;
  }

  import this.L.K;

  proc main() {
    writeln(K.x);
    writeln(K.foo());

    L.J.bar();
  }
}
