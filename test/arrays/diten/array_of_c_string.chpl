require "array_of_c_string.h";//, "array_of_c_string.c";

extern proc foo(argc: int, argv: c_void_ptr);

proc main(args: [] string) {
  var c_args: [0..#args.numElements] c_string;

  for i in 0..#args.numElements do
    c_args[i] = args[i].c_str();

  foo(c_args.numElements, c_ptrTo(c_args));
}
