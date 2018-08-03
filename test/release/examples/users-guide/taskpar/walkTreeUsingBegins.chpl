class Node {
  var data: int;
  var left, right: unmanaged Node;

  proc processNode() {
    writeln(data);
  }
}

const tree = new unmanaged Node(5,
               new unmanaged Node(2,
                 new unmanaged Node(1, nil, nil),
                 new unmanaged Node(4,
                   new unmanaged Node(3, nil, nil),
                   nil)),
               new unmanaged Node(8,
                 new unmanaged Node(6,
                   nil,
                   new unmanaged Node(7, nil, nil)),
                   new unmanaged Node(9, nil, nil)));


sync begin walkTree(tree);

proc walkTree(node) {
  node.processNode();
  if node.left != nil then
    begin walkTree(node.left);
  if node.right != nil then
    begin walkTree(node.right);
}

proc Node.deinit() {
  if this.left != nil then
    delete this.left;
  if this.right != nil then
    delete this.right;
}

delete tree;
