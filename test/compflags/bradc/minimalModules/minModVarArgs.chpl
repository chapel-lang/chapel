// Varargs work out of the box in limited cases.

use CTypes;

extern proc printf(f, args...);
extern {
    const char* worldStr = "World";
    const char* formatStr = "Hello %s #%d with %d varargs and #%d\n";
}

proc hello(args...) {
  printf(formatStr, (...args), args.size:int(8), args(1));
}

hello(worldStr, 32:int(8));
