/*
 * def.cc
 */
#include <sys/types.h>
#include <sys/time.h>
/* #include <sys/timeb.h> */

#include "def.h"

void print_vect(vect_t v)
{
  printf ("%f %f %f", VX (v), VY (v), VZ (v));
}

void print_rectangle(rectangle * rec)
{
  vect_t ll = rec->ll;
  vect_t ur = rec->ur;
  printf ("((%f %f %f) (%f %f %f))", 
	  VX (ll), VY (ll), VZ (ll), 
	  VX (ur), VY (ur), VZ (ur));
}

t_real diameter2(rectangle * rec)
{
  vect_t l = rec->ll;
  vect_t r = rec->ur;
  t_real x0 = VX(l), y0 = VY(l), z0 = VZ(l);
  t_real x1 = VX(r), y1 = VY(r), z1 = VZ(r);
  t_real dx = x1 - x0, dy = y1 - y0, dz = z1 - z0;
  return (dx * dx + dy * dy + dz * dz);
}

int cover_p(rectangle * rec, vect_t pos)
{
  vect_t ll = rec->ll;
  vect_t ur = rec->ur;
  t_real lx = VX (ll), ly = VY (ll), lz = VZ (ll);
  t_real rx = VX (ur), ry = VY (ur), rz = VZ (ur);
  t_real x = VX (pos), y = VY (pos), z = VZ (pos);
  return (lx <= x && ly <= y && lz <= z && x < rx && y < ry && z < rz);
}

rectangle * make_entire_rectangle(t_real a)
{
  vect_t l = make_vect(-a, -a, -a);
  vect_t r = make_vect(a, a, a);
#if USE_MALLOC
  rectangle * rc = (rectangle *) malloc(sizeof(rectangle));
  rc->ll = l;
  rc->ur = r;
  return rc;
#else
  return new rectangle(l, r);
#endif
}

space * make_empty_space(rectangle * area)
{
#if USE_MALLOC
  space * sp = (space *) malloc(sizeof(space));
  sp->state = NO_PARTICLE;
  sp->mass = 0.0;
  sp->cg = make_vect(0.0, 0.0, 0.0);
  sp->area = area;
  sp->diameter2 = diameter2(area);
  return sp;
#else
 return new space(NO_PARTICLE, 0.0, make_vect(0.0, 0.0, 0.0), 
			area, diameter2(area));
#endif
}

space * make_empty_space_morton(t_real diameter, unsigned long midx_low,
  unsigned long midx_high)
{
#if USE_MALLOC
  space * sp = (space *) malloc(sizeof(space));
  sp->state = NO_PARTICLE;
  sp->mass = 0.0;
  sp->cg = make_vect(0.0, 0.0, 0.0);
  sp->diameter2 = diameter;
  sp->midx.low = midx_low;
  sp->midx.high = midx_high;
  return sp;
#else
#error "make_empty_space_morton not implemented yet"
#endif
}

int current_real_time_micro()
{
  struct timeval tp[1];
  struct timezone tzp[1];
  gettimeofday (tp, tzp);
  return tp->tv_sec * 1000000 + tp->tv_usec;
}

int current_real_time_milli()
{
  return current_real_time_micro() / 1000;
}
