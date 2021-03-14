var count = 0;

label outerLoop for i in 1..4 {
  for j in 1..4 {
    count += 1;

    if count > 100 then
      halt("infinite loop");

    writeln((i,j));

    if j == 2 then
      continue outerLoop;
  }
}
