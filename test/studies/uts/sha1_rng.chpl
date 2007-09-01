module sha1_rng {

  // SHA1 Message Digest: 20 Bytes -- this is splittable and transferrable

  // The message digest (hash) is implemented in Chapel as an array of 20
  // uint(8)s.  C pointers to these hashes are passed into the external
  // functions declared below.  The way the pointer is generated from Chapel is
  // currently a workaround because calling out is not fully implemented.
  // To generate the pointer we declare the argument in the _extern to be an
  // inout uint(8) and at the call site we pass in hash[1].  In the current
  // Chapel spec, copy-in/out is the responsibility of the function being
  // called, so (voila!) this causes the compiler to give a pointer to the
  // first element in the hash to the _extern function.

  // Create a fresh RNG_state context from a given seed
_extern def rng_init(inout state: uint(8), seed: int);


  // Spawn the i'th child's hash given the parent's hash
  //  Parallel Note: Either all the spawns must be done by the same
  //  processor, or i must be shipped along with the child
_extern def rng_spawn(inout mystate: uint(8), inout newstate: uint(8), spawnNumber: int);


  // Return the current random 32 bit number from an RNG_state
_extern def rng_rand(inout mystate: uint(8)): int;


  // Scale 32 bit positive int onto the interval [0, 1)
  def to_prob(v: int): real {
    if (v < 0) {
      writeln("to_prob: warning, rand n = ", v, " out of range");
      return 0.0;
    } else {
      return v:real/(max(uint)/2);
    }
  }

  def rng_getName(): string {
    return "BRG SHA-1";
  }
}
