// Construct a binary tree in parallel and assign a unique id to each node.
// Compute the sum of the id's. Assume height >= 1.

param treeHeight = 4;

class node {
  var id: int;
  var left, right: node;
}

def buildTree(height: uint, id: int): node {
  var newNode = node();
  newNode.id = id;
  if height > 1 {
    cobegin {
      newNode.left = buildTree(height-1, id+1);
      newNode.right = buildTree(height-1, id+(1<<(height-1)));
    }
  }
  return newNode;
}


def sum(n: node): int {
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


var root = buildTree(treeHeight, 1);
writeln("sum=", sum(root));
