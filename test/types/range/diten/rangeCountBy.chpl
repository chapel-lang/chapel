proc rangewrite(r: range(?)) {
  for i in r do
    write(i, " ");
  writeln();
}

proc main {
// These are no longer legal.  hilde 2011-03-11.
//  rangewrite(..10 # 3);  
//  rangewrite(..10 # 3 by 2);
//  rangewrite(..10 # 3 by -2);

  rangewrite(..10 # -3);
  rangewrite(..10 # -3 by 2);
  rangewrite(..10 # -3 by -2);

  rangewrite(0.. # 3);
  rangewrite(0.. # 3 by 2);
  rangewrite(0.. # 3 by -2);

  rangewrite(..10 by -2 # 3);
  rangewrite(0.. by 2 # 3);
  rangewrite(1..10 # 3);
  rangewrite(1..10 by -1 # 3);

// The commented ones are no longer legal.
//  rangewrite(0.. by -2 # 3);  // 4 2 0
//UEB  rangewrite(0.. by -2 # -3);  // 4 2 0
  rangewrite(0.. by 2 # 3);   // 0 2 4
//  rangewrite(..10 by 2 # 3);  // 6 8 10
//UEB  rangewrite(..10 by 2 # -3);  // 6 8 10
  rangewrite(..10 by -2 # 3); // 10 8 6
}
