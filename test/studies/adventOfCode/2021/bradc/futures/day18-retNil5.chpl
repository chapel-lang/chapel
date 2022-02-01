class C { }

config const coinFlip = true;

proc nilOrNot() {
  if coinFlip then
    return new C?();
  else
    return nil: C?;
}

var myC = nilOrNot();
writeln(myC);


