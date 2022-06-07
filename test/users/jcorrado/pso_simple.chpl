use Random;

// dimension of search space
config param SEARCH_DIM: int = 3;
config param SWARM_SIZE: int = 2000;

// initialization constants
config const init_max_vel = 0.15;
config const space_lower = -2.5;
config const space_upper = 2.5;

// motion weights
config const momentum_weight = 0.35;
config const local_weight = 1.1;
config const global_weight = 2.8;
config const noise_weight = 0.5;
config const max_vel_mag = 0.5;
config const wall_bounce = -0.1;

config const NUM_ITERS = 250;

record Particle {
    // location and velocity in search space
    var pos: [0..#SEARCH_DIM] real;
    var vel: [0..#SEARCH_DIM] real;

    // minimum found cost and associated location
    var min_cost: real;
    var min_pos: [0..#SEARCH_DIM] real;

    // initialize position and velocity with random values
    proc init() {
        var rpv = rand_pos_vel();

        this.pos = rpv(0);
        this.vel = rpv(1);
        
        this.min_cost = 1e15;
    }

    // update the position and velocity vectors
    // enforce the search space bounds
    proc update(global_min_pos: [0..#SEARCH_DIM] real) {
        var noise_vec: [0..#SEARCH_DIM] real; fillRandom(noise_vec);

        var vel_sum = 0.0;

        for d in 0..#SEARCH_DIM {
            // update velocity
            this.vel[d] = noise_vec[d] * noise_weight +
                (global_min_pos[d] - this.pos[d]) * global_weight +
                (this.min_pos[d] - this.pos[d]) * local_weight +
                this.vel[d] * momentum_weight;

            vel_sum += vel[d] * vel[d];
        }

        // enforce maximum velocity
        if (sqrt(vel_sum) > max_vel_mag) {
            var scale = sqrt(vel_sum) / max_vel_mag;
            for d in 0..#SEARCH_DIM {
                this.vel[d] /= scale;
            }
        }

        for d in 0..#SEARCH_DIM {
            // update position
            this.pos[d] += this.vel[d];

            // enforce bounds
            if (this.pos[d] > space_upper) {
                this.pos[d] = space_upper;
                this.vel[d] *= wall_bounce;
            }
                
            if (this.pos[d] < space_lower) {
                this.pos[d] = space_lower;
                this.vel[d] *= wall_bounce;
            }
                
        }
    }

    // evaluate the cost function at the particles current location
    // updating the local minimum if necessary
    proc cost() {
        // // Rastrigin Function: https://www.sfu.ca/~ssurjano/rastr.html
        // var s = 0.0;
        // for d in 0..#SEARCH_DIM {
        //     s += this.pos[d] * this.pos[d] - 10.0 * cos(2 * pi * this.pos[d]);
        // }
        // var cost = 10.0 * SEARCH_DIM + s;

        // simple quadratic bowl
        var cost = 0.0;
        for d in 0..#SEARCH_DIM {
            cost += this.pos[d] * this.pos[d];
        }

        // update the local minimum if necessary
        if cost < this.min_cost {
            this.min_cost = cost;
            this.min_pos = this.pos;
        }
    }

    // move the particle to a new random location and perturb its velocity
    proc perturb() {
        var rpv = rand_pos_vel();

        this.pos = rpv(0);
        this.vel = this.vel + rpv(1);
    }
}

// generate random position and velocity vectors with values in the valid ranges
proc rand_pos_vel(): ([0..#SEARCH_DIM] real, [0..#SEARCH_DIM] real) {
    var rand_pos: [0..#SEARCH_DIM] real; fillRandom(rand_pos);
    var rand_vel: [0..#SEARCH_DIM] real; fillRandom(rand_vel);

    for d in 0..#SEARCH_DIM {
        rand_pos[d] -= 0.5;
        rand_pos[d] *= 2.0 * (space_upper - space_lower);
        
        rand_vel[d] -= 0.5;
        rand_vel[d] *= init_max_vel;
    }

    return (rand_pos, rand_vel);
}

record Swarm {
    var particles: [0..SWARM_SIZE] Particle;

    proc minimize(num_iterations: int): (real, [0..#SEARCH_DIM] real) {
        // global minimum cost and associated location
        var glob_min_cost = 1e15;
        var glob_min_pos: [0..#SEARCH_DIM] real;

        // optimize for num_iterations
        for i in 0..num_iterations {

            // update each particle in parallel and compute the cost at the new location
            forall p in this.particles {
                p.update(glob_min_pos);
                p.cost();

                // perturb particle motion once every 25 iterations
                if i % 25 == 0 && i != 0 {
                    p.perturb();
                }
            }

            // find the mimimum cost from this iteration
            var (i_min_cost, i_min_p_idx) = minloc reduce zip(this.particles.min_cost, this.particles.domain);

            // update the global minimum if this iteration's best beat the previous record
            if (i_min_cost < glob_min_cost) {
                glob_min_cost = i_min_cost;
                glob_min_pos = this.particles[i_min_p_idx].min_pos;
            }

            // writeln("iteration: ", i, "\t cost: ", glob_min_cost, "\t pos: ", glob_min_pos);
        }
        
        // return the global minimum cost and postion in the search space
        return (glob_min_cost, glob_min_pos);
    }
}

proc main() {
    var swarm = new Swarm();
    var (min_cost, min_cost_pos) = swarm.minimize(NUM_ITERS);

    // writeln("Found Mimimum: ", min_cost, "\t at: [", min_cost_pos, "]");

    var found_min_cost = abs(min_cost) < 1e-4;
    var found_min_pos = true;
    for d in 0..#SEARCH_DIM {
        found_min_pos = found_min_pos && (abs(min_cost_pos[d]) < 1e-2);
    }

    writeln("Minimized: ", found_min_cost, ", at Global Minimum: ", found_min_pos);
}