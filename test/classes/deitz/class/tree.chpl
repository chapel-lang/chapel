class BinaryTree {
  type eltType;
  var data: eltType;
  var left: BinaryTree(eltType);
  var right: BinaryTree(eltType);

  proc deinit() {
    if left  != nil then delete left;
    if right != nil then delete right;
  }
}

iter BinaryTree.postOrder(): eltType {
  if left then
    for e in left.postOrder() do
      yield e;
  if right then
    for e in right.postOrder() do
      yield e;
  yield data;
}

var bt =
  new BinaryTree(int, 1,
    new BinaryTree(int, 2,
      new BinaryTree(int, 3),
      new BinaryTree(int, 4)),
    new BinaryTree(int, 5,
      new BinaryTree(int, 6),
      new BinaryTree(int, 7)));

writeln(bt.postOrder());

delete bt;
