// Varargs work out of the box in limited cases.

extern proc printf(f:c_string, args...);

proc hello(args...) {
  printf(c"Hello %s #%d with %d varargs and #%d\n",
         (...args), args.size:int(8), args(1));
}

hello(c"World", 32:int(8));
