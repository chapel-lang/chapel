var global = 1;

iter g_iter() ref {
  yield global;
}

iter f_iter() {
  for g_index in g_iter() do
    g_index = 3;
  yield 2;
}

proc main {
  writeln(global);
  for f_index in f_iter() do
    writeln(f_index);
  writeln(global);
}
