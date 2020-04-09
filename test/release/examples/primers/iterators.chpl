// Iterators

/*
   This primer contains several examples of iterators:

     * an iterator to generate the Fibonacci numbers,
     * an iterator defined by multiple loops
     * and a recursive iterator over a tree.

   It also contains examples of the two kinds of parallel iteration:

     * data-parallel (``forall``), and
     * task-parallel (``coforall``).
*/

/*
.. primers-iterators-fibonacci

fibonacci
---------
*/

// Generates the first ``n`` Fibonacci numbers.
//
// The state of this iterator is stored in the tuple ``(current, next)``.
// Each time the yield statement is reached, it yields, or generates,
// the current Fibonacci number. It then updates the state to the next one.
//
iter fibonacci(n: int) {
  var (current, next) = (0, 1);
  for 1..n {
    yield current;
    (current, next) = (next, current + next);
  }
}

//
// An iterator is typically invoked in a loop. Whenever an iterator's yield
// statement is executed, the loop's index variable is initialized with the
// yielded value and the loop body is executed for a single iteration before
// returning to the iterator.
//
// When the iterator completes or encounters a return statement,
// the loop terminates.
//
write("The first few Fibonacci numbers are: ");

for indexVar in fibonacci(10) do
  write(indexVar, ", ");

writeln("...");
writeln();

//
// This example uses zipper iteration to iterate over the unbounded range ``1..``
// and the fibonacci iterator with ``n`` set to ten. Ranges, as well as arrays
// and domains, can be used as iterators in loops.
//
// Zipper iteration means that each iterator is advanced to its next yield
// and the two yielded values are combined into a tuple.
//
// A zippered loop can have a single index variable, which will be a tuple,
// or a tuple of variables like ``(i, j)``, each of which is initialized
// with the value yielded by the corresponding iterator.
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

/*
.. primers-iterators-multiloop

multiloop
---------
*/

// Generate the outer (Cartesian) product of indices in two ranges
// and yield them as tuples.
//
iter multiloop(n: int) {
  for i in 1..n do
    for j in 1..n do
      yield (i, j);
}

//
// Below is an example of promotion.
// In this case, promotion means that a procedure which normally takes a
// single argument will be repeated for each value that the iterator returns.
//
// In this case, ``writeln()`` is called with each value returned by the
// ``multiloop()`` iterator.
//
writeln("Multiloop Tuples");
writeln(multiloop(3));
writeln();

/*
.. primers-iterators-postorder

postorder
---------
*/

// Iterate over ``Tree`` in postorder using recursion.
//
// Each yield statement returns a node, or equivalently the subtree
// rooted at that node.
//
class Tree {
  var data: string;
  var left, right: owned Tree?;
}

iter postorder(tree: borrowed Tree?): borrowed Tree {
  if tree {
    if tree!.left {
      // Call the iterator recursively on the left subtree and expand.
      for child in postorder(tree!.left) do
        yield child;
    }

    if tree!.right {
      // Call the iterator recursively on the right subtree and expand.
      for child in postorder(tree!.right) do
        yield child;
    }

    // Finally, yield the node itself.
    yield tree!;
  }
}

/* Initialize a ``Tree`` instance to:

   .. code-block:: text

        a
       / \
      b   c
         / \
        d   e
*/
var tree = new Tree( "a",
  new Tree("b"),
  new Tree("c",
    new Tree("d"),
    new Tree("e")));

//
// This method uses the postorder iterator to print out each node.
// It uses the "first" flag to avoid printing a leading space.
//
proc Tree.writeThis(x)
{
  var first = true;

  for node in postorder(this) {
    if first then
      first = false;
    else
      x.write(" ");

    x.write(node.data);
  }
}

//
// Output the data in the tree using the postorder iterator.
//
writeln("Tree Data");
writeln(tree);
writeln();

/*
.. primers-iterators-parallel

Iterators in parallel
---------------------
*/

// When invoked in a forall loop (or semantically equivalent context),
// the iterator is required to create parallel tasks and assign work to
// them.  Such iterators are a fairly big topic and are described in
// detail in the :ref:`Parallel Iterators primer <primers-parIters>`.
//
// The coforall loop uses the serial iterator to spawn a separate task
// for each of the values it yields. If you use coforall, you are asserting
// that the manipulations done with each yielded value can be done in parallel.
// All of the spawned tasks will complete before execution continues
// at the end of the coforall statement body.
//
// This code decorates each node in the tree in parallel, using a coforall.
// Then it writes out the resulting tree data using a postorder traversal.
//
proc decorate(s:string) return "node_" + s;

writeln("Task parallel iteration");

coforall node in postorder(tree) do
  node.data = decorate(node.data);

writeln(tree);
writeln();
