// from issue #23653
const D = {1..3};
var Src = [1, 2, 3, 4, 5];
var Inds: [D] int;
// order matters, `zip(Inds, Dst)` does not trigger the bug
forall (d, i) in zip(Dst, Inds) do d = Src[i];
