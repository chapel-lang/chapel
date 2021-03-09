/*

This module implements the SHA256 cryptographic hash algorithm.

*/
module SHA256Implementation {

  // This implementation is based on FIPS 180-4 and uses some
  // hints from other implementations for reducing the number
  // of instructions in the basic functions.

  /* This record stores the state of the hash. */
  record SHA256State {
    var length:uint(64); // length of hashed message, in bits
    var H:8*uint(32);    // current hash
  };

  /* This function simply casts the passed integer to a uint(32) */
  inline proc ul(x) {
    return x:uint(32);
  }
  /* This function casts each portion of the passed tuple
     to uint(32) and returns the result. */
  inline proc ul(x:8*int) {
    var ret:8*uint(32);
    for i in 0..7 {
      ret[i] = x[i]:uint(32);
    }
    return ret;
  }


  // The K constants
  private
  var K:64*uint(32) =
(ul(0x428a2f98), ul(0x71374491), ul(0xb5c0fbcf), ul(0xe9b5dba5), ul(0x3956c25b),
 ul(0x59f111f1), ul(0x923f82a4), ul(0xab1c5ed5), ul(0xd807aa98), ul(0x12835b01),
 ul(0x243185be), ul(0x550c7dc3), ul(0x72be5d74), ul(0x80deb1fe), ul(0x9bdc06a7),
 ul(0xc19bf174), ul(0xe49b69c1), ul(0xefbe4786), ul(0x0fc19dc6), ul(0x240ca1cc),
 ul(0x2de92c6f), ul(0x4a7484aa), ul(0x5cb0a9dc), ul(0x76f988da), ul(0x983e5152),
 ul(0xa831c66d), ul(0xb00327c8), ul(0xbf597fc7), ul(0xc6e00bf3), ul(0xd5a79147),
 ul(0x06ca6351), ul(0x14292967), ul(0x27b70a85), ul(0x2e1b2138), ul(0x4d2c6dfc),
 ul(0x53380d13), ul(0x650a7354), ul(0x766a0abb), ul(0x81c2c92e), ul(0x92722c85),
 ul(0xa2bfe8a1), ul(0xa81a664b), ul(0xc24b8b70), ul(0xc76c51a3), ul(0xd192e819),
 ul(0xd6990624), ul(0xf40e3585), ul(0x106aa070), ul(0x19a4c116), ul(0x1e376c08),
 ul(0x2748774c), ul(0x34b0bcb5), ul(0x391c0cb3), ul(0x4ed8aa4a), ul(0x5b9cca4f),
 ul(0x682e6ff3), ul(0x748f82ee), ul(0x78a5636f), ul(0x84c87814), ul(0x8cc70208),
 ul(0x90befffa), ul(0xa4506ceb), ul(0xbef9a3f7), ul(0xc67178f2));

  // The logical functions

  // Rotate right
  private inline
  proc ROTR(x:uint(32), y:uint(32)):uint(32) {
    use BitOps;
    return rotr(x & ul(0xffffffff), y & 31);
  }

  private inline
  proc Ch(x:uint(32), y:uint(32), z:uint(32)):uint(32) {
    return z ^ (x & (y ^ z));
  }

  private inline
  proc Maj(x:uint(32), y:uint(32), z:uint(32)):uint(32) {
    return ((x | y) & z) | (x & y);
  }

  // Shift right
  private inline
  proc SHR(x:uint(32), n:uint(32)):uint(32) {
    return (x & ul(0xffffffff)) >> n;
  }

  private inline
  proc Sigma0(x:uint(32)):uint(32) {
    return ROTR(x, 2) ^ ROTR(x, 13) ^ ROTR(x, 22);
  }

  private inline
  proc Sigma1(x:uint(32)):uint(32) {
    return ROTR(x, 6) ^ ROTR(x, 11) ^ ROTR(x, 25);
  }

  private inline
  proc Gamma0(x:uint(32)):uint(32) {
    return ROTR(x, 7) ^ ROTR(x, 18) ^ SHR(x, 3);
  }

  private inline
  proc Gamma1(x:uint(32)):uint(32) {
    return ROTR(x, 17) ^ ROTR(x, 19) ^ SHR(x, 10);
  }

  // hash computation
  private
  proc sha256_compute(ref state:SHA256State, msg:16*uint(32)) {
    var W:64*uint(32);

    // Prepare the message schedule
    for i in 0..15 {
      W[i] = msg[i];
    }
    for i in 16..63 {
      W[i] = Gamma1(W[i - 2]) + W[i - 7] + Gamma0(W[i - 15]) + W[i - 16];
    }

    // Initialize the working variables
    var a = state.H[0];
    var b = state.H[1];
    var c = state.H[2];
    var d = state.H[3];
    var e = state.H[4];
    var f = state.H[5];
    var g = state.H[6];
    var h = state.H[7];

    for i in 0..63 {
      var t1:uint(32);
      var t2:uint(32);
      t1 = h + Sigma1(e) + Ch(e, f, g) + K[i] + W[i];
      t2 = Sigma0(a) + Maj(a, b, c);
      h = g;
      g = f;
      f = e;
      e = d + t1;
      d = c;
      c = b;
      b = a;
      a = t1 + t2;
    }

    // compute the intermediate hash value
    state.H[0] += a;
    state.H[1] += b;
    state.H[2] += c;
    state.H[3] += d;
    state.H[4] += e;
    state.H[5] += f;
    state.H[6] += g;
    state.H[7] += h;

  }

  // Set the bit numbered `bit` in `msg`.
  // Bit numbering starts with 0. Bit numbers 0-31 are in msg(1)
  // and bit 0 is the most-significant.
  private
  proc set_bit(ref msg:16*uint(32), bit:uint) {
    assert(bit < 512);
    var whichword = ul(bit / 32);
    var inword = ul(bit % 32);
    msg[whichword] |= ul(1) << (31 - inword);
  }

  // Clear the bits after bit `nbits` in `msg`.
  // Bit numbering starts with 0. Bit numbers 0-31 are in msg(1)
  // and bit 0 is the most-significant.
  private
  proc clear_bits_after(ref msg:16*uint(32), nbits:uint) {
    assert(nbits < 512);
    var whichword = ul(nbits / 32);
    var inword = ul(nbits % 32);

    var word:uint(32) = msg[whichword];
    // Clear the bits after inword in word
    var zero:uint(32) = 0;
    var ones:uint(32) = ~zero;
    var mask:uint(32);
    if inword == 0 then
      mask = 0;
    else
      mask = ones << (32-inword);
    word &= mask;
    msg[whichword] = word;

    // Clear the remaining words
    for i in whichword+1..15 {
      msg[i] = 0;
    }
  }

  /* Initialize a SHA256State so that it can perform hash computations. */
  proc SHA256State.init() {
    this.length = 0;

    this.H = ul( (0x6a09e667, 0xbb67ae85, 0x3c6ef372, 0xa54ff53a,
                  0x510e527f, 0x9b05688c, 0x1f83d9ab, 0x5be0cd19) );
  }

  /* Processes 512 bits of message.
     This function can be called an arbitrary number of times
     to compute the hash for a long message.

     Note, the `msg` words should already be in native byte order.
     That means the caller probably needed to already run big-endian-to-host
     on the message.
   */
  proc ref SHA256State.fullblock(msg:16*uint(32)) {
    this.length += 512;
    sha256_compute(this, msg);
  }

  /* Process the final <= 512 bits of message.
     Computes and returns the final hash.

     As with SHA256State.fullblock, the msg words should already be
     in native byte order. That means the caller probably needed to already run
     big-endian-to-host on the message.

     nbits is the number of bits in msg to use
   */
  proc ref SHA256State.lastblock(in msg:16*uint(32), in nbits:uint):8*uint(32)
  {
    this.length += nbits;

    // This routine is handling padding the message
    if nbits == 512 {
      sha256_compute(this, msg);
      nbits = 0;
    }
    // Append the 1 bit
    set_bit(msg, nbits);
    nbits += 1;
    // clear the last 512 - bits of the message
    clear_bits_after(msg, nbits);

    // Now we need to pad zeros so there is room to store the 8 bytes of
    if nbits > 512 - 64 {
      sha256_compute(this, msg);
      nbits = 0;
      clear_bits_after(msg, nbits);
    }

    // Now store the 64-bit length quantity in the last two words
    // of the message.
    msg[14] = ul((this.length >> 32) & ul(0xffffffff));
    msg[15] = ul(this.length & ul(0xffffffff));

    // hash the final block
    sha256_compute(this, msg);

    return this.H;
  }

  /* This function exists to test private methods in this module and can
     be called to run a sequence of unit tests. */
  proc unittest() {
    var zeros:16*uint(32);
    var ones:16*uint(32);
    var msg:16*uint(32);

    for i in 0..15 {
      ones[i] = ~zeros[i];
    }

    msg = ones;
    clear_bits_after(msg, 0);
    assert(msg[0] == 0 && msg[1] == 0 && msg[15] == 0);

    msg = ones;
    clear_bits_after(msg, 1);
    assert(msg[0] == 0x80000000 && msg[1] == 0 && msg[15] == 0);

    msg = ones;
    clear_bits_after(msg, 2);
    assert(msg[0] == 0xc0000000 && msg[1] == 0 && msg[15] == 0);

    msg = ones;
    clear_bits_after(msg, 31);
    assert(msg[0] == 0xfffffffe && msg[1] == 0 && msg[15] == 0);

    msg = ones;
    clear_bits_after(msg, 32);
    assert(msg[0] == 0xffffffff && msg[1] == 0 && msg[15] == 0);

    msg = ones;
    clear_bits_after(msg, 33);
    assert(msg[0] == 0xffffffff && msg[1] == 0x80000000 &&
           msg[3] == 0 && msg[15] == 0);

    msg = ones;
    clear_bits_after(msg, 34);
    assert(msg[0] == 0xffffffff && msg[1] == 0xc0000000 &&
           msg[3] == 0 && msg[15] == 0);

    msg = zeros;
    set_bit(msg, 0);
    assert(msg[0] == 0x80000000 && msg[1] == 0);

    msg = zeros;
    set_bit(msg, 1);
    assert(msg[0] == 0x40000000 && msg[1] == 0);

    msg = zeros;
    set_bit(msg, 2);
    assert(msg[0] == 0x20000000 && msg[1] == 0);

    msg = zeros;
    set_bit(msg, 31);
    assert(msg[0] == 0x00000001 && msg[1] == 0);

    msg = zeros;
    set_bit(msg, 32);
    assert(msg[0] == 0 && msg[1] == 0x80000000 && msg[3] == 0);

    msg = zeros;
    set_bit(msg, 33);
    assert(msg[0] == 0 && msg[1] == 0x40000000 && msg[3] == 0);


    var startingState:SHA256State;
    var state:SHA256State;
    var hash:8*uint(32);

    state = startingState;
    msg = zeros;
    msg[0] = 0x61000000;
    hash = state.lastblock(msg, 1*8);
    assert(hash==ul( ( 0xca978112, 0xca1bbdca, 0xfac231b3, 0x9a23dc4d,
                       0xa786eff8, 0x147c4e72, 0xb9807785, 0xafee48bb) ));

    // Try hashing varying lengths of 0 bytes
    // These tests help ensure we have the padding right

    // 0 bytes
    state = startingState;
    hash = state.lastblock(zeros, 0);

    assert(hash==ul( (0xe3b0c442, 0x98fc1c14, 0x9afbf4c8, 0x996fb924,
		      0x27ae41e4, 0x649b934c, 0xa495991b, 0x7852b855) ));


    // 55 bytes
    state = startingState;
    hash = state.lastblock(zeros, 55*8);

    assert(hash==ul( (0x02779466, 0xcdec1638, 0x11d07881, 0x5c633f21,
                      0x90141308, 0x1449002f, 0x24aa3e80, 0xf0b88ef7) ));

    // 56 bytes
    state = startingState;
    hash = state.lastblock(zeros, 56*8);
    assert(hash==ul( (0xd4817aa5, 0x497628e7, 0xc77e6b60, 0x6107042b,
                      0xbba31308, 0x88c5f47a, 0x375e6179, 0xbe789fbb) ));

    // 119 bytes
    state = startingState;
    state.fullblock(zeros); // 64 bytes
    hash = state.lastblock(zeros, 55*8);
    assert(hash==ul((0xf616b0d5, 0x4e78571a, 0x9611f343, 0xc9f8e022,
                     0xe859e920, 0x381ab0e4, 0xd3da01e1, 0x93a7bd7e) ));

    // 120 bytes
    state = startingState;
    state.fullblock(zeros); // 64 bytes
    hash = state.lastblock(zeros, 56*8);
    assert(hash==ul((0x6edd9f6f, 0x9cc92cde, 0xd36e6c4a, 0x580933f9,
                     0xc9f1b905, 0x62b46903, 0xb806f219, 0x02a1a54f) ));

    // 128 bytes
    state = startingState;
    state.fullblock(zeros);
    hash = state.lastblock(zeros, 64*8);
    assert(hash==ul( (0x38723a2e, 0x5e8a17aa, 0x7950dc00, 0x8209944e,
                      0x898f69a7, 0xbd10a23c, 0x839d341e, 0x935fd5ca) ));

    // 129 bytes
    state = startingState;
    state.fullblock(zeros);
    state.fullblock(zeros);
    hash = state.lastblock(zeros, 1*8);
    assert(hash==ul( (0x2c80619d, 0x7e7c5825, 0x7293cda3, 0xa878c13e,
                      0x5856f4e0, 0x6f6f9060, 0x1276f7b9, 0x179c9e07) ));
  }
}
