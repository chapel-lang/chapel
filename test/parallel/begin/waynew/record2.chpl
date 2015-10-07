// Test a begin block with a reference to a local record variable

record R {
  var x: int;
}

proc jam() {
  var r: R;
  r.x = 7;
  writeln( r);

  sync {  // make sure we don't de-allocate 'r' prior to leaving jam()'s scope
    begin with (ref r) {
      r.x = 14;
      writeln( r);
    }
  }
}

jam();
