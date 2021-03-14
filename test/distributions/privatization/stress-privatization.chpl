// Stress test privatization using a paired down version of a test that was
// running into races in the privatization code.

use BlockDist;

proc main () {
  const numVertices = 8*here.maxTaskPar;
  const vertex_domain = {1..#numVertices} dmapped Block ({1..#numVertices});

  forall s in vertex_domain {
    var min_distance$: [vertex_domain] int = -1;
  }
}
