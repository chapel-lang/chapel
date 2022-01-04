record R {
  var x: int;
}


proc returnTupleTwoRs() {
  return (new R(3), new R(4));
}

config const something = true;

proc acceptTwoRs(arg) {
  var x;

  if something {
    x = returnTupleTwoRs();
  } else {
    x = arg;
  }
}

proc main() {
  acceptTwoRs( (new R(1), new R(2)) );
}
