proc split(d: domain) where d.rank == 1 {
  const len = d.size;
  const r = d.dim(1);
  const d1 = d#(len/2);  // take the first floor(len/2) elements
  const d2 = d#-((len+1)/2); // take the remaining ceil(len/2) elements
  
  writeln("d is: ", d);
  writeln("split into: ", d1, " and ", d2);
  writeln("------");
}

split({1..10});
split({1..9});
split({1..10 by 2});
split({1..12 by 2});
split({1..10 by -2});
