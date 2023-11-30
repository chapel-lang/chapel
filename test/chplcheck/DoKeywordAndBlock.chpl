module DoKeywordAndBlock {
  for i in 1..10 do {
    writeln(i);
  }

  for i in 1..10 do writeln(i);

  for i in 1..10 {
    writeln(i);
  }
}