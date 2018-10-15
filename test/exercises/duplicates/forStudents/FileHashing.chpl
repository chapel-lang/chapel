module FileHashing {

  record SHA256Hash {
    var hash: 8*uint(32);

    // ought to be throws, see issue #7261
    proc writeThis(f) {
      for component in hash {
        var s = try! "%08xu".format(component);
        f <~> s;
      }
    }
    proc init() {
      // compiler adds initialization of hash to 0s
    }
    proc init(from: SHA256Hash) {
      this.hash = from.hash;
    }
    proc init(hash: 8*uint(32)) {
      this.hash = hash;
    }
  }

  proc =(ref lhs: SHA256Hash, rhs: SHA256Hash) {
    lhs.hash = rhs.hash;
  }

  /* no longer necessary
  proc chpl__defaultHash(h: SHA256Hash) {
    return chpl__defaultHash(h.hash);
  }*/

  proc compare(a: SHA256Hash, b: SHA256Hash): int {

    for i in 1..8 {
      var aa = a.hash[i];
      var bb = b.hash[i];
      if aa < bb {
        return -1;
      }
      if aa > bb {
        return 1;
      }
    }
    return 0;
  }

  proc <(a: SHA256Hash, b: SHA256Hash) {
    return compare(a, b) < 0;
  }
  proc <=(a: SHA256Hash, b: SHA256Hash) {
    return compare(a, b) <= 0;
  }
  proc ==(a: SHA256Hash, b: SHA256Hash) {
    return compare(a, b) == 0;
  }
  proc !=(a: SHA256Hash, b: SHA256Hash) {
    return compare(a, b) != 0;
  }
  proc >=(a: SHA256Hash, b: SHA256Hash) {
    return compare(a, b) >= 0;
  }
  proc >(a: SHA256Hash, b: SHA256Hash) {
    return compare(a, b) > 0;
  }


  /*
     Returns the SHA256Hash for the file stored at `path`.
   */
  proc computeFileHash(path: string): SHA256Hash throws {
    use SHA256Implementation;

    var f = open(path, iomode.r);
    var len = f.length();
    var r = f.reader(kind=iokind.big, locking=false,
                     start=0, end=len);


    var msg:16*uint(32); // aka 64 bytes
    var offset = 0;
    var state:SHA256State;

    // Read as many full blocks as we can
    // but don't ever read the last block in this loop
    // since we need to pass that to state.lastblock
    while offset+64 < len {
      r.read(msg);
      state.fullblock(msg);
      offset += 64;
    }


    // clear msg before last block, so unused data are zeros
    for i in 1..16 {
      msg[i] = 0;
    }

    var nbits:uint = 0;
    var msgi = 1;
    // Now handle the last 4-byte words
    while offset+4 <= len {
      r.read(msg[msgi]);
      msgi += 1;
      offset += 4;
      nbits += 32;
    }
    
    if offset < len {
      var lastword:uint(32);
      var byte:uint(8);
      var bytei = 0;
      // Now handle the last 1-byte portions
      while offset < len {
        r.read(byte);
        lastword <<= 8;
        lastword |= byte;
        bytei += 1;
        offset += 1;
        nbits += 8;
      }
      // Shift left so the data is in the high order bits of lastword.
      while bytei != 4 {
        lastword <<= 8;
        bytei += 1;
      }
      msg[msgi] = lastword;
    }

    var hash:8*uint(32) = state.lastblock(msg, nbits);

    return new SHA256Hash(hash);
  }

  /*
    Given a path argument, computes the realPath (that
    is the path after symbolic links are processed),
    and then computes a relative version of that to the
    current directory. Returns the result.
   */
  proc relativeRealPath(path: string): string throws {
    use Path only ;
    var currentDirectory = Path.realPath(".");
    var fullPath = Path.realPath(path);
    if !currentDirectory.endsWith("/") {
      currentDirectory += "/";
    }
    // If fullPath starts with currentDirectory, remove it
    if fullPath.startsWith(currentDirectory) {
      fullPath = fullPath[currentDirectory.length+1..];
    }
    return fullPath;
  }
}
