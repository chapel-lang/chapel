use HDFS;

assert(numLocales >= 2, "numLocales must be >= 2 for this example!");

var hdfs = hdfsChapelConnect("default", 0);
var gfl  = hdfs.hdfsOpen("/user/tzakian/multiB.txt", iomode.r);

for loc in Locales {
  on loc {
    var r = gfl.hdfsReader(start=50);
    var str: string;
    r.readline(str);
    writeln("on locale ", here.id, " string: " + str);
    r.close();
  }
}

on Locales[2] {
  gfl.hdfsClose();
}

on Locales[1] {
  hdfs.hdfsChapelDisconnect();
}

/* outputs:

   on locale 0 string: 0325

   on locale 1 string: 0325

   on locale 2 string: 0325

   on locale 3 string: 0325

 */
