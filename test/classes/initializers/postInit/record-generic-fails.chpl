// Split out from record-generic.chpl to track the failure mode when
// the instance is declared with only a type and no initial value

// When postinit is no longer skipped for this case, it should be merged
// back with record-generic.chpl

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

  proc postinit() { // This gets skipped, but shouldn't
    writeln('MyRec.postinit');
    writeln();
  }
}

proc main() {
  var r1 : MyRec(int);

  writeln(r1);
}
