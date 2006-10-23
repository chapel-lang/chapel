enum MemUnits {Bytes, KB, MB, GB};

def physicalMemory(loc: locale, unit: MemUnits = Bytes): int {
  const bytesInLocale = 526086144;
  select (unit) {
    when Bytes do return bytesInLocale;
    when KB do return bytesInLocale / 1024;
    when MB do return bytesInLocale / 1024**2;
    when GB do return bytesInLocale / 1024**3;
  }
}
