proc myiter() {
  for i in 1..6 {
    yield;
  }
} 

for i in myiter() do
  writeln(i);
