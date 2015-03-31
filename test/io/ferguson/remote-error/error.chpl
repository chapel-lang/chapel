use SysBasic;

extern proc printf(fmt: c_string, a:c_int, x:c_ptr(int(64)));
extern proc getaddress():c_ptr(int(64));

config const quiet = false;

var errs:[0..#numLocales] syserr;

for i in 0..#numLocales {
  on Locales[i] {
    if !quiet then
      printf("loc %i has address in image: %p\n", i:c_int, getaddress());

    // Generate a fancy error code.
    stdout.writef("%", 1, error=errs[i]);

    writeln("loc ", i, " error ", errorToString(errs[i]));
    stdout.flush();
  }
}

for i in 0..#numLocales {
  writeln("on loc 0, loc ", i, " error ",  errorToString(errs[i]));
}
