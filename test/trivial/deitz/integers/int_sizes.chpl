proc test_int_size(in i) {
  i -= 1;
  write(i, " ");
  i += 1;
  write(i, " ");
  i += 1;
  writeln(i);
}

test_int_size(max(int(8)));
test_int_size(max(int(16)));
test_int_size(max(int(32)));
test_int_size(max(int(64)));

test_int_size(max(uint(8)));
test_int_size(max(uint(16)));
test_int_size(max(uint(32)));
test_int_size(max(uint(64)));
