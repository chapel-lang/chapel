record outerRecord {

  record innerRecord {
    var field = 5;
  }

  var field = 7;
  var nr: innerRecord; // problem here

  // neither of the below works
  // var nr = new innerRecord;
  // var nr = new innerRecord();

  // this method is just to show a working case of a similar issue
  proc outerProc() {
    var ir: innerRecord; //it works inside a record method
    writeln(ir.field);
  }
}

var or: outerRecord;
or.outerProc();

writeln(or.nr.field);
