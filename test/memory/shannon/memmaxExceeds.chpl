class C {
  var x0: complex(128);
  var x1: complex(128);
  var x2: complex(128);
  var x3: complex(128);
  var x4: complex(128);
  var x5: complex(128);
  var x6: complex(128);
  var x7: complex(128);
  var x8: complex(128);
  var x9: complex(128);
}

var c = new C();

writeln("Shouldn't get here if --memmax is less than 164");
