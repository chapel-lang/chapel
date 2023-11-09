use CTypes;

require "index_space.h";
require "grgeometry.h";
require "input_database.h";
extern type Point = 3*int(32);
const GrGeomOctreeNumFaces = 6;

const GrGeomOctreeFaceL = 0;
const GrGeomOctreeFaceR = 1;
const GrGeomOctreeFaceD = 2;
const GrGeomOctreeFaceU = 3;
const GrGeomOctreeFaceB = 4;
const GrGeomOctreeFaceF = 5;

const fdirL = (-1,0,0);
const fdirR = (1,0,0);
const fdirD = (0,-1,0);
const fdirU = (0,1,0);
const fdirB = (0,0,-1);
const fdirF = (0,0,1);
const fdirs = (fdirL, fdirR, fdirD, fdirU, fdirB, fdirF);

proc create_fdir(f: int) {
    return fdirs[f];
}

extern record Box {
    var lo: Point;
    var up: Point;

    proc dom() { return {lo[0]..up[0],lo[1]..up[1],lo[2]..up[2]}; }

    iter points(param tag: iterKind) where tag == iterKind.standalone {
        forall point in dom() do yield point;
    }
    iter points() {
        for point in dom() do yield point;
    }
};

extern record BoxArray {
  var boxes : c_ptr(Box);
  var boxlimits : c_ptr(c_int);
  var size : c_uint;

  iter these(param tag: iterKind) where tag == iterKind.standalone
  {
    if boxes == nil then return;
    forall i in 0..<size do
        yield boxes[i];
  }
  iter these()
  {
    if boxes == nil then return;
    for i in 0..<size do
        yield boxes[i];
  }


}

extern record GrGeomSolid {
    var octree_bg_level, octree_ix, octree_iy, octree_iz: int;
    var interior_boxes: c_ptr(BoxArray);
    var surface_boxes : c_ptr(c_ptr(BoxArray));
    var patch_boxes : c_ptr(c_ptr(c_ptr(BoxArray)));

    proc interiorBoxes() { return interior_boxes[0]; }

    proc surfaceBoxes(face: int) {
        return surface_boxes[face][0];
    }

    proc patchBoxes(face: int, patchNum: int) { 
        return patch_boxes[face][patchNum][0]; 
    }
}



iter groundGeometryInteriorBoxes(ref groundGeometry: GrGeomSolid, 
                                 outerDom: domain(3, int(32))) {
    for box in groundGeometry.interiorBoxes() do
        for point in box.dom()[outerDom] do
            yield point;
}

iter groundGeometryInteriorBoxes(param tag: iterKind, 
                                 ref groundGeometry: GrGeomSolid, 
                                 outerDom: domain(3, int(32))) 
  where tag == iterKind.standalone {
    forall box in groundGeometry.interiorBoxes() do
        forall point in box.dom()[outerDom] do
            yield point;
}


iter groundGeometrySurfaceBoxes(ref groundGeometry: GrGeomSolid, 
                                outerDom: domain(3, int(32))) {
    for face in 0..<GrGeomOctreeNumFaces do 
        for box in groundGeometry.surfaceBoxes(face) do
            for (i,j,k) in box.dom()[outerDom] do
                yield (i,j,k,create_fdir(face));
}
iter groundGeometrySurfaceBoxes(param tag: iterKind,
                                ref groundGeometry: GrGeomSolid, 
                                outerDom: domain(3, int(32))) where tag == iterKind.standalone {
    forall face in 0..<GrGeomOctreeNumFaces do
        forall box in groundGeometry.surfaceBoxes(face) do
            forall (i,j,k) in box.dom()[outerDom] do
                yield (i,j,k,create_fdir(face));
}

iter groundGeometryPatchBoxes(ref groundGeometry: GrGeomSolid, patchNum: int, 
                              outerDom: domain(3, int(32))) {
    for face in 0..<GrGeomOctreeNumFaces do
        for box in groundGeometry.patchBoxes(face, patchNum) do
            for (i,j,k) in box.dom()[outerDom] do
                yield (i,j,k,face);
}
iter groundGeometryPatchBoxes(param tag: iterKind, ref groundGeometry: GrGeomSolid, patchNum: int, outerDom: domain(3, int(32)))
    where tag == iterKind.standalone
{
    forall face in 0..<GrGeomOctreeNumFaces do
        forall box in groundGeometry.patchBoxes(face, patchNum) do
            forall (i,j,k) in box.dom()[outerDom] do
                yield (i,j,k,face);
}