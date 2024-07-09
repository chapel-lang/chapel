// from issue #23653, where a use-before-def caused an internal error from auto-aggregation optimizations
const D = {1..3};
var Src = [1, 2, 3, 4, 5];
var Inds: [D] int;
// order matters, `zip(Inds, Dst)` does not trigger the bug
forall (d, i) in zip(Dst, Inds) do d = Src[i];
