class Node {
  var data: int;
  var left, right: owned Node;

  proc processNode() {
    writeln(data);
  }
}

const tree = new owned Node(5,
               new owned Node(2,
                 new owned Node(1, nil, nil),
                 new owned Node(4,
                   new owned Node(3, nil, nil),
                   nil)),
               new owned Node(8,
                 new owned Node(6,
                   nil,
                   new owned Node(7, nil, nil)),
                   new owned Node(9, nil, nil)));


sync begin walkTree(tree);

proc walkTree(node) {
  node.processNode();
  if node.left != nil then
    begin walkTree(node.left);
  if node.right != nil then
    begin walkTree(node.right);
}
