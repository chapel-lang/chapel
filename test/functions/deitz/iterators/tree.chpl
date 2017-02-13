class Tree {
  var data: int;
  var left, right: Tree;

  proc deinit() {
    if left  != nil then delete left;
    if right != nil then delete right;
  }
}

iter treeNodes(tree: Tree): Tree {
  yield tree;
  if tree.left then
    for t in treeNodes(tree.left) do
      yield t;
  if tree.right then
    for t in treeNodes(tree.right) do
      yield t;
}

var tree = new Tree(1, new Tree(2), new Tree(3, new Tree(4), new Tree(5)));

for t in treeNodes(tree) do
  writeln(t.data);

delete tree;
