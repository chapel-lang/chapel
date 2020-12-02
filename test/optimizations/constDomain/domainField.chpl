{
  writeln("Pass literal to var field");
  class C {
    var d: domain(1);
  }

  var c = new C({1..10});

  writeln(c.d.definedConst);
  writeln(c.d._value.definedConst);
}

{
  writeln("Pass const to var field");
  class C {
    var d: domain(1);
  }

  const d = {1..10};
  var c = new C(d);

  writeln(c.d.definedConst);
  writeln(c.d._value.definedConst);
}

{
  writeln("Pass literal to const field");
  class C {
    const d: domain(1);
  }

  var c = new C({1..10});

  writeln(c.d.definedConst);
  writeln(c.d._value.definedConst);
}

{
  writeln("Pass var to const field");
  class C {
    const d: domain(1);
  }

  var d = {1..10};
  var c = new C(d);

  writeln(c.d.definedConst);
  writeln(c.d._value.definedConst);
}
