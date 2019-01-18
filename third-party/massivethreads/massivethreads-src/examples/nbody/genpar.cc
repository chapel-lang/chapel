/*
 * genpar.cc
 */

#include "def.h"

#define MULT 1103515245
#define ADD 12345
#define MASK (0x7FFFFFFF)
#define TWOTO31 2147483648.0

static int A = 1;
static int B = 0;
static int randx = 1;
static int lastrand;   /* the last random number */

static int rand_inited = 0;

/*
 * XRAND: generate floating-point random number.
 */

t_real prand();

t_real xrand(t_real xl, t_real xh)
{
  t_real p = prand();
  t_real xx = xl + (xh - xl) * p;
  return xx;
}

void pranset(int seed)
{
  A = 1;
  B = 0;
  randx = (A*seed+B) & MASK;
  A = (MULT * A) & MASK;
  B = (MULT*B + ADD) & MASK;
}

/*
	Return a random t_real in [0, 1.0)
*/
t_real prand()
{
  if (rand_inited == 0) {
    rand_inited = 1;
    pranset(123456);
  }
  lastrand = randx;
  randx = (A*randx+B) & MASK;
  return((t_real)((double)lastrand/TWOTO31));
}

particle * make_particle(int id, t_real mass, t_real px, t_real py, t_real pz,
			 t_real vx, t_real vy, t_real vz)
{
  vect_t p = make_vect(px, py, pz);
  vect_t v = make_vect(vx, vy, vz);
  vect_t a = make_vect(0.0, 0.0, 0.0);
#if USE_MALLOC
  particle * pt = (particle *) malloc(sizeof(particle));
  pt->id = id;
  pt->mass = mass;
  pt->accel = a;
  pt->pos = p;
  pt->vel = v;
  return pt;
#else
  return new particle(id, mass, a, p, v);
#endif
}

vect_t pick_shell(t_real r)
{
  t_real x = 0.0, y = 0.0, z = 0.0, rsq = 0.0;
  do {
    x = xrand(-1.0, 1.0);
    y = xrand(-1.0, 1.0);
    z = xrand(-1.0, 1.0);
    rsq = x * x + y * y + z * z;
  } while (rsq <= 1.0);
  t_real rsc = r / sqrt(rsq);
  return make_vect(x / rsc, y / rsc, z / rsc);
}

void particle::centralize(t_real px, t_real py, t_real pz, 
			  t_real vx, t_real vy, t_real vz)
{
  VX(pos) += px; VY(pos) += py; VZ(pos) += pz;
  VX(vel) += vx; VY(vel) += vy; VZ(vel) += vz;
}

void centralize_particles(particle ** a, int n,
			  t_real px, t_real py, t_real pz,
			  t_real vx, t_real vy, t_real vz)
{
  for (int i = 0; i < n; i++)
    a[i]->centralize(px, py, pz, vx, vy, vz);
}
			   

const t_real mfrac = 0.999;
// because PI may be reserved
const t_real xxPI = 3.14159265358979323846;

particle ** generate_particles(int n)
{
#if USE_MALLOC
  particle ** a = (particle **) malloc(sizeof(particle *) * n);
#else
	particle ** a = new particle* [n];
#endif
	t_real rsc = 9.0 * xxPI / 16.0;
	t_real vsc = sqrt(rsc);
	t_real cmrx = 0.0, cmry = 0.0, cmrz = 0.0;
	t_real cmvx = 0.0, cmvy = 0.0, cmvz = 0.0;
	int h = (n + 1) / 2;
	t_real mass = 1.0 / (t_real)n;
	t_real offs = 4.0;
	for (int i = 0; i < h; i++) {
		int j = h + i; t_real r = 0.0;
		do {
	  	t_real xr = xrand(0.0, mfrac);
			t_real po = pow(xr, -2.0/3.0);
			t_real rs = sqrt(po - 1.0);
			r = 1.0 / rs;
		} while (r > 9.0);
		vect_t pos = pick_shell(r);
		cmrx += (VX(pos) + VX(pos) + offs);
		cmry += (VY(pos) + VY(pos) + offs);
		cmrz += (VZ(pos) + VZ(pos) + offs);
		t_real x = 0.0, y = 0.0;
		do {
		    x = xrand (0.0, 1.0); y = xrand(0.0, 0.1);
		} while (y > x * x * pow(1.0 - x * x, 3.5));
		t_real v = sqrt(2.0) * x / pow(1.0 + r * r, 0.25);
		vect_t vel = pick_shell(vsc * v);
		cmvx += (VX(vel) + VX(vel));
		cmvy += (VY(vel) + VY(vel));
		cmvz += (VZ(vel) + VZ(vel));
		a[i] = make_particle(i, mass, VX(pos), VY(pos), VZ(pos), 
			 VX(vel), VY(vel), VZ(vel));
		if (j < n) {
	  		a[j] = make_particle(j, mass, 
			   VX(pos) + offs, VY(pos) + offs, VZ(pos) + offs, 
			   VX(vel), VY(vel), VZ(vel));
		}
	}
	centralize_particles(a, n, cmrx / n, cmry / n, cmrz / n, 
			cmvx / n, cmvy / n, cmvz / n);
    
  for (int i = 0; i < n; i++)
    printf("%f %f %f\n", VX(a[i]->pos), VY(a[i]->pos), VZ(a[i]->pos));
	
  return a;
}

particle ** generate_particles_uniform(int n)
{
#if USE_MALLOC
  particle ** a = (particle **) malloc(sizeof(particle *) * n);
#else
	particle ** a = new particle* [n];
#endif
	t_real rsc = 9.0 * xxPI / 16.0;
	t_real vsc = sqrt(rsc);
	t_real mass = 1.0 / (t_real)n;
  for (int i = 0; i < n; i++) {
    vect_t pos = pick_shell(rsc);
    vect_t vel = pick_shell(vsc);
		a[i] = make_particle(i, mass, VX(pos), VY(pos), VZ(pos), 
			 VX(vel), VY(vel), VZ(vel));
    //printf("%f %f %f\n", a[i]->pos.x, a[i]->pos.y, a[i]->pos.z);
  }
	return a;
}

void particle::dump()
{
  printf ("id: %d pos: %f %f %f vel: %f %f %f\n",
	  id, VX(pos), VY(pos), VZ(pos), 
	  VX(vel), VY(vel), VZ(vel));
}

void dump_particles(particle ** particles, int n)
{
  for (int i = 0; i < n; i++) particles[i]->dump();
}
