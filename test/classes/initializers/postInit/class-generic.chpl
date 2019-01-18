// Verify that postinit() is called correctly when the new
// expression is a statement i.e. has a lock statement as
// its parent.
//
// This can occur in two situations
//
//   1) A new expression at top level.
//      Note that the instance will leak
//
//   2) A type/init  expr for a procedure formal
//

class MyCls {
  type t;
  var  x : t;

  proc init(x : int) {
    this.t = int;
    this.x = x;

    writeln('MyCls.init');
  }

  proc postinit() {
    writeln('MyCls.postinit');
  }
}




proc main() {
  // 1) A new expression at top level
  new unmanaged MyCls(10);

  writeln();

  foo();
}





// The initExpr is a block statement with the new expr.
proc foo(x = new unmanaged MyCls(20)) {
  writeln(x);
  writeln();
}
