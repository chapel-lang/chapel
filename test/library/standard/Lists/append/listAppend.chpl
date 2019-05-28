use Lists;

config type listType = int;
config const stop = 64;

var lst = new List(listType, false);

for i in 1..stop do {
  var x = i:listType;
  lst.append(x);
}

writeln(lst);
