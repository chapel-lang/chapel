use Map;

// doing the same for a local map causes things to go off the rails on locale 1
proc main() {
  var m: map(int, string);
  m.add(1, "one");

  for k in m.keys() do
    writeln(k);

  for v in m.values() do
    writeln(v);

  on Locales.last {
    for k in m.keys() do
      writeln(k);

    for v in m.values() do
      writeln(v);
  }
}
