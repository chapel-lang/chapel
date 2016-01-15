// Demonstrate that range.length works for most uint ranges, but overflows for
// maximal uint ranges.

proc writeLength(type t) {
  const min_t = min(t);
  const near_min_t = min(t) + 1:t;

  const max_t = max(t);
  const near_max_t = max(t) - 1:t;

  const zero = 0:t;

  writeln((near_min_t..min_t).length); // low end empty ok
  writeln((max_t..near_max_t).length); // high end empty ok

  writeln((min_t..min_t).length);      // low end length 1 ok
  writeln((max_t..max_t).length);      // high end length 1 ok

  writeln((min_t..near_max_t).length); // low end near maximal ok
  writeln((near_min_t..max_t).length); // high end near maximal ok

  writeln((min_t..max_t).length);      // maximal overflows :(
  writeln();
}

writeLength(uint(8));
writeLength(uint(16));
writeLength(uint(32));
writeLength(uint(64));
