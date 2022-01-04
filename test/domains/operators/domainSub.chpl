{
  var D1 : domain(int) = { 10..30 };
  var D2 : domain(int) = { 20..30 };
  var D3 : domain(int) = D1 - D2;
  writeln(D1.sorted());
  writeln(D2.sorted());
  writeln(D3.sorted());
}

{
  var D1 : domain(int) = { 1..10 };
  var D3 : domain(int) = D1 - 5;
  writeln(D1.sorted());
  writeln(D3.sorted());
}
