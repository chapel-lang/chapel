module OuterModule {

  // Based on original program from #8555, combined with comment by @cassella
  // demonstrating other failure modes.

  module A {
    record CR {
      var a: int;

      proc init(a: int) {
        this.a = a + 1;
      }
    }

    record GR {
      type t;
      var a : t;
      proc init(type t, a:t) {
        this.t = t;
        this.a = a + 1;
      }
    }

    class CC {
      var a: int;

      proc init(a: int) {
        this.a = a + 1;
      }
    }

    class GC {
      type t;
      var a : t;
      proc init(type t, a:t) {
        this.t = t;
        this.a = a + 1;
      }
    }
  }

  proc printThing(x) {
    use A; // get 'writeThis'
    writeln(x);
  }

  writeln("Concrete Record");
  {         var x : A.CR = new A.CR(1); printThing(x); }
  { use A;  var x :   CR = new A.CR(1); printThing(x); }
  { use A;  var x        = new A.CR(1); printThing(x); }
  {         var x        = new A.CR(1); printThing(x); }
  { use A;  var x : A.CR = new   CR(1); printThing(x); }
  { use A;  var x :   CR = new   CR(1); printThing(x); }
  { use A;  var x        = new   CR(1); printThing(x); }

  writeln("\nConcrete Class");
  {         var x : owned A.CC = new owned A.CC(1); printThing(x); }
  { use A;  var x : owned   CC = new owned A.CC(1); printThing(x); }
  { use A;  var x              = new owned A.CC(1); printThing(x); }
  {         var x              = new owned A.CC(1); printThing(x); }
  { use A;  var x : owned A.CC = new owned   CC(1); printThing(x); }
  { use A;  var x : owned CC   = new owned   CC(1); printThing(x); }
  { use A;  var x              = new owned   CC(1); printThing(x); }

  writeln("\nGeneric Record");
  {         var x : A.GR(int) = new A.GR(int, 1); printThing(x); }
  { use A;  var x :   GR(int) = new A.GR(int, 1); printThing(x); }
  { use A;  var x             = new A.GR(int, 1); printThing(x); }
  {         var x             = new A.GR(int, 1); printThing(x); }
  { use A;  var x : A.GR(int) = new   GR(int, 1); printThing(x); }
  { use A;  var x :   GR(int) = new   GR(int, 1); printThing(x); }
  { use A;  var x             = new   GR(int, 1); printThing(x); }

  writeln("\nGeneric Class");
  {         var x : owned A.GC(int) = new owned A.GC(int, 1); printThing(x); }
  { use A;  var x : owned   GC(int) = new owned A.GC(int, 1); printThing(x); }
  { use A;  var x                   = new owned A.GC(int, 1); printThing(x); }
  {         var x                   = new owned A.GC(int, 1); printThing(x); }
  { use A;  var x : owned A.GC(int) = new owned   GC(int, 1); printThing(x); }
  { use A;  var x : owned   GC(int) = new owned   GC(int, 1); printThing(x); }
  { use A;  var x                   = new owned   GC(int, 1); printThing(x); }
}
