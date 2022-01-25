//Correctly gives error on passing a ref which is not a array of strings
// or homogeneous list of strings
use Shlex;
writeln(join(("a", "b", 1)));
