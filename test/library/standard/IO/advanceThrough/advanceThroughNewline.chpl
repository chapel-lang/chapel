use IO;
var r = openReader("advanceThroughNewline.chpl");
r.advanceThroughNewline();
write(r.readLine()); // 🐍
r.advanceThroughNewline();
write(r.readLine());
