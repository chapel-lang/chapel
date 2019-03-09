class Tree {
  var data: int;
  var left: owned Tree;
  var right: owned Tree;
  
  iter these(): int {
    if left then
      for e in left.these() do yield e;
    yield data;
    if right then
      for e in right.these() do yield e;
  }
}

var tree = new owned Tree(40, new owned Tree(20, new owned Tree(10), new owned Tree(30)), new owned Tree(60, new owned Tree(50), new owned Tree(70)));

for e in tree do
  writeln(e);
