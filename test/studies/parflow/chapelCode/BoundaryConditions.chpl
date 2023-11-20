use CTypes;
use GroundGeometry;

require "problem_bc.h";
const BC_ALL = -1;
const DirichletBC = 0;
const FluxBC = 1;
const OVerlandBC = 2;
const SeepageFaceBC = 3;
const OverlandKinematicBC = 4;
const OverlandDiffusiveBC = 5;

extern record BCStruct {
    //var gr_domain: c_ptr(GrGeomSolid);
    var num_patches: int;
    var patch_indexes: c_ptr(int);
    var bc_types: c_ptr(int);
    var values: c_ptr(c_ptr(c_ptr(real)));
}
