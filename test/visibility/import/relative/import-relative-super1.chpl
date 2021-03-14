module M {
  module A {
    module AA {
      import super.super.B;

      module AAA {
        import super.super.super.B;
        proc gaaa() {
          writeln(B.aaa);
          writeln(B.faaa());
        }
      }
      public import this.AAA;

      proc gaa() {
        writeln(B.aa);
        writeln(B.faa());
      }
    }
    public import this.AA;

    import super.B;
    proc ga() {
      writeln(B.a);
      writeln(B.fa());
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

