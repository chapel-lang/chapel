module FileHashing {
  use Crypto only;
  use Path only ;

  record SHA256Hash {
    var hash: owned Crypto.CryptoBuffer;

    // ought to be throws, see issue #7261
    proc writeThis(f) {
      if hash == nil {
        f <~> " "*64;
      } else {
        var len = hash.getBuffSize();
        var ptr = hash.getBuffPtr();
        for i in 0..#len {
          var s = try! "%02xu".format(ptr[i]);
          f <~> s;
        }
      }
    }
    proc init() {
      this.hash = nil;
    }
    proc init(from: SHA256Hash) {
      if from.hash {
        this.hash = new owned Crypto.CryptoBuffer(from.hash.getBuffData());
      } else {
        this.hash = nil;
      }
    }
    proc init(taking: owned Crypto.CryptoBuffer) {
      this.hash = taking;
    }
  }

  proc =(ref lhs: SHA256Hash, rhs: SHA256Hash) {
    // If the sizes are the same, copy the bytes
    if lhs.hash != nil && rhs.hash != nil &&
       lhs.hash.getBuffSize() == rhs.hash.getBuffSize() {

      for (lhsByte,rhsByte) in zip(lhs.hash.buff, rhs.hash.buff) {
        lhsByte = rhsByte;
      }

    // Otherwise, replace lhs.hash with a new one
    } else {
      if rhs.hash == nil {
        lhs.hash = nil;
      } else {
        lhs.hash = new owned Crypto.CryptoBuffer(rhs.hash.buff);
      }
    }
  }

  proc chpl__defaultHash(h: SHA256Hash) {
    return chpl__defaultHash(h.hash.buff);
  }

  proc compare(a: SHA256Hash, b: SHA256Hash): int {

    // First, establish that the hashes aren't nil
    // we sort nil before all other hashes
    if a.hash == nil || b.hash == nil {
      if a.hash == nil && b.hash == nil {
        return 0;
      } else if a.hash == nil {
        return -1;
      } else {
        return 1;
      }
    }

    var ret = 0;
    for (aa,bb) in zip(a.hash.buff, b.hash.buff) {
      if ret == 0 {
        if aa < bb {
          ret = -1;
        }
        if aa > bb {
          ret = 1;
        }
      }
    }
    return ret;
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
    var f = open(path, iomode.r);
    var len = f.length();
    if len == 0 {
      return new SHA256Hash();
    }

    var r = f.reader(kind=iokind.native, locking=false,
                     start=0, end=len);
    var bytes: [0..#len] uint(8);
    r.read(bytes);
    
    var buf = new owned Crypto.CryptoBuffer(bytes);

    var SHA256 = new owned Crypto.Hash(Crypto.Digest.SHA256);

    var hash = new SHA256Hash(SHA256.getDigest(buf));
    return hash;
  }
  
  /*
    Given a path argument, computes the realPath (that
    is the path after symbolic links are processed),
    and then computes a relative version of that to the
    current directory. Returns the result.
   */
  proc relativeRealPath(path: string): string throws {
    var currentDirectory = Path.realPath(".");
    var fullPath = Path.realPath(path);
    var prefix = Path.commonPath(currentDirectory, fullPath);
    var normalized = fullPath[prefix.length+1..];
    // If there was a meaningful prefix, that means
    // fullPath is within currentDirectory. In that event,
    // make sure normalized doesn't start with a "/".
    if prefix.length > 0 && prefix != "/" {
      normalized = normalized.strip("/", leading=true, trailing=false);
    }
    return normalized;
  }
}
