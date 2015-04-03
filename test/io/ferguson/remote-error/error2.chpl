use SysBasic;

extern proc printf(fmt: c_string, x:c_ptr(int(64)), y:c_ptr(int(64)));
extern proc printf(fmt: c_string, a:c_int, x:c_ptr(int(64)));
extern proc getaddress():c_ptr(int(64));
extern proc getaddress2():c_ptr(int(64));

extern proc qio_format_error_too_many_args():syserr;

//extern proc qio_error_get_base():c_ptr(int(64));
//extern proc qio_print_raw_error(x:syserr);

config const quiet = false;

var errs:[0..#numLocales] syserr;

for i in 0..#numLocales {
  on Locales[i] {
    if !quiet {
      printf("loc %i has address in image: %p\n", i:c_int, getaddress());
      printf("loc %i has address2 in image: %p\n", i:c_int, getaddress2());
      //printf("loc %i has qio base: %p\n", i:c_int, qio_error_get_base());
      stdout.flush();
    }

    stdout.writef("%", 1, error=errs[i]);

    writeln("loc ", i, " error ", errorToString(errs[i]));
    stdout.flush();
  }
}

for i in 0..#numLocales {
  writeln("on loc 0, loc ", i, " error ",  errorToString(errs[i]));
}

