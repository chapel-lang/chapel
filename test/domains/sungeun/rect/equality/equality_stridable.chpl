config const debug = false;
config param rank = 4;
config const n = 17;
config const s = 3;

proc checkit(d1, d2) {
  if debug {
    writeln(d1);
    writeln(d2);
  }
  writeln(d1==d2);
  writeln(d1!=d2);
}

{
  var init_range: rank*range;
  for param i in 0..rank-1 do
    init_range(i) = 1..n*i;
  const D1: domain(rank, stridable=true) = init_range;
  const D2: domain(rank, stridable=false) = init_range;
  checkit(D1, D2);
}
