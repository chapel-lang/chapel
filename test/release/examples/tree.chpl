/*
 * This test constructs a binary tree in parallel and assigns a unique
 * id to each node.  It then computes the sum of the id's in parallel.
 *
 */

//
// configuration constant indicating the height of the tree to
// construct.  Override using --treeHeight=<height>.
//
config const treeHeight: uint = 4;

class node {
  var id: int;
  var left, right: node;
}

def buildTree(height: uint, id: int): node {
  var newNode = node(id);

  if height > 1 {
    cobegin {
      newNode.left  = buildTree(height-1, id + 1);
      newNode.right = buildTree(height-1, id + (1 << (height-1)));
    }
  }

  return newNode;
}


def sum(n: node): int {
  var total = n.id;

  if n.left != nil {
    var sumLeft, sumRight: int;
    cobegin {
      sumLeft = sum(n.left);
      sumRight = sum(n.right);
    }
    total += (sumLeft + sumRight);
  }
  return total;
}


var root = buildTree(treeHeight, 1);
writeln("sum=", sum(root));
