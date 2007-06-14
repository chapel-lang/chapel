/*
 * Iterators Example
 *
 * This example contains several sample iterators:
 *   an iterator to generate the Fibonacci numbers
 *   an iterator defined by multiple loops
 *   a recursive iterator over a tree
 *
 */

//
// fibonacci - generates the first n Fibonacci numbers
//
iterator fibonacci(n: int) {
  var (current, next) = (0, 1); // equiv. var current = 0, next = 1;
  for 1..n {
    yield current;
    (current, next) = (next, current + next);
  }
}

//
// use zipper iteration to iterate over the counting numbers (1..) and
// the fibonacci iterator with n set to ten, and write out the numbers
//
writeln("Fibonacci Numbers");
for (i, j) in (1.., fibonacci(10)) do
  writeln(i, ": ", j);

writeln(); // line break

//
// multiloop - generate the cross product of indices in two ranges and
// yield them as tuples
//
iterator multiloop(n: int) {
  for i in 1..n do
    for j in 1..n do
      yield (i, j);
}

//
// use writeln to output the values that the iterator generates
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

var tree = Tree("a", Tree("b"), Tree("c", Tree("d"), Tree("e")));

//
// postorder - iterate over the Tree in postorder using recursion
//
iterator postorder(tree: Tree): string {
  if tree != nil {
    for child in postorder(tree.left) do
      yield child;
    for child in postorder(tree.right) do
      yield child;
    yield tree.data;
  }
}

//
// output the data in the tree using the postorder iterator
//
writeln("Tree Data");
writeln(postorder(tree));
