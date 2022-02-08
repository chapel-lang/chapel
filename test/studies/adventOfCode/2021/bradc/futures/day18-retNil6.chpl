class C { }

config const coinFlip = true;

proc nilOrNot() {
  if coinFlip then
    return new C?();
  else
    return nil: owned C?;
}

var myC = nilOrNot();
writeln(myC);


