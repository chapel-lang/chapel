use Random;
use RandomSupport;
config param n_retries: int = 10;

{
  var seed = RandomSupport.SeedGenerator.devURandomSeed;
  if (seed == 0) {
    for i in 1..n_retries {
      seed = RandomSupport.SeedGenerator.devURandomSeed;
      if (seed != 0) {
        break;
      }
    }
    if (seed == 0) {
      writeln("Looks like /dev/urandom is not available.");
    }
  }
  // ensure that it works to create a random stream.
  var udr = createRandomStream(real, seed=seed);
}