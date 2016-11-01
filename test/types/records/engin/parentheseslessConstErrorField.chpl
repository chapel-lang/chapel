class outerClass {
  // var r: innerRecord; //doesn't work

  // Following line throws an internal bug error when compiled with --verify.
  // Also, when compiled with --devel on top of that, error message is more 
  // useful
  var r = new innerRecord;

  // var r = new innerRecord(); // works

  record innerRecord{ var field = 7;}
}

var oc = new outerClass;
writeln(oc.r.field);
