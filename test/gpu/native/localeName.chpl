writeln(here.name); // why empty?
writeln(here.gpus[0]);
on here.gpus[0] {
  writeln(here.name);
}
