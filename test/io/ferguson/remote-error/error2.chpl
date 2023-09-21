use IO, CTypes, OS.POSIX;
import OS.{errorCode};

extern proc printf(fmt: c_ptrConst(c_char), x:c_ptr(int(64)), y:c_ptr(int(64)));
extern proc printf(fmt: c_ptrConst(c_char), a:c_int, x:c_ptr(int(64)));
extern proc getaddress():c_ptr(int(64));
extern proc getaddress2():c_ptr(int(64));

extern proc qio_format_error_too_many_args():errorCode;

//extern proc qio_error_get_base():c_ptr(int(64));
//extern proc qio_print_raw_error(x:errorCode);

config const quiet = false;

var errs:[0..#numLocales] errorCode;

for i in 0..#numLocales {
  on Locales[i] {
    if !quiet {
      printf("loc %i has address in image: %p\n", i:c_int, getaddress());
      printf("loc %i has address2 in image: %p\n", i:c_int, getaddress2());
      //printf("loc %i has qio base: %p\n", i:c_int, qio_error_get_base());
      stdout.flush();
    }

    try {
      stdout.writef("%", 1);
    } catch e: SystemError {
      errs[i] = e.err;
    } catch {
      errs[i] = EINVAL;
    }

    writeln("loc ", i, " error ", errorToString(errs[i]));
    stdout.flush();
  }
}

for i in 0..#numLocales {
  writeln("on loc 0, loc ", i, " error ",  errorToString(errs[i]));
}

