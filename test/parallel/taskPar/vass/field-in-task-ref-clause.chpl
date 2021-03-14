// see also errorFieldMethodInWithClause.chpl
//
// with-clauses of forall loops are tested in
//  parallel/forall/vass/errorFieldMethodInWithClause.chpl

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
}  // class MyClass

var c = new owned MyClass();
writeln(c);
c.myBegin();
writeln(c);
c.myCobegin();
writeln(c);
c.myCoforall();
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
}  // record MyRecord

var r = new MyRerord();
writeln(r);
r.myBegin();
writeln(r);
r.myCobegin();
writeln(r);
r.myCoforall();
writeln(r);
