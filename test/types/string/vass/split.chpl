
proc test(s:string) {
  write(s, ": ");
  for w in s.split() do
    write(w, "; ");
  writeln();
}

test("apples grapes");
test("apples g");
test("smoke smoke s");

for part1 in ("", " ", "  ") do
  for part2 in ("a", "aa",) do
    for part3 in (" ", "  ") do
      for part4 in ("g", "gg") do
        for part5 in ("", " ", "  ") do
          test(part1+part2+part3+part4+part5);
