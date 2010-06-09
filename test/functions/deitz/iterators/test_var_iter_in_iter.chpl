var global = 1;

def g_iter() var {
  yield global;
}

def f_iter() {
  for g_index in g_iter() do
    g_index = 3;
  yield 2;
}

def main {
  writeln(global);
  for f_index in f_iter() do
    writeln(f_index);
  writeln(global);
}
