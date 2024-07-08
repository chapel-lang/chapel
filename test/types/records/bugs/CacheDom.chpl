use Map;

type domType = {0..0}.type;
var tab: map(int, domType);

proc cachedDom(size: int) const ref {
  if !tab.contains(size) {
    tab.add(size, {0..<size});
  }
  return tab[size];
}

proc main() {
  var d = cachedDom(10);
  writeln(d);
}

