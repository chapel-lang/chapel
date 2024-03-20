// The easiest way to get Dyno to print string literals is to cause an error,
// in this case by trying to use a string literal in an 'except' clause.

use IO except "hello";
use IO except b"hello";
use IO except """hello""";
use IO except b"""hello""";
