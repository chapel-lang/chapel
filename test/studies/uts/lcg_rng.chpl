/** The LCG RNG's state **/
record RNG_state {
  var state: int;
}

def rng_init(out state: RNG_state, seed: int) {
  state.state = seed;
}

/** Spawn the i'th child's hash given the parent's hash
 *  Parallel Note: Either all the spawns must be done by the same
 *  processor, or i must be shipped along with the child
 */
def rng_spawn(parent: RNG_state, out child: RNG_state, i: int) {
  child.state = rng_rand(parent.state + i:uint);
}


// Linear Congruential pseudoRandom Number Generator
def rng_rand(in mystate: RNG_state): uint {
  var v_n: uint(64) = mystate.state;

  v_n *= 279470273:uint(64);
  v_n %= 4294967291:uint(64);
  
  return v_n:uint(32);
}


// Convert random number to a probability
def to_prob(v: uint): real {
  return v:real/max(uint);
}

