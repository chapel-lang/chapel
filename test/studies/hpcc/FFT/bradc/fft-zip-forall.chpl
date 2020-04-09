var k = 0;
forall (i,j) in (1..8, 2..10) { // unequal yields error message?
  k +== 1;
}
writeln(k);
