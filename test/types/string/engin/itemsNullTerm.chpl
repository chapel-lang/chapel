var s = "asdf";

for i in s.items() {
  writeln(i.buffSize >= 2); // do we have the room for the null terminator?
  writeln(i.buff[1] == 0);  // is it really null?
}
