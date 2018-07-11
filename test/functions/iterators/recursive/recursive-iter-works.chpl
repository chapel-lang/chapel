class Tree {
  var data: int;
  var left: unmanaged Tree;
  var right: unmanaged Tree;
  
  iter these(): int {
    if left then
      for e in left.these() do yield e;
    yield data;
    if right then
      for e in right.these() do yield e;
  }
}

var tree = new unmanaged Tree(40, new unmanaged Tree(20, new unmanaged Tree(10), new unmanaged Tree(30)), new unmanaged Tree(60, new unmanaged Tree(50), new unmanaged Tree(70)));

for e in tree do
  writeln(e);
