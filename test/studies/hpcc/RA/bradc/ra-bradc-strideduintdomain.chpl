config const n = 10:uint(64);

var D = {1..n};

var step = 2:uint;

for i in D by step {
  writeln("i is: ", i);
}

for i in D by 2 {
  writeln("i is: ", i);
}


for i in D by 2:uint {
  writeln("i is: ", i);
}


