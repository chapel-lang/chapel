use decodePolicy;
proc showCodepoints(str) {
  for c in str.codepoints() {
    writef("0x%04xu ", c);
  }
  writeln();
}


// The examples are from Unicode 12.0.0 spec
// https://www.unicode.org/versions/Unicode12.0.0/ch03.pdf#M9.16602.Heading.38.Unicode.Encoding.Forms)
// Section 3.9, pages 128-129. Where it talks about this strategy as the
// "recommended" strategy and alludes to W3C's convention.
showCodepoints(b"\xC0\xAF\xE0\x80\xBF\xF0\x81\x82\x41".decode(replace));
showCodepoints(b"\xED\xA0\x80\xED\xBF\xBF\xED\xAF\x41".decode(replace));
showCodepoints(b"\xF4\x91\x92\x93\xFF\x41\x80\xBF\x42".decode(replace));
showCodepoints(b"\xE1\x80\xE2\xF0\x91\x92\xF1\xBF\x41".decode(replace));
