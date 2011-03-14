var count: int;

cobegin {
  atomic count += 1;
  atomic count += 1;
  atomic count += 1;
  atomic count += 1;
}

sync {
  begin atomic {count += 1; }
  begin atomic {count += 1; }
  begin atomic {count += 1; }
}

writeln("Final Value: ", count);

