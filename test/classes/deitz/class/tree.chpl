class BinaryTree {
  type eltType;
  var data: eltType;
  var left: unmanaged BinaryTree(eltType);
  var right: unmanaged BinaryTree(eltType);

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
  new unmanaged BinaryTree(int, 1,
    new unmanaged BinaryTree(int, 2,
      new unmanaged BinaryTree(int, 3),
      new unmanaged BinaryTree(int, 4)),
    new unmanaged BinaryTree(int, 5,
      new unmanaged BinaryTree(int, 6),
      new unmanaged BinaryTree(int, 7)));

writeln(bt.postOrder());

delete bt;
