/* treecode.cc */

#include <cstdlib>
#include <cstdio>
#include "treecode.h"

simulation sim;
int verbose;

#define MFRAC  0.999  //

void init(arguments *args)
{
  verbose = args->verbose;
  sim.mol = args->mol;
  sim.steps = args->steps;

  if (verbose >= 1) {
    printf("Initial with following configurations...\n"
           "  # of molecules: %d\n"
           "      # of steps: %d\n",
           sim.mol, sim.steps);
  }

  if (sim.mol > 0)
    sim.mols = (mol *) xmalloc(sizeof(mol) * sim.mol);

  // Initial molecular conditions with Plummer model
  real rsc, vsc, r, v, x, y;
  vector rcm, vcm;
  rsc = (3 * PI) / 16;
  vsc = sqrt(1.0 / rsc);
  rcm.clr();
  vcm.clr();
  for (int i = 0; i < sim.mol; i++) {
    mol &m = sim.mols[i];
    m.mass = 1.0 / sim.mol;
    x = xrandom(0.0, MFRAC);
    r = 1.0 / sqrt(pow(x, -2.0/3.0) - 1);
  }
}

int main(int argc, char *argv[])
{
  arguments *args = parse_cmdline(argc, argv);
  if (args == NULL) exit(1);

  init(args);
}
