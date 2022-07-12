use BigInteger;

const a = new bigint(5);
writeln(a.fits_ulong_p() & a.fits_slong_p() & a.fits_uint_p() & a.fits_sint_p() & a.fits_ushort_p() & a.fits_sshort_p());
