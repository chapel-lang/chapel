class Tree {
  var data: int;
  var left, right: unmanaged Tree?;

  proc deinit() {
    if left  != nil then delete left;
    if right != nil then delete right;
  }
}

iter treeNodes(tree: unmanaged Tree): unmanaged Tree {
  yield tree;
  if tree.left then
    for t in treeNodes(tree.left!) do
      yield t;
  if tree.right then
    for t in treeNodes(tree.right!) do
      yield t;
}

var tree = new unmanaged Tree(1, new unmanaged Tree(2), new unmanaged Tree(3, new unmanaged Tree(4), new unmanaged Tree(5)));

for t in treeNodes(tree) do
  writeln(t.data);

delete tree;
