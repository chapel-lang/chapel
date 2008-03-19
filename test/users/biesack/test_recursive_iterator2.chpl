enum NodeType { value, branch, none };
class Node {
  def nodeType() : NodeType { return NodeType.none; }
}
class ValueNode : Node {
  const val : int;
  def nodeType() : NodeType { return NodeType.value; }
}
class Branch : Node {
  const left, right: Node;
  def nodeType() : NodeType { return NodeType.branch; }
}

def node(a:int, b:int) : Branch {
  return new Branch(new ValueNode(a), new ValueNode(b));
}
def node(a, b:Node) : Branch {
  return new Branch(a, b);
}

def leaves(tree : Node) : int {
  select tree.nodeType() {
    when NodeType.value do {
      yield (tree:ValueNode).val;
    }
    when NodeType.branch do {
      const node = tree:Branch;
      if (node.left != nil) {
        for leaf in leaves(node.left) yield leaf;
      }
      if (node.right != nil) {
        for leaf in leaves(node.right) yield leaf;
      }
    }
    otherwise;
  }
}

def main() {
  var leaf0 = node(0,1), leaf1 = node(2,3), leaf2 = node(4,5), leaf3 = node(6,7),
      node0 = node(leaf0, leaf1), node1 = node(leaf3, leaf3),
      root = node(node0, node1);
  for leaf in leaves(root) {
    if (leaf > 0) then
      writeln("Leaf NodeType.value: ", leaf);
  }
}
