param r32:real(32) = 1.0;
param r64:real(64) = 1.0;
param i32:imag(32) = 1.0i;
param i64:imag(64) = 1.0i;
param c64r:complex(64) = 1.0;
param c128r:complex(128) = 1.0;
param c64i:complex(64) = 1.0i;
param c128i:complex(128) = 1.0i;

proc writeparam(param p)
{
  writeln(p);
}

writeparam(r32);
writeparam(r64);
writeparam(i32);
writeparam(i64);
writeparam(c64r);
writeparam(c128r);
writeparam(c64i);
writeparam(c128i);
