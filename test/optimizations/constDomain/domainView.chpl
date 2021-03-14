//rank change
{
  var d = {1..10, 1..10};

  const domView = d[2..4, 3];

  writeln(domView.definedConst);
  writeln(domView._value.definedConst);
}

//slice
{
  var d = {1..10, 1..10};

  const domView = d[2..4, 2..4];

  writeln(domView.definedConst);
  writeln(domView._value.definedConst);
}
