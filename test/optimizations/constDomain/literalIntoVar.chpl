proc makeDomainLiteral() {
  return {1..3};
}

writeln("Passing around a literal returned from a function");
{
  writeln("Assign to var variable");
  var a = makeDomainLiteral();
  writeln(a.definedConst);
}

{
  writeln("Assign to const variable");
  const a = makeDomainLiteral();
  writeln(a.definedConst);
}

{
  writeln("Pass to generic in intent");
  proc receiveDomainLiteral(in d) {
    writeln(d.definedConst);
  }

  receiveDomainLiteral(makeDomainLiteral());
}

{
  writeln("Pass to concrete in intent");
  proc receiveDomainLiteral(in d: domain(1, int, false)) {
    writeln(d.definedConst);
  }

  receiveDomainLiteral(makeDomainLiteral());
}

{
  writeln("Pass to var field with default initializer");
  record R {
    var d;
    var a: [d] int;
  }

  var r = new R(makeDomainLiteral());

  writeln(r.d.definedConst);
  writeln("How many arrays are tracked? ",  +reduce for r.d._value._arrs do 1);
}

{
  writeln("Pass to const field with default initializer");
  record R {
    const d;
    var a: [d] int;
  }

  var r = new R(makeDomainLiteral());

  writeln(r.d.definedConst);
  writeln("How many arrays are tracked? ",  +reduce for r.d._value._arrs do 1);
}

