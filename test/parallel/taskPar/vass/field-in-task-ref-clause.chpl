// see also errorFieldMethodInWithClause.chpl

class MyClass {
  var myField:int = 111;
  proc myBegin() {
     sync {
      begin with (ref myField) {
        writeln(myField);
      }
     }
  }
  proc myCobegin() {
      cobegin with (ref myField) {
        writeln(myField);
        writeln(myField);
      }
  }
  proc myCoforall() {
      coforall 1..1 with (ref myField) {
        writeln(myField);
      }
  }
  proc myForall() {
      forall 1..1 with (ref myField) {
        writeln(myField);
      }
  }
}  // class MyClass

var c = new MyClass();
writeln(c);
c.myBegin();
writeln(c);
c.myCobegin();
writeln(c);
c.myCoforall();
writeln(c);
c.myForall();
writeln(c);

record MyRecord {
  var myField:int = 111;
  proc myBegin() {
     sync {
      begin with (ref myField) {
        writeln(myField);
      }
     }
  }
  proc myCobegin() {
      cobegin with (ref myField) {
        writeln(myField);
        writeln(myField);
      }
  }
  proc myCoforall() {
      coforall 1..1 with (ref myField) {
        writeln(myField);
      }
  }
  proc myForall() {
      forall 1..1 with (ref myField) {
        writeln(myField);
      }
  }
}  // record MyRecord

var r = new MyRerord();
writeln(r);
r.myBegin();
writeln(r);
r.myCobegin();
writeln(r);
r.myCoforall();
writeln(r);
r.myForall();
writeln(r);
