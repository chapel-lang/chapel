proc writeLength(type t) {
  const min_t = min(t);
  const max_t = max(t);

  writeln((min_t..max_t).size);
}

writeLength(uint(8));
writeLength(uint(16));
writeLength(uint(32));
writeLength(uint(64));

// can't output int versions, because int overflow is undefined.
