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
  const D1: domain(rank) = init_range;
  var D2: domain(rank) = init_range;
  checkit(D1, D2);

  init_range(rank/2) = 1..1;
  D2 = init_range;
  checkit(D1, D2);
}

{
  var init_range: rank*range(stridable=true);
  for param i in 0..rank-1 do
    init_range(i) = 1..n*i by s;
  const D1: domain(rank, stridable=true) = init_range;
  var D2: domain(rank, stridable=true) = init_range;
  checkit(D1, D2);

  init_range(rank/2) = 1..1;
  D2 = init_range;
  checkit(D1, D2);
}

{
  var init_range: rank*range(stridable=true);
  for param i in 0..rank-1 do
    init_range(i) = 1..n*i by -s;
  const D1: domain(rank, stridable=true) = init_range;
  var D2: domain(rank, stridable=true) = init_range;
  checkit(D1, D2);

  init_range(rank/2) = 1..1;
  D2 = init_range;
  checkit(D1, D2);
}

