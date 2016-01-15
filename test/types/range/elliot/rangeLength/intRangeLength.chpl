// Demonstrate that range.length works for int ranges so long as the length is
// <= max(int(?))

proc writeLength(type t) {
  const min_t = min(t);
  const near_min_t = min(t) + 1:t;

  const max_t = max(t);
  const near_max_t = max(t) - 1:t;

  writeln((near_min_t..min_t).length); // low end empty ok
  writeln((max_t..near_max_t).length); // high end empty ok

  writeln((min_t..min_t).length);      // low end length 1 ok
  writeln((max_t..max_t).length);      // high end length 1 ok

  writeln((0:t..near_max_t).length);   // 0 start length max(t) ok
  writeln((1:t..max_t).length);        // max end length max(t) ok

  writeln((min_t..0:t-2:t).length);    // min start length max(t) ok
  writeln((min_t+2:t..0:t).length);    // 0 end length max(t) ok

  writeln();
}

writeLength(int(8));
writeLength(int(16));
writeLength(int(32));
writeLength(int(64));
