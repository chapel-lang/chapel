proc dowork(): int {
  writeln("hello");
  return 1;
}

select dowork() {
  when 0 do writeln("zero");
  when 1 do writeln("one");
}

