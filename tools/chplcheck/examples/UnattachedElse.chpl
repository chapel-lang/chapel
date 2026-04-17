/*
  The recommended Chapel style is for else statements to be attached to the
  curly brace of the preceding block, rather than being on a separate line.
*/

// this uses an unattached else statement
if condition {
  doSomething();
}
else {
  doSomethingElse();
}

// this uses an attached else statement
if condition {
  doSomething();
} else {
  doSomethingElse();
}
