module BoolLits {
  proc myProc () {
    if true then
      writeln("true");
    else
      writeln("false");

    if false then
      writeln("false");
    else
      writeln("true");

    if true {
      writeln("true");
    } else {
      writeln("false");
    }

    if false {
      writeln("false");
    } else {
      writeln("true");
    }

    if true then {
      writeln("true");
    } else {
      writeln("false");
    }

    if false then {
      writeln("false");
    } else {
      writeln("true");
    }
  }
}
