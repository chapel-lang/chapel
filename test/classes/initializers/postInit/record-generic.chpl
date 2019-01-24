// Verify that postinit() is called correctly when the new
// expression is a statement i.e. has a block statement as
// its parent.
//
// This can occur in two situations
//
//   1) A new expression at top level.
//      Note that the instance will leak
//
//   2) A type/init  expr for a procedure formal
//

record MyRec {
  type t;
  var  x : t;

  proc init() {
    this.t = int;
    this.x = 10;

    writeln('MyRec.init default');
  }

  proc init(type t) {
    this.t = t;

    writeln('MyRec.init type');
  }

  proc init(x : int) {
    this.t = int;
    this.x = x;

    writeln('MyRec.init value');
  }

  proc postinit() {
    writeln('MyRec.postinit');
    writeln();
  }
}




proc main() {
  var r1 : MyRec(int);

  writeln(r1);
  writeln();



  var r2 : MyRec(int) = new MyRec(20);

  writeln(r2);
  writeln();



  var r3         = new MyRec(30);

  writeln(r3);
  writeln();



  r3 = new MyRec(40);
  writeln(r3);
  writeln();

  // 1) A new expression at top level
  new MyRec(50);

  writeln();

  foo();
}





// The initExpr is a block statement with the new expr.
proc foo(x = new MyRec(20)) {
  writeln('foo ', x);
  writeln();
}


