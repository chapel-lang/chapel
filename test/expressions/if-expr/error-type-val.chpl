
config param thenType = true;
config const cond : bool;

if thenType {
  var foo = if cond then int else 1;
} else {
  var foo = if cond then "hi" else string;
}
