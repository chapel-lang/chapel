module sha1_rng {

  // SHA1 Message Digest: 20 Bytes -- this is splittable and transferrable
  //const var RNG_state: type = string;


  // Create a fresh RNG_state context from a given seed
_extern def rng_init(state: string, seed: int);


  // Spawn the i'th child's hash given the parent's hash
  //  Parallel Note: Either all the spawns must be done by the same
  //  processor, or i must be shipped along with the child
_extern def rng_spawn(mystate: string, newstate: string, spawnNumber: int);


  // Return the current random 32 bit number from an RNG_state
_extern def rng_rand(mystate: string): int;


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
