proc foo() {
  begin {
    return true;
  }
  cobegin {
    return false;
    return true;
  }
  coforall i in 1..3 {
    break;
  }
  return true;
}

writeln(foo());
