def rangewrite(r: range(?)) {
  for i in r do
    write(i, " ");
  writeln();
}

def main {
  rangewrite(..10 # 3);
  rangewrite(..10 # 3 by 2);
  rangewrite(..10 # 3 by -2);
  rangewrite(0.. # 3);
  rangewrite(0.. # 3 by 2);
  rangewrite(0.. # 3 by -2);
  rangewrite(..10 by -2 # 3);
  rangewrite(0.. by 2 # 3);
  rangewrite(1..10 # 3);
  rangewrite(1..10 by -1 # 3);

  rangewrite(0.. by -2 # 3);  // 4 2 0
  rangewrite(0.. by 2 # 3);   // 0 2 4
  rangewrite(..10 by 2 # 3);  // 6 8 10
  rangewrite(..10 by -2 # 3); // 10 8 6

}
