// Varargs work out of the box in limited cases.

extern proc printf(f, args...);

proc hello(args...) {
  printf("Hello %s #%d with %d varargs and #%d\n",
         (...args), args.size:int(8), args(1));
}

hello("World", 32:int(8));
