import Sort;
{
  var D1 : domain(int) = { 10..30 };
  var D2 : domain(int) = { 20..30 };
  var D3 : domain(int) = D1 - D2;
  writeln(Sort.sorted(D1));
  writeln(Sort.sorted(D2));
  writeln(Sort.sorted(D3));
}

{
  var D1 : domain(int) = { 1..10 };
  var D3 : domain(int) = D1 - 5;
  writeln(Sort.sorted(D1));
  writeln(Sort.sorted(D3));
}
