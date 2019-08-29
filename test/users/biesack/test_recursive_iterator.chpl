module missing_return_bug {
  class Node { }

  class ValueNode : Node { const value : int; }

  class Branch : Node { const left, right : unmanaged Node; }

  proc node(a : int, b : int) : unmanaged Branch {
    return new unmanaged Branch(new unmanaged ValueNode(a), new unmanaged ValueNode(b));
  }

  proc node(a, b : unmanaged Node) : unmanaged Branch {
    return new unmanaged Branch(a, b);
  }

  iter leaves(tree : unmanaged Node) : int {
    select tree.type {
      when unmanaged ValueNode do {
        yield tree.value;
      }

      when unmanaged Branch do {
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

  proc main() {
    var leaf0 = node(0,1), leaf2 = node(2,3), leaf3 = node(4,5),
        node0 = node(leaf0, leaf2), node1 = node(node0, leaf3),
        root  = node(node0, node1);

    writeln(root);

    for leaf in leaves(root) {
      writeln("Leaf value: ", leaf);
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

    delete leaf0.left;
    delete leaf0.right;
    delete leaf0;
  }
}

