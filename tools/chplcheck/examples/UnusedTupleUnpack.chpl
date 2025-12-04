/*
  Tuple unpacking should not have all unused elements like '(_, _)'.
*/

// This unpacks a tuple but ignores all elements which violates the rule.
var tuple = (1, 2);
var (_, _) = tuple;

// This unpacks a tuple and uses at least one element which is correct.
var (a, _) = tuple;
writeln(a);
