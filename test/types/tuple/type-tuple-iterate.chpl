type tup = (real, string, int);

for type t in tup {
  writeln(t:string);
}

/*
// The loop above could translate into:
for param i in 1..tup.size {
  writeln(tup(i):string);
}
*/
