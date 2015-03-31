var errs:[0..#numLocales] syserr;

use SysBasic;

extern proc qio_print_raw_error(x:syserr);
extern proc printf(fmt: c_string, x:c_ptr(int(64)));
extern proc getaddress():c_ptr(int(64));
extern proc qio_error_get_base():syserr;

for i in 0..#numLocales {
  on Locales[i] {
    printf("%p\n", getaddress());
    write("base ");
    qio_print_raw_error(qio_error_get_base());
    writeln();

    stdout.writef("%", 1, error=errs[i]);

    write("offset ");
    qio_print_raw_error(errs[i]);
    writeln();
    stdout.flush();
    writeln(errorToString(errs[i]));
    stdout.flush();
  }
}

for i in 0..#numLocales {
  writeln(errorToString(errs[i]));
}
