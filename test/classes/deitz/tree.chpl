class BinaryTree {
  type eltType;
  var data: eltType;
  var left: BinaryTree(eltType);
  var right: BinaryTree(eltType);
}

def BinaryTree.postOrder(): eltType {
  if left then
    for e in left.postOrder() do
      yield e;
  if right then
    for e in right.postOrder() do
      yield e;
  yield data;
}

var bt =
  BinaryTree(int, 1,
    BinaryTree(int, 2,
      BinaryTree(int, 3),
      BinaryTree(int, 4)),
    BinaryTree(int, 5,
      BinaryTree(int, 6),
      BinaryTree(int, 7)));

writeln(bt.postOrder());
