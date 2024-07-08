
use GroundGeometry;

use CTypes;
config param call_only = 0;
proc subvector_elt_index(x,y,z,ix,iy,iz,nx,ny) {
    return ((x - ix) + ((y - iy) + ((z - iz) *  ny)) * nx);
}

proc c2ChapelDomain(ix, iy, iz, nx, ny, nz) {
    const minPoint: Point = (ix:int(32),iy:int(32),iz:int(32));
    const maxPoint: Point = ( (ix+nx-1) :int(32), (iy+ny-1) :int(32), (iz+nz-1) :int(32));
    return {minPoint[0]..maxPoint[0],minPoint[1]..maxPoint[1],minPoint[2]..maxPoint[2]};
}

export proc calcfcn_compute_vang_curve_surface(ref grgeom:GrGeomSolid, r: int, 
    ix: int, iy: int, iz: int, nx: int, ny: int, nz: int, pr_sub: c_ptr(real), 
    pp_sub: c_ptr(real), pd_sub: c_ptr(real), alphas: c_ptr(real), ns: c_ptr(real), 
    gravity: real, region_idx: int, 
    ixv: int, iyv: int, izv: int, nxv: int, nyv: int) {
    
    const outerDomain:domain(3,int(32)) = c2ChapelDomain(ix,iy,iz,nx,ny,nz);
    for (i,j,k,fdir) in groundGeometrySurfaceBoxes(grgeom, outerDomain) {
        var idx = subvector_elt_index(i + fdir[0], j + fdir[1], k + fdir[2], ixv, iyv, izv, nxv, nyv);

        if (pp_sub[idx] >= 0.0) {
            pr_sub[idx] = 1.0;
        } else {
            var alpha = alphas[region_idx];
            var n = ns[region_idx];
            var m = 1.0 - (1.0 / n);
            const head = abs(pp_sub[idx]) / (pd_sub[idx] * gravity);
            const opahn = 1.0 + (alpha * head) ** n;
            const ahnm1 = (alpha * head) ** (n-1);
            const top = (1.0 - ahnm1 / (opahn ** m)) ** 2;
            const bottom = opahn ** (m / 2);
            pr_sub[idx] = top / bottom;
        }
    }
}

export proc calcder_compute_vang_curve_surface(ref grgeom:GrGeomSolid, r: int, 
    ix: int, iy: int, iz: int, nx: int, ny: int, nz: int, 
    pr_sub: c_ptr(real), pp_sub: c_ptr(real), pd_sub: c_ptr(real), alphas: c_ptr(real), ns: c_ptr(real), 
    gravity: real, region_idx: int, 
    ixv: int, iyv: int, izv: int, nxv: int, nyv: int) {
    
    const outerDomain:domain(3,int(32)) = c2ChapelDomain(ix,iy,iz,nx,ny,nz);
    for (i,j,k,fdir) in groundGeometrySurfaceBoxes(grgeom, outerDomain) {
        var idx = subvector_elt_index(i + fdir[0], j + fdir[1], k + fdir[2], ixv, iyv, izv, nxv, nyv);

        if (pp_sub[idx] >= 0.0) {
            pr_sub[idx] = 0.0;
        } else {
            var alpha = alphas[region_idx];
            var n = ns[region_idx];
            var m = 1.0 - (1.0 / n);

            const head = abs(pp_sub[idx]) / (pd_sub[idx] * gravity);
            const opahn = 1.0 + (alpha * head) ** n;
            const ahnm1 = (alpha * head) ** (n-1);
            const coeff = 1.0 - ahnm1 * (opahn ** -m);
            pr_sub[idx] =  2.0 * (coeff / ((opahn ** (m / 2))))
                                * ((n - 1) * ((alpha * head) ** (n - 2)) * alpha
                                * (opahn ** -m)
                                - ahnm1 * m * (opahn ** -(m + 1)) * n * alpha * ahnm1)
                                + (coeff ** 2) * (m / 2) * (opahn ** (-(m + 2) / 2))
                                * n * alpha * ahnm1;
        }
    }

}
export proc calcder_compute_vang_curve_interior(ref grgeom:GrGeomSolid, r: int,
 ix: int, iy: int, iz: int, nx: int, ny: int, nz: int,
 pr_sub: c_ptr(real), pp_sub: c_ptr(real), pd_sub: c_ptr(real),
 alphas: c_ptr(real), ns: c_ptr(real), gravity: real, region_idx: int,
 ixv: int, iyv: int, izv: int, nxv: int, nyv: int) {
    const outerDomain:domain(3,int(32)) = c2ChapelDomain(ix,iy,iz,nx,ny,nz);
    for (i,j,k) in groundGeometryInteriorBoxes(grgeom, outerDomain) {
        var idx = subvector_elt_index(i , j , k, ixv, iyv, izv, nxv, nyv);

        if (pp_sub[idx] >= 0.0) {
            pr_sub[idx] = 0.0;
        } else {
            var alpha = alphas[region_idx];
            var n = ns[region_idx];
            var m = 1.0 - (1.0 / n);

            const head = abs(pp_sub[idx]) / (pd_sub[idx] * gravity);
            const opahn = 1.0 + (alpha * head) ** n;
            const ahnm1 = (alpha * head) ** (n-1);
            const coeff = 1.0 - ahnm1 * (opahn ** -m);
            pr_sub[idx] =  2.0 * (coeff / ((opahn ** (m / 2))))
                                * ((n - 1) * ((alpha * head) ** (n - 2)) * alpha
                                * (opahn ** -m)
                                - ahnm1 * m * (opahn ** -(m + 1)) * n * alpha * ahnm1)
                                + (coeff ** 2) * (m / 2) * (opahn ** (-(m + 2) / 2))
                                * n * alpha * ahnm1;
        }
    }
}

export proc calcfcn_compute_vang_curve_interior(ref grgeom:GrGeomSolid, r: int,
ix: int, iy: int, iz: int, nx: int, ny: int, nz: int, 
pr_sub: c_ptr(real), pp_sub: c_ptr(real), pd_sub: c_ptr(real),
alphas: c_ptr(real), ns: c_ptr(real), gravity: real, region_idx: int,
ixv: int, iyv: int, izv: int, nxv: int, nyv: int) {
    const outerDomain:domain(3,int(32)) = c2ChapelDomain(ix,iy,iz,nx,ny,nz);
    forall (i,j,k) in groundGeometryInteriorBoxes(grgeom, outerDomain) {
        var idx = subvector_elt_index(i, j, k, ixv, iyv, izv, nxv, nyv);

        if (pp_sub[idx] >= 0.0) {
            pr_sub[idx] = 1.0;
        } else {
            var alpha = alphas[region_idx];
            var n = ns[region_idx];
            var m = 1.0 - (1.0 / n);

            const head = abs(pp_sub[idx]) / (pd_sub[idx] * gravity);
            const opahn = 1.0 + (alpha * head) ** n;
            const ahnm1 = (alpha * head) ** (n-1);
            const top = (1.0 - ahnm1 / (opahn ** m)) ** 2;
            const bottom = opahn ** (m / 2);
            pr_sub[idx] = top / bottom;
        }
    }
}
