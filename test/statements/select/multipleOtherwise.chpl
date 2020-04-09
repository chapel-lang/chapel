
config const n = 10;

select n {
  when 1 do writeln("one");
  otherwise do writeln("first");
  otherwise do writeln("second");
}
