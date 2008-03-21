module missing_return_bug {
  class Node { }
  class ValueNode : Node { const value : int; }
  class Branch : Node { const left, right : Node; }
  def node(a : int, b : int) : Branch {
    return new Branch(new ValueNode(a), new ValueNode(b));
  }
  def node(a, b : Node) : Branch {
    return new Branch(a, b);
  }
  def leaves(tree : Node) : int {
    type select tree {
      when ValueNode do {
        yield tree.value;
      }
      when Branch do {
        if (tree.left != nil) {
          for leaf in leaves(tree.left) do yield leaf;
        }
        if (tree.right != nil) {
          for leaf in leaves(tree.right) do yield leaf;
        }
      }
      otherwise;
    }
    // uncomment the following to fix the compiler error
    // return -1;
  }
  def main() {
    var leaf0 = node(0,1), leaf2 = node(2,3), leaf3 = node(4,5),
        node0 = node(leaf0, leaf2), node1 = node(node0, leaf3),
        root = node(node0, node1);
    writeln(root);
    for leaf in leaves(root) {
      writeln("Leaf value: ", leaf);
    }
  }
}

