use IO;

var r = openReader("./advancePastByte.chpl");
r.advancePastByte("X".toByte());
write(r.readAll());
