config param case = 0;

class MyClass {
  var myField:int = 111;
  proc myBegin() {
    if case == 1 {
     sync {
      begin with (ref myField) {
        myField = 222;
      }
     }
    }
  }
  proc myCobegin() {
    if case == 2 {
      cobegin with (ref myField) {
        myField = 333;
        var i = 444;
      }
    }
  }
  proc myCoforall() {
    if case == 3 {
      coforall 1..1 with (ref myField) {
        myField = 555;
      }
    }
  }
  proc myForall() {
    if case == 4 {
      // todo: uncomment the 'ref' clause when implemented
      coforall 1..1 /*with (ref myField)*/ {
        myField = 666;
      }
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
    if case == 5 {
     sync {
      begin with (ref myField) {
        myField = 222;
      }
     }
    }
  }
  proc myCobegin() {
    if case == 6 {
      cobegin with (ref myField) {
        myField = 333;
        var i = 444;
      }
    }
  }
  proc myCoforall() {
    if case == 7 {
      coforall 1..1 with (ref myField) {
        myField = 555;
      }
    }
  }
  proc myForall() {
    if case == 8 {
      // todo: uncomment the 'ref' clause when implemented
      coforall 1..1 /*with (ref myField)*/ {
        myField = 666;
      }
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
