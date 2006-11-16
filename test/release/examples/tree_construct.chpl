// Construct a binary tree and assign a unique id to each node.
// Compute the sum of the id's. Assume height >= 1.

param TREE_HEIGHT = 4;

class node {
  var id: int;
  var left, right: node;
}

def build_tree( height: uint, id: int): node {
  var new_node = node();
  new_node.id = id;
  if height > 1 {
    cobegin {
      new_node.left = build_tree( height-1, id+1);
      new_node.right = build_tree( height-1, id+(1<<(height-1)));
    }
  }
  return new_node;
}


def sum( n: node): int {
  if n.left != nil {
    var sum_left, sum_right: int;
    cobegin {
      sum_left = sum(n.left);
      sum_right = sum(n.right);
    }
    return n.id + sum_left + sum_right;
  } else {
    return n.id;
  }
}


var root = build_tree( TREE_HEIGHT, 1);
writeln( "sum=", sum(root));
