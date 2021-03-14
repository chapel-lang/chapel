
// This test exists to verify that we have a more helpful error message in the
// case that the user wrote something that looked like it should use a param
// conditional. E.g., an if-expr with differing type variables.

config const cond : bool;

var foo = if cond then int else string;
