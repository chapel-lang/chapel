use BigInteger;

/* fails wrapped in a function too */
/* proc f() { */

var y:bigint;
/* var x = new bigint("123456"); // PASS var works okay here */
const x = new bigint("123456"); // FAIL

on Locales[1] {
  /* var x = new bigint("123456"); // FAIL and with const x too */
  /* var x:bigint = 123456; // FAIL */

  on Locales[2] {
    /* var x = new bigint("123456"); // PASS */
    y = x;
    writeln(x.sizeInBase(10));
    writeln(y.sizeInBase(10));
  }
}

/* } */
/* f(); */
