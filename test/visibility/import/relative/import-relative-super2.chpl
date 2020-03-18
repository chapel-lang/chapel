module M {
  module A {
    module AA {
      import super.super.B.aa;
      import super.super.B.faa;

      module AAA {
        import super.super.super.B.aaa;
        import super.super.super.B.faaa;
        proc gaaa() {
          writeln(aaa);
          writeln(faaa());
        }
      }
      public import this.AAA;

      proc gaa() {
        writeln(aa);
        writeln(faa());
      }
    }
    public import this.AA;

    import super.B.a;
    import super.B.fa;
    proc ga() {
      writeln(a);
      writeln(fa());
    }
  }
  module B {
    var a  = 1;
    proc fa() {
      return 1;
    }

    var aa  = 2;
    proc faa() {
      return 2;
    }

    var aaa  = 3;
    proc faaa() {
      return 3;
    }
  }

  import this.A;
  import this.B;

  proc main() {
    A.ga();
    A.AA.gaa();
    A.AA.AAA.gaaa();
  }
}

