proc obfuscate_void() type { return void; }
iter voiditer(): obfuscate_void() {
  yield 1;
}
for i in voiditer() do writeln(i);
