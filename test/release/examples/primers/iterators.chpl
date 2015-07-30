/*
 * Iterators Primer
 *
 * This primer contains several examples of iterators:
 *   an iterator to generate the Fibonacci numbers,
 *   an iterator defined by multiple loops
 *   and a recursive iterator over a tree.
 * 
 * It also contains examples of the two kinds of parallel iteration:
 *  data-parallel (forall), and
 *  task-parallel (coforall).
 */

//
// fibonacci - generates the first n Fibonacci numbers
//
// The state of the iterator is stored in the tuple (current, next).
// Each time the iterator runs, it returns the current Fibonacci number,
// and then updates the state to the next one.
iter fibonacci(n: int) {
  var (current, next) = (0, 1);
  for 1..n {
    yield current;
      // The first time this iterator is run, it proceeds this far
      // and then yields (returns) the first value of current (== 0),
      // and its state is preserved.
      // The next time it is called, execution resumes here.
      // It continues until another yield is reached.
      // If the iterator completes or encounters a return statement,
      // execution of the enclosing loop terminates immediately.
    (current, next) = (next, current + next);
  }
}

//
// This example uses zipper iteration to iterate over the unbounded range 1..
// and the fibonacci iterator with n set to ten.
// Zipper iteration means that each iterator is advanced to the next yield
// and the two values they return are returned as a tuple.
//
writeln("Fibonacci Numbers");
for (i, j) in zip(1.., fibonacci(10)) {
  write("The ", i);
  select i {
    when 1 do write("st");
    when 2 do write("nd");
    when 3 do write("rd");
    otherwise write("th");
  }
  writeln(" Fibonacci number is ", j);
}
writeln();

//
// multiloop - generate the outer (Cartesian) product of indices in two ranges
// and yield them as tuples.
//
iter multiloop(n: int) {
  for i in 1..n do
    for j in 1..n do
      yield (i, j);
}

//
// Use writeln to output the values that the iterator generates.
//
// This is an example of promotion.
// Promotion means that a procedure containing an iterator call is repeated
// for each value the iterator returns.
//
// In this case, writeln() is called with each value returned by the multiloop()
// iterator.
//
writeln("Multiloop Tuples");
writeln(multiloop(3));
writeln(); // line break

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

var tree = new Tree("a", new Tree("b"), new Tree("c", new Tree("d"),
                                                 new Tree("e")));

//
// postorder - iterate over the Tree in postorder using recursion
//
// Each yield statement returns a node, 
// or equivalently the subtree rooted at that node.
//
iter postorder(tree: Tree): Tree {
  if tree != nil {
    // Call the iterator recursively on the left subtree and then expand
    // the result.
    for child in postorder(tree.left) do
      yield child;

    // Call the iterator recursively on the right subtree and then expand
    // the result.
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
proc Tree.writeThis(x)
{
  var first = true;
  for node in postorder(tree) {
    if first then first = false;
      else x.write(" ");
    x.write(node.data);
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
// When invoked in a forall loop (or semantically equivalent context),
// iterators that create parallel tasks and assign work to
// those tasks are required.  Such iterators are a fairly big topic
// and are described in detail in the parIters.chpl example in
// this directory.

//
// The coforall loop uses the serial version iterator 
// to spawn a separate task for each of the values it yields.
// If you use coforall, you are asserting that the manipulations
// done with each yielded value can be done in parallel 
// (i.e. in no particular order).
//
// All of the spawned tasks must complete before execution continues
// at the end of the coforall statement body.
//

// This just does something else noticeable to the tree data --
// prefixing each string with "node_".
proc decorate(s:string) return "node_" + s;

// 
// This decorates each node in the tree in parallel, using a coforall.
// Then it writes out the resulting tree data using a postorder traversal.
//
writeln("Task parallel iteration");
coforall node in postorder(tree) do
  node.data = decorate(node.data);
writeln(tree);
writeln();
