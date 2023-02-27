use IO;

var r = openreader("./advancePastByte.chpl");
r.advancePastByte("X".toByte());
write(r.readAll());
