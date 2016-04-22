class Node {
  var data: int;
  var left, right: Node;

  proc ~Node() {
    if left  then delete left;
    if right then delete right;
  }

  proc processNode() {
    writeln(data);
  }
}

const tree = new Node(5,
               new Node(2,
                 new Node(1, nil, nil),
                 new Node(4,
                   new Node(3, nil, nil),
                   nil)),
               new Node(8,
                 new Node(6,
                   nil,
                   new Node(7, nil, nil)),
                   new Node(9, nil, nil)));


begin walkTree(tree);

delete tree;

proc walkTree(node) {
  node.processNode();

  if node.left != nil then
    begin walkTree(node.left);

  if node.right != nil then
    begin walkTree(node.right);
}

