
// Based on original program from #8555, combined with comment by @cassella
// demonstrating other failure modes.

module A {
  record R {
    var a: int;

    proc init(a: int) {
      this.a = a + 1;
    }
  }
}

proc printR(r:A.R) {
  use A; // get 'writeThis'
  writeln(r);
}

{         var r : A.R = new A.R(1); printR(r); }
{ use A;  var r :   R = new A.R(1); printR(r); }
{ use A;  var r       = new A.R(1); printR(r); }
{         var r       = new A.R(1); printR(r); }

{ use A;  var r : A.R = new   R(1); printR(r); }
{ use A;  var r :   R = new   R(1); printR(r); }
{ use A;  var r       = new   R(1); printR(r); }
