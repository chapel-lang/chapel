use IO;

const stdout = (new file(1)).writer(kind=iokind.native, locking=false);

stdout.write(">foo bar\n");
stdout.writef("%s", ">foo bar\n");
