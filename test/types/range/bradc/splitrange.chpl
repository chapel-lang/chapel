proc split(r: range(?)) {
  const len = r.size;
  const r1 = r#(len/2);  // take the first floor(len/2) elements
  const r2 = r#-((len+1)/2); // take the remaining ceil(len/2) elements
  
  writeln("r is: ", r);
  writeln("split into: ", r1, " and ", r2);
  writeln("------");
}

split(1..10);
split(1..9);
split(1..10 by 2);
split(1..12 by 2);
split(1..10 by -2);
