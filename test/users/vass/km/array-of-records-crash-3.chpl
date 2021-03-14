const vertex_domain = {1..64};

// bug note: eliminating vertex_id hides the bug
type vertex_id = int;

record vertex_struct {
  var nd: domain(1);
}

// bug note: uncommenting Vertices' type hides the bug
var Vertices // : [vertex_domain] vertex_struct
  = [i in vertex_domain] new vertex_struct(nd={1..5});
