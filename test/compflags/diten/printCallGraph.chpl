proc e(n) {
  if n > 0 {
    f(n-1);
  } else if n < 0 {
    g(n+1);
  } else {
    h(n);
  }
}

proc f(n) {
  if n > 0 {
    e(n-1);
  } else if n < 0 {
    e(n+1);
  } else {
    h(n);
  }
}

proc g(n) {
  if n > 0 {
    f(n-1);
  } else if n < 0 {
    f(n+1);
  } else {
    h(n);
  }
}

proc h(n) {
  writeln("H");
}

e(4);
e(-4);
