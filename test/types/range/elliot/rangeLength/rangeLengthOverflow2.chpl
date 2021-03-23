config param numbits = 8, signed=true;

proc writeLength(type t) {
  const min_t = min(t);
  const max_t = max(t);

  writeln((min_t..max_t).sizeAs(t));
}

if signed then
  writeLength(int(numbits));
else
  writeLength(uint(numbits));

// can't output int versions, because int overflow is undefined.
