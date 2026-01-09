module BoolComparison {
  proc checkFlag(flag: bool) {
    if flag == true {
      writeln("Flag is true");
    } else if flag == false {
      writeln("Flag is false");
    }

    var isTrue = flag != false;
    var isFalse = flag == false;
  }
}
