
/*****
      ex_file.chpl -
      Example of writing and reading a class to/from a file.  See text for
      details.  Pass --binout=true|false on the command line to switch
      between a binary and text representation.  Pass --fname="file" to
      change the input/output file name.

      c 2016-2018 Primordial Machine Vision Systems
*****/

use FileSystem;
use IO;


config const fname = "test.out";        /* name of output file */
config const binout = false;            /* true to store in binary format */

/* Simplified form of our cluster.  Doesn't include the variables for
   determining the cluster's center of mass. */
record cluster : serializable {
  var npix : int;
  var r, g, b : uint(8);
  var c1, c2, c3 : real;

  proc init() {}

  proc init(reader, ref deserializer) throws {
    init this;
    readThis(reader);
  }

  /***
      writeThis:      General serialization of the data.
      args:           f - writer channel
  ***/
  proc writeThis(f) {
    /* Reading will fail if you put spaces in a binary output stream. */
    if (binout) {
      f.write(npix, c1, c2, c3, r, g, b);
    } else {
      f.write(npix);
      f.writeLiteral(" ");
      f.write(c1);
      f.writeLiteral(" ");
      f.write(c2);
      f.writeLiteral(" ");
      f.write(c3);
      f.writeLiteral(" ");
      f.write(r);
      f.writeLiteral(" ");
      f.write(g);
      f.writeLiteral(" ");
      f.write(b);
    }
  }

  proc serialize(writer, ref serializer) throws {
    writeThis(writer);
  }

  /***
      readThis:       General deserialization of the data.
      args:           f - reader channel
  ***/
  proc ref readThis(f) {
    /* Reading will fail if you put spaces in a binary output stream. */
    f.read(npix, c1, c2, c3, r, g, b);
  }

  proc deserialize(reader, ref deserializer) throws {
    readThis(reader);
  }
}

proc main() {
  var kout : [1..10] cluster;           /* original data we write to disk */
  var kin : [1..10] cluster;            /* read-back from file */
  var s = new binarySerializer(); /* general write setup */
          /* TODO: I don't know how to handle iostyle(precision=17, realfmt=1)
          with a serializer */
  var d = new binaryDeserializer(); /* general read setup */
          /* TODO: I don't know how to handle iostyle(precision=17, realfmt=1)
          with a deserializer */
  /* This causes a mismatch on read-back - reals are written with 6 digits,
     not enough. */

  for i in 1..10 {
    kout(i).npix = i ** 4;
    kout(i).r = (20 * i) : uint(8);
    kout(i).g = (100 - (10 * i)) : uint(8);
    kout(i).b = (10 + i) : uint(8);
    kout(i).c1 = i / 7.0;
    kout(i).c2 = i / 11.0;
    kout(i).c3 = i / 13.0;
  }

  /* No error checking - see main version for full usage.  We first open
     the file, then a write/read channel to it, then close in the opposite
     order. */
  var fout = open("tst.out", ioMode.cw);
  var chout = fout.writer(serializer = s, locking=false);
  chout.write(kout);
  chout.close();
  fout.close();

  var fin = open("tst.out", ioMode.r);
  var chin = fin.reader(deserializer = d, locking=false);
  chin.read(kin);
  chin.close();
  fin.close();

  writeln("Compare write to read back");
  var differ = false;                   /* true if read-back different */
  for i in 1..10 {
    if ((kout(i).npix != kin(i).npix) || (kout(i).r != kin(i).r) ||
        (kout(i).g != kin(i).g) || (kout(i).b != kin(i).b) ||
        (kout(i).c1 != kin(i).c1) || (kout(i).c2 != kin(i).c2) ||
        (kout(i).c3 != kin(i).c3)) {
      writef("> %3i     %6i     %7.2dr  %7.2dr  %7.2dr     %3u  %3u  %3u\n",
              i, kout(i).npix, kout(i).c1, kout(i).c2, kout(i).c3,
              kout(i).r, kout(i).g, kout(i).b);
      writef("< %3i     %6i     %7.2dr  %7.2dr  %7.2dr     %3u  %3u  %3u\n",
              i, kin(i).npix, kin(i).c1, kin(i).c2, kin(i).c3,
              kin(i).r, kin(i).g, kin(i).b);
      differ = true;
    }
  }

  if (!differ) {
    writeln("  no differences");
  }

  FileSystem.remove("tst.out");
}
