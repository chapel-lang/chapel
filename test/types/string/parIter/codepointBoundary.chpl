config const n = 17; // make sure this is odd

var s = "ç"*n;

var totalBytes = 0;
var totalCodepoints = 0;
forall c in s with (+ reduce totalBytes, + reduce totalCodepoints) {
  totalBytes += c.numBytes;
  totalCodepoints += c.size;
}

assert(totalBytes == n*2);
assert(totalCodepoints == n);
