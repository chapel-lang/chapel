{
  writeln("Slice with tuple of ranges");
  var d = {1..10, 1..10};
  var a: [d] int;

  var arrView = a[2..3, 2..3];

  writeln(arrView.domain);
  writeln(arrView.domain._value.definedConst);
  writeln(arrView.domain.definedConst);
}

{
  writeln("Slice with domain literal");
  var d = {1..10, 1..10};
  var a: [d] int;

  var arrView = a[{2..3, 2..3}];

  writeln(arrView.domain._value.definedConst);
  writeln(arrView.domain.definedConst);
}

{
  writeln("Slice with var domain");
  var d = {1..10, 1..10};
  var a: [d] int;

  var sliceDom = {2..3, 2..3};
  var arrView = a[sliceDom];

  writeln(arrView.domain._value.definedConst);
  writeln(arrView.domain.definedConst);
}

{
  writeln("Reindex with tuple of ranges");
  var d = {1..10, 1..10};
  var a: [d] int;

  var arrView = a.reindex(2..11, 2..11);

  writeln(arrView.domain._value.definedConst);
  writeln(arrView.domain.definedConst);
}

{
  writeln("Reindex with domain literal");
  var d = {1..10, 1..10};
  var a: [d] int;

  var arrView = a.reindex({2..11, 2..11});

  writeln(arrView.domain._value.definedConst);
  writeln(arrView.domain.definedConst);
}

{
  writeln("Reindex with var domain");
  var d = {1..10, 1..10};
  var a: [d] int;

  var reindexDom = {2..11, 2..11};
  var arrView = a.reindex(reindexDom);

  writeln(arrView.domain._value.definedConst);
  writeln(arrView.domain.definedConst);
}

{
  writeln("Rank change with tuple of ranges");
  var d = {1..10, 1..10};
  var a: [d] int;

  var arrView = a[2..3, 2];

  writeln(arrView.domain._value.definedConst);
  writeln(arrView.domain.definedConst);
}
