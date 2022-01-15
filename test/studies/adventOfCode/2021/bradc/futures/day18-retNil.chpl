class C { }

config const coinFlip = true;

proc nilOrNot() {
  if coinFlip then
    return new C();
  else
    return nil;
}

var myC = nilOrNot();
writeln(myC);
