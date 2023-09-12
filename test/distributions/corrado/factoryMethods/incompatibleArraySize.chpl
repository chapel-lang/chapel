use BlockDist, CyclicDist, StencilDist;
enum Dists { B, C, S }

config const dist = Dists.B;

const dom1 = {1..6, 1..6},
      dom2 = {0..10, 0..10},
      a: [dom1] int;

select dist {
  when Dists.B do blockDist.createArray(dom2, int, a);
  when Dists.C do cyclicDist.createArray(dom2, int, a);
  when Dists.S do stencilDist.createArray(dom2, int, a);
}
