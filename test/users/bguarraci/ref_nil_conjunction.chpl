class Node {
  var left: Node;
  var right: Node;
}

var node = new Node();
if (node.left) then writeln("left child is not nil");
if (node.right) then writeln("right child is not nil");
if (node.left && node.right) then writeln("both children are not nil");
