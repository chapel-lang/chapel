// run it with
//  cd sort-random-rust
//  cargo run --release

use rand::{distributions::Standard, Rng};
use std::time::Instant;
// these are used in the alternative way to generate the array in parallel
//use rand::{rngs::SmallRng,SeedableRng};
//use rayon::prelude::*;

fn main() {
    let n = 128 * 1024 * 1024;

    let t1 = Instant::now();

    // This is arguably the most obvious way to generate the
    // random array, but it does it in one thread
    let mut values: Vec<u64> = rand::thread_rng().sample_iter(Standard).take(n).collect();

    // This alternative generates in parallel.
    // Using it increases the sequential speed (probably due to NUMA effects)
    /*
    let rng = SmallRng::seed_from_u64(42);
    let mut values = Vec::<u64>::new();
    (0..n)
        .into_par_iter()
        .map_with(rng, |rng, _| rng.sample(Standard))
        .collect_into_vec(&mut values);
    */

    let gen_duration = t1.elapsed();
    println!(
        "generating {} MiB/s",
        8.0 * (n as f64) / gen_duration.as_secs_f64() / 1024.0 / 1024.0
    );

    let start = Instant::now();
    // Without rayon.
    values.sort_unstable();
    let duration = start.elapsed();

    println!(
        "{} MiB/s",
        8.0 * (n as f64) / duration.as_secs_f64() / 1024.0 / 1024.0
    );

    println!(
        "{} million elements sorted per second",
        (n as f64) / duration.as_secs_f64() / 1000.0 / 1000.0
    );
}
