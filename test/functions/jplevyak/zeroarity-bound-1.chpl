class C {
  function f { return 1; }
  function g() { return 2; }
  
}

var c = C();

writeln(c.f);
writeln(c.g());

writeln(c.g);
writeln(c.f());

