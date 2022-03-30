// This test is a slimmed-down version of the code in OP of #19301.
// It is essentially the same as forall-reciter-todo.chpl
// except:
// * it uses findfiles() as the recursive iterator
// * it is not a .future, so the error messages are in a .good file.

use FileSystem;

config const inputDir = "InputFiles";
config const debug = true;
config const minCount = 1000;

forall file in findfiles(inputDir) {
  writeln((file, debug, minCount));
}
