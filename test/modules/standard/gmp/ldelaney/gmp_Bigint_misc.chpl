use BigInteger;

// Tests the miscellaneous functions
var a = new bigint(-65536);
var b = new bigint(129);

writeln(a, parse_fit(a.fits_ulong_p()), " in an unsigned long integer");
writeln(b, parse_fit(b.fits_ulong_p()), " in an unsigned long integer");
writeln(a, parse_fit(a.fits_slong_p()), " in a signed long integer");
writeln(b, parse_fit(b.fits_slong_p()), " in a signed long integer");
writeln();

writeln(a, parse_fit(a.fits_uint_p()), " in an unsigned integer");
writeln(b, parse_fit(b.fits_uint_p()), " in an unsigned integer");
writeln(a, parse_fit(a.fits_sint_p()), " in a signed integer");
writeln(b, parse_fit(b.fits_sint_p()), " in a signed integer");
writeln();

writeln(a, parse_fit(a.fits_ushort_p()), " in an unsigned short integer");
writeln(b, parse_fit(b.fits_ushort_p()), " in an unsigned short integer");
writeln(a, parse_fit(a.fits_sshort_p()), " in a signed short integer");
writeln(b, parse_fit(b.fits_sshort_p()), " in a signed short integer");
writeln();

if a.odd_p() then writeln("a is odd");
else if a.even_p() then writeln("a is even");

if b.odd_p() then writeln("b is odd");
else if b.even_p() then writeln("b is even");

writeln(a, " is ", a.sizeinbase(10), " digits in base 10");
writeln(a, " is ", a.sizeinbase(2), " digits in base 2");
writeln(b, " is ", b.sizeinbase(10), " digits in base 10");
writeln(b, " is ", b.sizeinbase(2), " digits in base 2");

a.set(35);
b.set(9);

writeln(jacobi(b, a));
b.set(7);
writeln(jacobi(b, a));

a.set(11);
writeln(legendre(b, a));
b.set(9);
writeln(legendre(b, a));

a.set(14);
b.set(11);
writeln(kronecker(b, a));
writeln(kronecker(b, 35));
writeln(kronecker(b, 25));
writeln(kronecker(29, a));
writeln(kronecker(5, a));

// parse the fits function return value
proc parse_fit(val){
  if val == 0 then
    return " does not fit ";
  else
    return " fits ";
}
