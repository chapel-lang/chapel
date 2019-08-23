use List;

config type listType = int;
config param listLock = true;
config const testIters = 16;

var lst = new list(listType, listLock);

writeln(lst);

try {
  lst.pop();
} catch e {
  writeln(e);
}

for i in 1..testIters do
  lst.append(i);

writeln(lst);

for i in 1..lst.size {
  var x = lst.pop(1);
  lst.append(x);
}

writeln(lst);


