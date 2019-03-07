class C {
  var x: 100*complex(128);
}

var c = new unmanaged C();

writeln("Shouldn't get here if --memmax is less than 164");
