class C {
  proc f { return 1; }
  proc g() { return 2; }
  
}

var c = (new owned C()).borrow();

writeln(c.f());

