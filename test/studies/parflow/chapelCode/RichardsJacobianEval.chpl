use Math;
use GroundGeometry;
use CTypes;
use IO;
use PhaseRelPerm;
use BoundaryConditions;


proc mean(a,b) { return (a+b) / 2;}

proc harmonic_mean(a,b) {
    if (a + b == 0) {
        return 0;
    } else {
        return (2 * a * b) / (a + b);
    }
}

proc harmonic_mean_dz(a,b,c,d) {
    if((c*b) + (a*d) != 0) {
        return (((c+d) * a*b) / ((b*c) + (a*d)));
    } else {
        return 0;
    }
}

proc upstream_mean(a,b,c,d) {if (a - b) >= 0 then return c; else return d;}

proc Mean(a,b) {return mean(a,b);}

proc PMean(a,b,c,d) { return harmonic_mean(c,d);}

proc PMeanDZ(a,b,c,d) {return harmonic_mean_dz(a,b,c,d);}

proc RPMean(a,b,c,d) {return upstream_mean(a,b,c,d);}

export proc richards_gravity_and_second_order_derivative_interior(ref gr_domain: GrGeomSolid,
r: int, ix: int, iy: int, iz: int, nx: int, ny: int, nz: int, //args for the iteration
 pp: c_ptr(real), dp: c_ptr(real), rpp: c_ptr(real), ddp: c_ptr(real), rpdp: c_ptr(real), //presure, density, relperm, derivatives therein
permxp: c_ptr(real), permyp: c_ptr(real), permzp: c_ptr(real), //permeabilities
fb_x : c_ptr(real), fb_y: c_ptr(real), fb_z: c_ptr(real), //FB data (not exactly sure what FB means)
x_ssl: c_ptr(real), y_ssl: c_ptr(real), z_mult: c_ptr(real), //x and y slopes and variable dz
J: c_ptr(real), cp: c_ptr(real), wp: c_ptr(real), ep: c_ptr(real), sop: c_ptr(real), np: c_ptr(real), lp: c_ptr(real), up: c_ptr(real), // jacobian submatrix stencils
grid2d_iz: int, dx: int, dy: int, dz: int, dt: int, sy_v: int, sz_v: int, sy_m: int, sz_m: int,//scalar parameters, sizes, etc
tfgupwind: int, gravity: real, viscosity: real, // other scalars
pix: int, piy: int, piz: int, pnx: int, pny: int,
jix: int, jiy: int, jiz: int, jnx: int, jny: int,
six: int, siy: int, siz: int, snx: int, sny: int,
symm_part: int
) {
    const ffx = dy * dz;
    const ffy = dx * dz;
    const ffz = dx * dy;
    const outerDomain: domain(3,int(32)) = c2ChapelDomain(ix,iy,iz,nx,ny,nz);
    for (i,j,k) in groundGeometryInteriorBoxes(gr_domain, outerDomain) {
        const ip = subvector_elt_index(i,j,k,pix,piy,piz,pnx,pny);
        const im = subvector_elt_index(i,j,k,jix,jiy,jiz,jnx,jny);
        const ioo = subvector_elt_index(i,j,grid2d_iz,six,siy,siz,snx,sny);

        const prod = rpp[ip] * dp[ip];
        const prod_der = rpdp[ip] * dp[ip] + rpp[ip] * ddp[ip];

        const prod_rt = rpp[ip + 1] * dp[ip + 1];
        const prod_rt_der = rpdp[ip + 1] * dp[ip + 1] + rpp[ip + 1] * ddp[ip + 1];

        const prod_no = rpp[ip + sy_v] * dp[ip + sy_v];
        const prod_no_der = rpdp[ip + sy_v] * dp[ip + sy_v]
                + rpp[ip + sy_v] * ddp[ip + sy_v];

        const prod_up = rpp[ip + sz_v] * dp[ip + sz_v];
        const prod_up_der = rpdp[ip + sz_v] * dp[ip + sz_v]
                + rpp[ip + sz_v] * ddp[ip + sz_v];

        var x_dir_g: real = nan;
        var x_dir_g_c: real = nan;
        var y_dir_g: real = nan;
        var y_dir_g_c: real = nan;

        select tfgupwind {
            when 0 {
                x_dir_g = mean(gravity * sin(atan(x_ssl[ioo])), gravity * sin(atan(x_ssl[ioo + 1])));
                x_dir_g_c = mean(gravity * cos(atan(x_ssl[ioo])), gravity * cos(atan(x_ssl[ioo + 1])));
                y_dir_g = mean(gravity * sin(atan(y_ssl[ioo])), gravity * sin(atan(y_ssl[ioo + sy_v])));
                y_dir_g_c = mean(gravity * cos(atan(y_ssl[ioo])), gravity * cos(atan(y_ssl[ioo + sy_v])));
            }
            when 1 {
                x_dir_g = gravity * sin(atan(x_ssl[ioo]));
                x_dir_g_c = gravity * cos(atan(x_ssl[ioo]));
                y_dir_g = gravity * sin(atan(y_ssl[ioo]));
                y_dir_g_c = gravity * cos(atan(y_ssl[ioo]));
            }
            when 2 {
                x_dir_g = x_ssl[ioo];
                x_dir_g_c = 1.0;
                y_dir_g = y_ssl[ioo];
                y_dir_g_c = 1.0;
            }
        }

        var diff = pp[ip] - pp[ip + 1];
        var updir = (diff / dx) * x_dir_g_c - x_dir_g;

        var x_coeff = fb_x[ip] * dt * ffx * (1.0 / dx) * z_mult[ip]
            * PMean(pp[ip], pp[ip+1], permxp[ip], permxp[ip + 1])
            / viscosity;
        var sym_west_temp = (-x_coeff
                    * RPMean(updir, 0.0, prod, prod_rt)) * x_dir_g_c; //@RMM TFG contributions, sym


        var west_temp = (-x_coeff * diff
                    * RPMean(updir, 0.0, prod_der, 0.0)) * x_dir_g_c
                    + sym_west_temp;

        west_temp += (x_coeff * dx * RPMean(updir, 0.0, prod_der, 0.0)) * x_dir_g; //@RMM TFG contributions, non sym

        var sym_east_temp = (-x_coeff
                        * RPMean(updir, 0.0, prod, prod_rt)) * x_dir_g_c; //@RMM added sym TFG contributions

        var east_temp = (x_coeff * diff
                    * RPMean(updir, 0.0, 0.0, prod_rt_der)) * x_dir_g_c
                    + sym_east_temp;

        east_temp += -(x_coeff * dx * RPMean(updir, 0.0, 0.0, prod_rt_der)) * x_dir_g; //@RMM  TFG contributions non sym

        /* diff >= 0 implies flow goes south to north */
        diff = pp[ip] - pp[ip + sy_v];
        updir = (diff / dy) * y_dir_g_c - y_dir_g;


        /* multiply y_coeff by FB in y */
        var y_coeff = fb_y[ip] * dt * ffy * (1.0 / dy) * z_mult[ip]
                    * PMean(pp[ip], pp[ip + sy_v], permyp[ip], permyp[ip + sy_v])
                    / viscosity;

        var sym_south_temp = -y_coeff
                        * RPMean(updir, 0.0, prod, prod_no) * y_dir_g_c; //@RMM TFG contributions, SYMM

        var south_temp = -y_coeff * diff
                    * RPMean(updir, 0.0, prod_der, 0.0) * y_dir_g_c
                    + sym_south_temp;

        south_temp += (y_coeff * dy * RPMean(updir, 0.0, prod_der, 0.0)) * y_dir_g; //@RMM TFG contributions, non sym


        var sym_north_temp = y_coeff
                        * -RPMean(updir, 0.0, prod, prod_no) * y_dir_g_c; //@RMM  TFG contributions non SYMM

        var north_temp = y_coeff * diff
                    * RPMean(updir, 0.0, 0.0,
                                prod_no_der) * y_dir_g_c
                    + sym_north_temp;

        north_temp += -(y_coeff * dy * RPMean(updir, 0.0, 0.0, prod_no_der)) * y_dir_g; //@RMM  TFG contributions non sym

        var sep = (dz * Mean(z_mult[ip], z_mult[ip + sz_v]));
        /* diff >= 0 implies flow goes lower to upper */


        var lower_cond = pp[ip] / sep - (z_mult[ip] / (z_mult[ip] + z_mult[ip + sz_v])) * dp[ip] * gravity;

        var upper_cond = pp[ip + sz_v] / sep + (z_mult[ip + sz_v] / (z_mult[ip] + z_mult[ip + sz_v])) * dp[ip + sz_v] * gravity;


        diff = lower_cond - upper_cond;

        /* multiply z_coeff by FB in z */
        var z_coeff = fb_z[ip] * dt * ffz
                    * PMeanDZ(permzp[ip], permzp[ip + sz_v], z_mult[ip], z_mult[ip + sz_v])
                    / viscosity;

        var sym_lower_temp = -z_coeff * (1.0 / (dz * Mean(z_mult[ip], z_mult[ip + sz_v])))
                        * RPMean(lower_cond, upper_cond, prod,
                                    prod_up);

        var lower_temp = -z_coeff
                    * (diff * RPMean(lower_cond, upper_cond, prod_der, 0.0)
                        + (-gravity * 0.5 * dz * (Mean(z_mult[ip], z_mult[ip + sz_v])) * ddp[ip]
                            * RPMean(lower_cond, upper_cond, prod,
                                    prod_up)))
                    + sym_lower_temp;

        var sym_upper_temp = z_coeff * (1.0 / (dz * Mean(z_mult[ip], z_mult[ip + sz_v])))
                        * -RPMean(lower_cond, upper_cond, prod,
                                    prod_up);

        var upper_temp = z_coeff
                    * (diff * RPMean(lower_cond, upper_cond, 0.0,
                                        prod_up_der)
                        + (-gravity * 0.5 * dz * (Mean(z_mult[ip], z_mult[ip + sz_v])) * ddp[ip + sz_v]
                            * RPMean(lower_cond, upper_cond, prod,
                                    prod_up)))
                    + sym_upper_temp;
        cp[im] += -(west_temp + south_temp + lower_temp);
        cp[im + 1] += -east_temp;
        cp[im + sy_m] += -north_temp;
        cp[im + sz_m] += -upper_temp;
        if(!symm_part) {
            ep[im] += east_temp;
            np[im] += north_temp;
            up[im] += upper_temp;

            wp[im + 1] += west_temp;
            sop[im + sy_m] += south_temp;
            lp[im + sz_m] += lower_temp;
        } else {
            ep[im] += sym_east_temp;
            np[im] += sym_north_temp;
            up[im] += sym_upper_temp;
        }
    }
}

export proc bc_all_correction (ival: int, in bc_struct: BCStruct, ipatch: int, is: int,
ref gr_domain: GrGeomSolid,
r: int, ix: int, iy: int, iz: int, nx: int, ny: int, nz: int, //args for the iteration
 pp: c_ptr(real), dp: c_ptr(real), rpp: c_ptr(real), ddp: c_ptr(real), rpdp: c_ptr(real), //presure, density, relperm, derivatives therein
permxp: c_ptr(real), permyp: c_ptr(real), permzp: c_ptr(real), //permeabilities
z_mult_dat: c_ptr(real), //x and y slopes and variable dz
cp: c_ptr(real), wp: c_ptr(real), ep: c_ptr(real), sop: c_ptr(real), np: c_ptr(real), lp: c_ptr(real), up: c_ptr(real), // jacobian submatrix stencils
dx: int, dy: int, dz: int, dt: int, sy_v: int, sz_v: int, sy_m: int, sz_m: int,//scalar parameters, sizes, etc
ffx: int, ffy: int, ffz: int,
gravity: real, viscosity: real, // other scalars
pix: int, piy: int, piz: int, pnx: int, pny: int,
jix: int, jiy: int, jiz: int, jnx: int, jny: int,
bc_patch_values: c_ptr(real)
) {
    if (bc_struct.bc_types[ipatch] != BC_ALL && bc_struct.bc_types[ipatch] != BC_ALL) then return;

    const patch_index = bc_struct.patch_indexes[ipatch];
    
    const outerDomain: domain(3,int(32)) = c2ChapelDomain(ix,iy,iz,nx,ny,nz);
    for (i,j,k,f) in groundGeometryPatchBoxes(gr_domain, patch_index, outerDomain) {
        const ip = subvector_elt_index(i,j,k,pix,piy,piz,pnx,pny);
        const im = subvector_elt_index(i,j,k,jix,jiy,jiz,jnx,jny);
        const value = bc_patch_values[ival];

        const prod = rpp[ip] * dp[ip];
        select f {
            when GrGeomOctreeFaceL {
                const diff = pp[ip - 1] - pp[ip];
                const prod_der = rpdp[ip - 1] * dp[ip - 1] + rpp[ip - 1] * ddp[ip - 1];
                const coeff = dt * z_mult_dat[ip] * ffx * (1.0 / dx)
                        * PMean(pp[ip - 1], pp[ip], permxp[ip - 1], permxp[ip])
                        / viscosity;
                wp[im] = -coeff * diff
                        * RPMean(pp[ip - 1], pp[ip], prod_der, 0.0);
            }
            when GrGeomOctreeFaceR {
                const diff = pp[ip] - pp[ip + 1];
                const prod_der = rpdp[ip + 1] * dp[ip + 1] + rpp[ip + 1] * ddp[ip + 1];
                const coeff = dt * z_mult_dat[ip] * ffx * (1.0 / dx)
                        * PMean(pp[ip], pp[ip + 1], permxp[ip], permxp[ip + 1])
                        / viscosity;
                ep[im] = coeff * diff
                        * RPMean(pp[ip], pp[ip + 1], 0.0, prod_der);
            }
            when GrGeomOctreeFaceD {
                const diff = pp[ip - sy_v] - pp[ip];
                const prod_der = rpdp[ip - sy_v] * dp[ip - sy_v]
                            + rpp[ip - sy_v] * ddp[ip - sy_v];
                const coeff = dt * z_mult_dat[ip] * ffy * (1.0 / dy)
                        * PMean(pp[ip - sy_v], pp[ip],
                                permyp[ip - sy_v], permyp[ip])
                        / viscosity;
                sop[im] = -coeff * diff
                            * RPMean(pp[ip - sy_v], pp[ip], prod_der, 0.0);
            }
            when GrGeomOctreeFaceU {
                const diff = pp[ip] - pp[ip + sy_v];
                const prod_der = rpdp[ip + sy_v] * dp[ip + sy_v]
                            + rpp[ip + sy_v] * ddp[ip + sy_v];
                const coeff = dt * z_mult_dat[ip] * ffy * (1.0 / dy)
                        * PMean(pp[ip], pp[ip + sy_v],
                                permyp[ip], permyp[ip + sy_v])
                        / viscosity;
                np[im] = -coeff * diff
                        * RPMean(pp[ip], pp[ip + sy_v], 0.0, prod_der);
            }

            when GrGeomOctreeFaceB {
                const lower_cond = (pp[ip - sz_v]) - 0.5 * dz
                            * Mean(z_mult_dat[ip], z_mult_dat[ip - sz_v])
                            * dp[ip - sz_v] * gravity;
                const upper_cond = (pp[ip]) + 0.5 * dz * Mean(z_mult_dat[ip], z_mult_dat[ip - sz_v])
                            * dp[ip] * gravity;
                const diff = lower_cond - upper_cond;
                const prod_der = rpdp[ip - sz_v] * dp[ip - sz_v]
                            + rpp[ip - sz_v] * ddp[ip - sz_v];
                const prod_lo = rpp[ip - sz_v] * dp[ip - sz_v];
                const coeff = dt * ffz * (1.0 / (dz * Mean(z_mult_dat[ip], z_mult_dat[ip - sz_v])))
                        * PMeanDZ(permzp[ip - sz_v], permzp[ip],
                                    z_mult_dat[ip - sz_v], z_mult_dat[ip])
                        / viscosity;
                lp[im] = -coeff *
                        (diff * RPMean(lower_cond, upper_cond,
                                        prod_der, 0.0)
                            - gravity * 0.5 * dz
                            * Mean(z_mult_dat[ip], z_mult_dat[ip - sz_v]) * ddp[ip]
                            * RPMean(lower_cond, upper_cond, prod_lo, prod));
            }

            when GrGeomOctreeFaceF {
                const lower_cond = (pp[ip]) - 0.5 * dz
                            * Mean(z_mult_dat[ip], z_mult_dat[ip + sz_v])
                            * dp[ip] * gravity;
                const upper_cond = (pp[ip + sz_v]) + 0.5 * dz
                            * Mean(z_mult_dat[ip], z_mult_dat[ip + sz_v])
                            * dp[ip + sz_v] * gravity;
                const diff = lower_cond - upper_cond;
                const prod_der = rpdp[ip + sz_v] * dp[ip + sz_v]
                            + rpp[ip + sz_v] * ddp[ip + sz_v];
                const prod_up = rpp[ip + sz_v] * dp[ip + sz_v];
                const coeff = dt * ffz * (1.0 / (dz * Mean(z_mult_dat[ip], z_mult_dat[ip + sz_v])))
                        * PMeanDZ(permzp[ip], permzp[ip + sz_v],
                                    z_mult_dat[ip], z_mult_dat[ip + sz_v])
                        / viscosity;
                up[im] = -coeff *
                        (diff * RPMean(lower_cond, upper_cond,
                                        0.0, prod_der)
                            - gravity * 0.5 * dz
                            * (Mean(z_mult_dat[ip], z_mult_dat[ip + sz_v])) * ddp[ip]
                            * RPMean(lower_cond, upper_cond, prod, prod_up));

            }
        }//select on face
    }
}