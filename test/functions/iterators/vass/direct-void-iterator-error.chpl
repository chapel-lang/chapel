iter voiditer(): void {
  yield 1;
}
for i in voiditer() do writeln(i);
