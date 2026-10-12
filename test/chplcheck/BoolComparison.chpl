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

  proc getBool(): bool {
    return true;
  }
  record R {
    proc type get(): bool do return false;
  }
  proc combos(flag1, flag2) {
    if (flag1 && flag2) == false {
      writeln("flag1 is false");
    }
    if 1 > 2 == false {
      writeln("1 is greater than 2");
    }
    if (2 < 1) == false {
      writeln("2 is less than 1");
    }
    if (flag2) == false {
      writeln("flag2 is false");
    }
    while (getBool()) == false {
      writeln("In while loop");
      break;
    }
    var arr: [1..10] bool;
    var r1 = (arr[1] == false);
    var r2 = (R.get() && R.get()) == false;
    return R.get() == false;
  }

}
