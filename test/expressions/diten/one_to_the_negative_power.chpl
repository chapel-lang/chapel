proc f(a,b) {
  return a**b;
}

writeln((f(1,-1), f(-1,-1), f(-1,-2)));
writeln((1**-1, (-1)**-1, (-1)**-2));
var neg1 = -1, neg2 = -2;

writeln((1**neg1, (-1)**neg1, (-1)**neg2));
