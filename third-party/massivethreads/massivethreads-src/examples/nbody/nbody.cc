/*
 * nbody.cc
 */
#include <sys/types.h>
#include <sys/time.h>

#include "def.h"

void simulate_a_step(particle ** particles, int n_particles,  t_real dt)
{
  int t0, t1, t2, t3, t4, t5;
  space * tree;

  t0 = current_real_time_milli();
#if BUILD_TREE_PARALLEL
#if USE_MORTON
  tree = build_tree_morton(particles, n_particles);
#else
  tree = build_tree(particles, n_particles);
#endif
//  tree = build_tree_bottomup(particles, n_particles);
#else
  tree = generate_tree(particles, n_particles);
#endif
  t1 = current_real_time_milli();
  mass_momentum mm = tree->set_mass_and_cg();
  t2 = current_real_time_milli();
//  set_accels(particles, n_particles, tree);
  t3 = current_real_time_milli();
//  move_particles(particles, n_particles, dt);
  t4 = current_real_time_milli();
  free_tree(tree);
  t5 = current_real_time_milli();
  printf("-----------------------------\n");
  
  printf("  Tree nodes:    %d\n"
#if BUILD_TREE_PARALLEL
  	     "  GenTree (pll): %d msec\n"
#else
  	     "  GenTree (seq): %d msec\n"
#endif /* BUILD_TREE_PARALLEL */
         "  SetMass:       %d msec\n" \
         "  CompForces:    %d msec\n" \
         "  Leapfrog:      %d msec\n" \
         "  FreeTree:      %d msec\n" \
         "  TotalElapsed:  %d msec\n",
		 mm.n_nodes, t1 - t0, t2 - t1, t3 - t2, t4 - t3, t5 - t4, t5 - t0);
//		 -1, t1 - t0, t2 - t1, t3 - t2, t4 - t3, t5 - t4, t5 - t0);
}

void dump_particles (particle **, int);

void nbody_main (int n_particles, int n_steps, t_real dt, int dump_p)
{
  particle ** particles = generate_particles_uniform(n_particles);
  if (dump_p) dump_particles(particles, n_particles);
  for (int i = 0; i < n_steps; i++)
    simulate_a_step(particles, n_particles, dt);
  if (dump_p) dump_particles(particles, n_particles);
}

int main (int argc, char ** argv)
{
  if (argc != 3) {
    printf ("usage: %s N STEPS\n", argv[0]);
    exit(1);
  }
  int n_particles = atoi(argv[1]);
  int n_steps = atoi(argv[2]);
  int dump_p = 0;
  t_real dt = 0.1;
  
  printf("Start with N=%d, STEPS=%d\n", n_particles, n_steps);
  int t0 = current_real_time_milli();
  nbody_main(n_particles, n_steps, dt, dump_p);
  int t1 = current_real_time_milli();
  printf("Finish in %d msec\n", t1 - t0);
  return 0;
}
