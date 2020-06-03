class BinaryTree {
  type eltType;
  var data: eltType;
  var left: unmanaged BinaryTree(eltType)?;
  var right: unmanaged BinaryTree(eltType)?;

  proc deinit() {
    if left  != nil then delete left;
    if right != nil then delete right;
  }
}

//
// Old code uses a recursive iterator, which has trouble compiling currently,
// see issue #7134.
//
/*
  if left then
    for e in left!.postOrder() do
      yield e;
  if right then
    for e in right!.postOrder() do
      yield e;
  yield data;
*/
iter BinaryTree.postOrder(): eltType {
  use List;

  type Tree = unmanaged BinaryTree(eltType);

  var isDescending = true;
  var last: Tree? = nil;
  var nodes: list(Tree);

  nodes.append(this:unmanaged);

  while !nodes.isEmpty() {
    var node = nodes.pop();
    var next: Tree? = nil;

    if node.left != nil && isDescending {
      next = node.left;
    } else if node.right != nil && !isDescending && last != node.right {
      isDescending = true;
      next = node.right;
    }

    if next {
      nodes.append(node);
      nodes.append(next!);
      continue;
    }

    isDescending = false;
    last = node;
    yield node.data;
  }
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
