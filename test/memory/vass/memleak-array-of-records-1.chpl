const vertex_domain = {1..10};

record vertex_struct {
  var nd: domain(1);
}

var Vertices : [vertex_domain] vertex_struct
  = [i in vertex_domain] new vertex_struct(nd={1..5});
