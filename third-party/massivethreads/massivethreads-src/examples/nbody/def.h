/*
 * def.h
 */
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <pthread.h>

#ifndef REAL_TYPE
#define REAL_TYPE float
#endif
typedef REAL_TYPE t_real;

/* Use malloc does not have much performance gain? */
#ifndef USE_MALLOC
#define USE_MALLOC 1
#endif

#ifndef USE_MORTON
#define USE_MORTON 1
#endif

#ifndef UNBOX_VECT 
#define UNBOX_VECT 1
#endif

#ifndef BUILD_TREE_PARALLEL 
#define BUILD_TREE_PARALLEL 1
#endif

struct vect
{
#if UNBOX_VECT
  void * operator new(size_t sz) {
    printf("do not call vect::new\n");
	exit(1);
  }
#endif /* UNBOX_VECT */
  vect (t_real xx, t_real yy, t_real zz) {
    x = xx; y = yy; z = zz;
  }
  vect () { }
  t_real x, y, z;
};

#if UNBOX_VECT
#define vect_t vect
#define VX(v) (v).x
#define VY(v) (v).y
#define VZ(v) (v).z
static inline vect_t make_vect (t_real x, t_real y, t_real z)
{
    return vect (x, y, z);
}
#else
#define vect_t vect *
#define VX(v) (v)->x
#define VY(v) (v)->y
#define VZ(v) (v)->z
static inline vect_t make_vect (t_real x, t_real y, t_real z)
{
#if USE_MALLOC
    vect_t p = (vect_t) malloc(sizeof(vect));
    p->x = x; p->y = y; p->z = z;
    return p;
#else
  vect_t p = new vect (x, y, z);
#endif /* USE_MALLOC */
  return p;
}
#endif

static inline vect_t v_plus_v (vect_t v0, vect_t v1) 
{
  return make_vect(VX(v0) + VX(v1), VY(v0) + VY(v1), VZ(v0) + VZ(v1));
}

static inline vect_t v_minus_v (vect_t v0, vect_t v1) 
{
  return make_vect(VX(v0) - VX(v1), VY(v0) - VY(v1), VZ(v0) - VZ(v1));
}

void print_vect(vect_t);
static inline vect_t k_times_v(t_real k, vect_t v) {
  return make_vect (k * VX(v), k * VY(v), k * VZ(v));
}
static inline vect_t v_div_k (t_real k, vect_t v) {
  return make_vect (VX(v) / k, VY(v) / k, VZ(v) / k);
}

struct rectangle
{
  vect_t ll;
  vect_t ur;
  rectangle(vect_t ll_, vect_t ur_) {
    ll = ll_; ur = ur_;
  }
};

struct mass_momentum 
{
  void * operator new(size_t sz) {
    printf("do not call mass_momentum::new\n");
  }
  mass_momentum (t_real ma, vect_t mo, int nn) { 
    mass = ma; momentum = mo; n_nodes = nn; 
  }
  t_real mass;
  vect_t momentum;
  int n_nodes;
};

enum space_state { NO_PARTICLE, ONE_PARTICLE, MULTIPLE_PARTICLES };

#define N_CHILDREN 8

struct morton_idx
{
  unsigned long low;
  unsigned long high;
};

struct particle;
struct space
{
  space_state state;
  space ** subspaces;
  
  t_real mass;
  vect_t cg;
  rectangle * area;
  t_real diameter2;
  morton_idx midx;
  
  int cached;
  space (space_state s, t_real m, vect_t c, rectangle * a, t_real d) {
    state = s; mass = m; cg = c; area = a; diameter2 = d;
    cached = 0;
  }
  void add_particle(t_real, vect_t);
  void add_particle_morton(unsigned long, t_real, vect_t);
  void divide();
  void divide_morton();
  mass_momentum set_mass_and_cg();
  vect_t calc_accel(vect_t);
  vect_t calc_accel1(vect_t);
  vect_t calc_accel_morton(vect_t);
};

struct particle_data
{
  int id;
  t_real mass;
  t_real px, py, pz;
  t_real vx, vy, vz;
  particle_data (int id_, t_real mass_, t_real px_, t_real py_, t_real pz_, 
		 t_real vx_, t_real vy_, t_real vz_) {
    id = id_; mass = mass_; px = px_; py = py_; pz = pz_;
    vx = vx_; vy = vy_; vz = vz_;
  }
};

struct particle
{
  int id;
  t_real mass;
  vect_t accel;
  vect_t pos;
  vect_t vel;
  unsigned long mid; /* morton id */
  particle (int id_, t_real mass_, vect_t accel_, vect_t pos_, vect_t vel_) {
    id = id_; mass = mass_; accel = accel_;
    pos = pos_; vel = vel_;
  }
  void dump();		/* dump content of the particle */
  void set_accel(space *);
  void move(t_real);
  void centralize(t_real, t_real, t_real, t_real, t_real, t_real);
  t_real calc_limit();
};

void print_rectangle(rectangle *);

static inline t_real edge(rectangle * rec)
{
  return (VX(rec->ur) - VX(rec->ll));
}

static inline t_real distance2(vect_t p0, vect_t p1)
{
  t_real dx = VX (p1) - VX (p0);
  t_real dy = VY (p1) - VY (p0);
  t_real dz = VZ (p1) - VZ (p0);
  return dx * dx + dy * dy + dz * dz;
}

static inline t_real rabs(t_real x)
{
  return x < 0.0 ? -x : x;
}

static inline t_real distance(vect_t p0, vect_t p1)
{
  return sqrt(distance2(p0, p1));
}

void morton_sort(particle **, int);
particle ** generate_particles(int);
particle ** generate_particles_uniform(int);
space * make_empty_space(rectangle *);
space * make_empty_space_morton(t_real, unsigned long, unsigned long);
rectangle * make_entire_rectangle(t_real);
space * generate_tree(particle **, int);
space * build_tree(particle **, int);
void free_tree(space *);
space * build_tree_bottomup(particle **, int);
space * build_tree_morton(particle **, int);
void set_accels(particle **, int, space *);
void move_particles(particle **, int, t_real);
t_real diameter2(rectangle *);

unsigned long vect2morton(vect_t);
int cover_p(rectangle *, vect_t);
t_real distance2(vect_t, vect_t);
int select_covering_rectangle(vect_t, rectangle *);

int current_real_time_micro(void);
int current_real_time_milli(void);
