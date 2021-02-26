module sha1_rng {
  use SysCTypes;

  //
  // This should really be something like c_int
  //
  type sha_int = c_int;

  // SHA1 Message Digest: 20 Bytes -- this is splittable and transferrable

  // Create a fresh RNG_state context from a given seed
extern proc rng_init(ref state: uint(8), seed: sha_int);


  // Spawn the i'th child's hash given the parent's hash
  //  Parallel Note: Either all the spawns must be done by the same
  //  processor, or i must be shipped along with the child
extern proc rng_spawn(ref mystate: uint(8), ref newstate: uint(8), spawnNumber: sha_int);


  // Return the current random 32 bit number from an RNG_state
extern proc rng_rand(ref mystate: uint(8)): sha_int;


  // Scale 32 bit positive int onto the interval [0, 1)
 proc to_prob(v: sha_int): real {
    if (v < 0) {
      writeln("to_prob: warning, rand n = ", v, " out of range");
      return 0.0;
    } else {
      return v:real/(max(uint(32))/2);
    }
  }

  proc rng_getName(): string {
    return "BRG SHA-1";
  }
}
