record independentRecord { var field = 5;} 

var ir = new independentRecord;
writeln(ir.field);
//above lines compile with/without --verify

class outerClass {
  // var r: innerRecord; //doesn't work

  // following line throws an internal bug error when compiled with --verify
  // when compiled with --devel on top of that error message is more useful
  var r = new innerRecord;

  // var r = new innerRecord(); // works

  record innerRecord{ var field = 7;}
}

var oc = new outerClass;
writeln(oc.r.field);
