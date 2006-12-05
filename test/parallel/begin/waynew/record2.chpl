// Test a begin block with a reference to a local record variable

class R {
  var x: int;
}

def jam() {
  var r = R();
  r.x = 7;
  writeln( r);

  begin {
    r.x = 14;
    writeln( r);
  }
}

jam();
