var A, B, C: [1..5] int;

for (a, b, c, i, j, k) in zip(A, B, C, 1..5, 6..10, 11..15) {
  a = i;
  b = j;
  c = k;
}

writeln((A, B, C));

for (a, b, c) in zip(A, B, C) {
  a += b + c;
}

writeln((A, B, C));

for t in zip(A, B, C) {
  t(1) += t(0) + t(2);
}

writeln((A, B, C));
