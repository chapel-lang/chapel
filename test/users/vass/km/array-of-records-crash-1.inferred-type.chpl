// This is an extract from KM's code, May 2011.

const vertex_domain = {1..64};

record vertex_struct {

  var vlock$: sync bool;
  proc init() { vlock$ = true; }

  // Note: the default for vlock$ should be "empty", so that the array elements
  // are initialized to that. The explicit constructor should make it "full".
  // That way the assignment below will succeed (rather than block).
}

// bug note: uncommenting the type of Vertices eliminates the symptoms
var Vertices // : [vertex_domain] vertex_struct
  = [i in vertex_domain] new vertex_struct();
