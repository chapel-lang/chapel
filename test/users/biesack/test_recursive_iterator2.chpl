enum NodeType { value, branch, noNode };

class Node {
  proc nodeType() : NodeType { return NodeType.noNode; }
}

class ValueNode : Node {
  const val : int;
  override proc nodeType() : NodeType { return NodeType.value; }
}

class Branch : Node {
  const left, right: unmanaged Node;
  override proc nodeType() : NodeType { return NodeType.branch; }
}

proc node(a:int, b:int) : unmanaged Branch {
  return new unmanaged Branch(new unmanaged ValueNode(a), new unmanaged ValueNode(b));
}

proc node(a, b:unmanaged Node) : unmanaged Branch {
  return new unmanaged Branch(a, b);
}

iter leaves(tree : unmanaged Node) : int {
  select tree.nodeType() {
    when NodeType.value do {
      yield (tree:unmanaged ValueNode).val;
    }

    when NodeType.branch do {
      const node = tree:unmanaged Branch;

      if (node.left != nil) {
        for leaf in leaves(node.left) do yield leaf;
      }

      if (node.right != nil) {
        for leaf in leaves(node.right) do yield leaf;
      }
    }
    otherwise;
  }
}

proc main() {
  var leaf0 = node(0,1), leaf1 = node(2,3), leaf2 = node(4,5), leaf3 = node(6,7),
      node0 = node(leaf0, leaf1), node1 = node(leaf3, leaf3),
      root  = node(node0, node1);

  for leaf in leaves(root) {
    if (leaf > 0) then
      writeln("Leaf NodeType.value: ", leaf);
  }


  delete root;

  delete node1;
  delete node0;

  delete leaf3.left;
  delete leaf3.right;
  delete leaf3;

  delete leaf2.left;
  delete leaf2.right;
  delete leaf2;

  delete leaf1.left;
  delete leaf1.right;
  delete leaf1;

  delete leaf0.left;
  delete leaf0.right;
  delete leaf0;
}
