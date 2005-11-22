for i in 1..10 {
  for j in 1..5 {
    writeln(i, " and ", j);
    if j == 3 then
      break;
  }
  if i == 5 then
    break;
}
