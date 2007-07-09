/*
**  Linear Conguential Generator (LCG)
**  James Dinan <dinan@cray.com>
**
**  Note: This RNG produces questionable randomness, for development
**        use only!
*/

module lcg_rng {

  // The LCG RNG's state -- this is splittable and transferrable
  record RNG_state {
    var state: uint;
  }


  // Create a fresh RNG_state context from a given seed
  def rng_init(inout state: RNG_state, seed: uint) {
    state.state = rng_nextrand(seed);
  }


  // Spawn the i'th child's hash given the parent's hash
  //  Parallel Note: Either all the spawns must be done by the same
  //  processor, or i must be shipped along with the child
  def rng_spawn(in parent: RNG_state, inout child: RNG_state, i: int) {
    child.state = rng_nextrand(parent.state + i:uint);
  }


  // Return the current random 32 bit number from an RNG_state
  def rng_rand(in state: RNG_state): uint {
    return state.state;
  }


  // Generate the next random number from the given RNG_state
  def rng_nextrand(v: uint): uint {
    var v_n: uint(64) = v:uint(64);

    v_n *= 279470273:uint(64);
    v_n %= 4294967291:uint(64);

    return v_n:uint(32);
  }


  // Convert random number to a probability.
  //  Assuming that LCG is uniform which is probably not true  :)
  def to_prob(v: uint): real {
    return v:real/max(uint);
  }

}
