def foo(a, (, b, ), c, (d, (e, ))) {
  writeln((a, b, c, d, e));
}

def main() {
  foo(1, (2, 3, 4), 5, (6, (7, 8)));
}
