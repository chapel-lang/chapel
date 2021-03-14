// Abstracted from iterators.chp. in the primers directory.
// The important line is marked with ### below. <hilde>
//
// define a tree class and initialize an instance to
//
//      a
//     / \ 
//    b   c
//       / \
//      d   e
//
class Tree {
  var data: string;
  var left, right: Tree;
}

var tree = new Tree("a", new Tree("b"), new Tree("c", new Tree("d"), new Tree("e")));

//
// postorder - iterate over the Tree in postorder using recursion
//
// Each yield statement returns a node, 
// or equivalently the subtree rooted at that node.
//
iter postorder(tree: Tree): Tree {
  if tree != nil {
    // Call the iterator recursively on the left subtree and then expand the result.
    for child in postorder(tree.left) do
      yield child;
    // Call the iterator recursively on the right subtree and then expand the result.
    for child in postorder(tree.right) do
      yield child;
    // Finally, yield the node itself.
    yield tree;
  }
}

//
// This visits the nodes of the tree in postorder and prints them out.
// It uses the "first" flag to avoid printing a leading space.
//
proc Tree.writeThis(x: Writer)
{
  var first = true;
  for node in postorder(tree) {
    if first then first = false;
      else " ".writeThis(x);
    node.data.writeThis(x);
  }
}
  
//
// Output the data in the tree using the postorder iterator.
//
writeln("Tree Data");
writeln(tree);
writeln();


//
// Iterators get more interesting in a parallel context.
// When invoked in a forall statement or forall expression,
// the iterator may yield up several values which are used simultaneously.
//
// The forall statement supports data parallelism.
// In the current implementation, leader and follower iterators must be supplied
// explicitly to support parallel iteration.  This requirement may be removed in
// future versions. 
//

//
// This is the leader, it orchestrates how the iteration task is divided up.
// The follower is run with each chunk it yields.
//
iter postorder(param tag: iterKind, tree : Tree): Tree 
  where tag == iterKind.leader
{
  if tree == nil then return;

  // This leader just returns the whole tree as a chunk. (Very boring.)
  yield tree;
}

//
// This is the follower.
// It performs the fine-grained execution under control of the leader.
// The follower is called once with each chunk returned by the leader.
// The chunk contains whatever was yielded by the leader (in this case, a subtree).
// The last parameter provides global context which the follower may require.
//
//### Allow the follower state to be called anything, such as "chunk". ###
iter postorder(param tag: iterKind, chunk, tree: Tree)
  where tag == iterKind.follower
{
  // The follower does normal postorder traversal on each chunk.
  for node in postorder(chunk) do yield node;
}

// Do something noticeable with a string, like repeating it twice.
proc echo(s:string) return s + s;

writeln("Data parallel iteration");

// This doubles the data in the tree.
// The nodes of the tree can be visited in any order.
forall node in postorder(tree) do
  node.data = echo(node.data);

writeln(tree);
writeln();

