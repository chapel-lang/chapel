class Tree {
  var data: int;
  var left: owned Tree?;
  var right: owned Tree?;
  
  iter these(): int {
    if left then
      for e in left! do yield e;
    yield data;
    if right then
      for e in right! do yield e;
  }
}

var tree = new Tree(40, new Tree(20, new Tree(10), new Tree(30)),
                        new Tree(60, new Tree(50), new Tree(70)));

for e in tree do
  writeln(e);
