// This test is intentionally racy. It should not generally be run.

record R {
  var x:int = 0;
  proc deinit() {
    assert(x == 0);
    x = 99;
    writeln("deinit");
  }
}

config const option = true;

proc t28EOB() {
  writeln("t28");
  sync {
    var r: R;
    begin {
      writeln("begin inner");
      r;
      writeln("end inner");
    }
  }
  writeln("end outer");
}
t28EOB();

proc t29EOB() {
  writeln("t29");
  sync {
    {
      var r: R;
      begin {
        writeln("begin inner");
        r;
        writeln("end inner");
      }
    }
  }
  writeln("end outer");
}
t29EOB();

proc t30EOB() {
  writeln("t30");
  sync {
    var r: R;
    {
      begin {
        writeln("begin inner");
        r;
        writeln("end inner");
      }
    }
  }
  writeln("end outer");
}
t30EOB();

proc t31EOB() {
  writeln("t31");
  sync {
    if option {
      var r: R;
      if option {
        begin {
          writeln("begin inner");
          r;
          writeln("end inner");
        }
      }
    }
  }
  writeln("end outer");
}
t31EOB();

proc t32EOB() {
  writeln("t32");
  var r: R;
  {
    if option {
      begin {
        writeln("begin inner");
        r;
        writeln("end inner");
      }
    }
  }
  writeln("end outer");
}
t32EOB();
