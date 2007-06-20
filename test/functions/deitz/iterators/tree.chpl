class Tree {
  var data: int;
  var left, right: Tree;
}

def treeNodes(tree: Tree): Tree {
  yield tree;
  if tree.left then
    for t in treeNodes(tree.left) do
      yield t;
  if tree.right then
    for t in treeNodes(tree.right) do
      yield t;
}

var tree = Tree(1, Tree(2), Tree(3, Tree(4), Tree(5)));

for t in treeNodes(tree) do
  writeln(t.data);
