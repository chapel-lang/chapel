var D = {1..10};
var A: [D] int;

var handshake: atomic bool;

ref As = A[D];

sync  {
  begin {
    on Locales[numLocales-1]{
      handshake.write(true);    // let other task know we're in on-clause
      handshake.waitFor(false); // wait for domain resize
      As[11] = 1;
      writeln(As.domain);
      handshake.write(true);    // let other task know we're done writing
    }
  }
  handshake.waitFor(true);      // wait for on-clause
  D = {11..20};
  handshake.write(false);       // signal domain has been resized
  handshake.waitFor(true);      // wait for domain to be written
  writeln(As);
}
