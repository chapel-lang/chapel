/* 
 * gentree.cc
 */

#include <mcheck.h>
#include "def.h"

#define MORTON_LEVEL 9

t_real g_stride;
unsigned long g_morton_len;
rectangle * g_limit_area;
int n_buildtree = 0;

inline unsigned int bitspread( unsigned int n ) {
  if (n > 65536) {
    fprintf(stderr, "too many cells\n");
    exit(1);
  };
  n = (n | (n << 16)) & 0x030000FF;
  n = (n | (n <<  8)) & 0x0300F00F;
  n = (n | (n <<  4)) & 0x030C30C3;
  n = (n | (n <<  2)) & 0x09249249;
  /*
  n = (n | (n << 8)) & 0x00ff00ff;
  n = (n | (n << 4)) & 0x0f0f0f0f;
  n = (n | (n << 2)) & 0x33333333;
  n = (n | (n << 1)) & 0x55555555;
  */
  return n;
}

static inline unsigned long morton_id(unsigned int x, unsigned int y, unsigned int z)
{
  return bitspread(x) | bitspread(y) << 1 | bitspread(z) << 2;
}

unsigned long vect2morton(vect_t vec)
{
  unsigned int mx, my, mz;
  mx = (unsigned int) floor((VX(vec) - VX(g_limit_area->ll)) / g_stride);
  my = (unsigned int) floor((VY(vec) - VY(g_limit_area->ll)) / g_stride);
  mz = (unsigned int) floor((VZ(vec) - VZ(g_limit_area->ll)) / g_stride);
  return morton_id(mx, my, mz);
}

struct qsrt_thread_dat {
  particle ** p;
  int left;
  int right;
};

void * pqsort_particles(void *args)
{
  pthread_t tha, thb;
  qsrt_thread_dat *dta, *dtb;
  int i, j, left, right;
  unsigned int pivot;
  particle **pars, *tmp;

  qsrt_thread_dat * p = (qsrt_thread_dat *) args;
 
  pars = p->p;
  i = left = p->left;
  j = right = p->right;
  free(p);
  
  pivot = pars[(left + right) / 2]->mid;

  while(1) {
    while (pars[i]->mid < pivot) i++;
    while (pivot < pars[j]->mid) j--;
    if (i >= j) break;
    tmp = pars[i];
    pars[i] = pars[j];
    pars[j] = tmp;
    i++;
    j--;
  }
  
  if (left < i - 1) {
    dta = (qsrt_thread_dat *) malloc(sizeof(qsrt_thread_dat));
    if (dta == NULL) {
      perror("malloc failed\n");
      exit(1);
    }
    dta->p = pars;
    dta->left = left;
    dta->right = i - 1;
    pthread_create(&tha, NULL, pqsort_particles, dta);
    //pqsort_particles(dta);
  }

  if (j + 1 < p->right) {
    dtb = (qsrt_thread_dat *) malloc(sizeof(qsrt_thread_dat));
    if (dtb == NULL) {
      perror("malloc failed\n");
      exit(1);
    }
    dtb->p = pars;
    dtb->left = j + 1;
    dtb->right = right;
    //pthread_create(&thb, NULL, pqsort_particles, dtb);
    pqsort_particles(dtb);
  }
}

inline void qsort_particles(particle ** particles, int left, int right)
{
  int i, j;
  unsigned int pivot;

  i = left;
  j = right;
  pivot = particles[(i + j) / 2]->mid;

  while (1) {
    while (particles[i]->mid < pivot) i++;
    while (pivot < particles[j]->mid) j--;
    if (i >= j) break;
    particle * tmp = particles[i];
    particles[i] = particles[j];
    particles[j] = tmp;
    i++;
    j--;
  }
  if (left < i - 1)
    qsort_particles(particles, left, i - 1);
  if (j + 1 < right)
    qsort_particles(particles, j + 1, right);
}

void morton_sort(particle ** particles, int n_particles)
{
  int i, j;
 
  g_stride = fabs(VX(g_limit_area->ur) - VX(g_limit_area->ll)) / 
    pow(2, MORTON_LEVEL);
  // Update morton id
  for (i = 0; i < n_particles; i++) {
    particles[i]->mid = vect2morton(particles[i]->pos);
    //printf("%i (%f %f %f) -> (%u, %u, %u) -> %lu\n", i, VX(particles[i]->pos), VY(particles[i]->pos),
    // VZ(particles[i]->pos), mx, my, mz, particles[i]->mid);
  }

  // Quicksort particles in place by morton id
#if 1
  qsort_particles(particles, 0, n_particles - 1);
#else
  qsrt_thread_dat * dat;
  dat = (qsrt_thread_dat *) malloc(sizeof(qsrt_thread_dat));
  if (dat == NULL) {
    perror("malloc failed\n");
    exit(1);
  }
  dat->p = particles;
  dat->left = 0;
  dat->right = n_particles - 1;
  pqsort_particles(dat);
#endif
  /*
  for (i = 0; i < n_particles; i++) {
    //printf("%i %f %f %f\n", i, VX(particles[i]->pos), VY(particles[i]->pos),
    // VZ(particles[i]->pos));
    printf("%i morton id=%u\n", i, particles[i]->mid);
  }
  */
  g_morton_len = vect2morton(g_limit_area->ur);
  //printf("- morton id=%u\n", morton_id(mx, my, mz));
}

int select_covering_rectangle(vect_t pos, rectangle * rec)
{
  if (cover_p(rec, pos)) {
    vect_t ll = rec->ll; vect_t ur = rec->ur;
    t_real x = VX(pos), y = VY(pos), z = VZ(pos);
    t_real lx = VX(ll), ly = VY(ll), lz = VZ(ll);
    t_real rx = VX(ur), ry = VY(ur), rz = VZ(ur);
    t_real cx = (lx + rx) * 0.5, cy = (ly + ry) * 0.5, cz = (lz + rz) * 0.5;
    int r = 0;
    if (x >= cx) r += 4;
    if (y >= cy) r += 2;
    if (z >= cz) r += 1;
    return r;
  } else {
    printf("ERROR: select_covering_rectangle\nRectangle: ");
    print_rectangle(rec);
    printf("\nPOS: ");
    print_vect(pos);
    printf("\n");
    return 0;
  }
}

rectangle * make_sub_rectangle(rectangle * parent, int idx)
{
  vect_t ll = parent->ll; vect_t ur = parent->ur;
  t_real lx = VX(ll), ly = VY(ll), lz = VZ(ll);
  t_real rx = VX(ur), ry = VY(ur), rz = VZ(ur);
  t_real cx = 0.5 * (lx + rx), cy = 0.5 * (ly + ry), cz = 0.5 * (lz + rz); 
  t_real px = lx, py = ly, pz = lz;
  t_real qx = cx, qy = cy, qz = cz;
  if ((idx >> 2) % 2 == 1) { px = cx; qx = rx; }
  if ((idx >> 1) % 2 == 1) { py = cy; qy = ry; }
  if ((idx >> 0) % 2 == 1) { pz = cz; qz = rz; }
#if USE_MALLOC
  rectangle * r = (rectangle *) malloc(sizeof(rectangle));
  r->ll = make_vect(px, py, pz);
  r->ur = make_vect(qx, qy, qz);
  return r;
#else
  return new rectangle(make_vect(px, py, pz), make_vect(qx, qy, qz));
#endif
}

space ** make_new_spaces(rectangle * area)
{
#if USE_MALLOC
  space ** s = (space **) malloc(N_CHILDREN * sizeof(space*));
#else
  space ** s = new space* [N_CHILDREN];
#endif
  //printf("make_new_spaces (%p)\n", s);
  for (int i = 0; i < N_CHILDREN; i++) {
    rectangle *r = make_sub_rectangle(area, i);
    //printf("!!make_sub_rectangle (%p)\n", r);
    s[i] = make_empty_space(r);
    //printf("make_empty_space: make_new_spaces (%p), s=(%p), area=(%p)\n", 
    // s[i], s, s[i]->area);
  }
  return s;
}

space ** make_new_spaces_morton(t_real diameter2, unsigned long low, 
  unsigned high)
{
#if USE_MALLOC
  space ** s = (space **) malloc(N_CHILDREN * sizeof(space*));
#else
  space ** s = new space* [N_CHILDREN];
#endif
  t_real d = diameter2 * 0.25;
  double stride = (high - low + 1) / 8 ;
  s[0] = make_empty_space_morton(d, low, low + stride);
//  printf("-s[0]: %f|%f, %lu, %f\n", d, stride, low, low + stride);
  for (int i = 1; i < N_CHILDREN; i++) {
    s[i] = make_empty_space_morton(d, low + stride * i + 1,
      low + stride * (i + 1));
//    printf("-s[%d]: %f|%f, %f, %f\n", i, d, stride,
//     low + stride * i + 1, low + stride * (i + 1));
  }
  
  /*
  for (int i = 0; i < N_CHILDREN; i++) {
    printf("[%lu, %lu] s[%d] = %p\n", low, high, i, s[i]);
  }
  */
  return s;
}

void space::add_particle(t_real m, vect_t p)
{
  switch (state) {
    case NO_PARTICLE: {
      state = ONE_PARTICLE;
      mass = m; cg = p;
      return;
    } /* NO_PARTICLE */
    case ONE_PARTICLE: {
      subspaces = make_new_spaces(area);
      int idx0 = select_covering_rectangle(cg, area);
      subspaces[idx0]->add_particle(mass, cg);
      int idx = select_covering_rectangle(p, area);
      subspaces[idx]->add_particle(m, p);
      state = MULTIPLE_PARTICLES;
      return;
    } /* ONE_PARTICLE */
    case MULTIPLE_PARTICLES: {
      int idx = select_covering_rectangle(p, area);
      subspaces[idx]->add_particle(m, p);
      return;
    } /* MULTIPLE_PARTICLES */
    default: {
      printf("ERROR: add_particle\n");
    } /* default */
  }
}

void space::add_particle_morton(unsigned long pmid, t_real m, vect_t p)
{
  switch (state) {
    case NO_PARTICLE: {
      state = ONE_PARTICLE;
      mass = m; cg = p;
      return;
    } /* NO_PARTICLE */
    case ONE_PARTICLE: {
      unsigned int stride = (midx.high - midx.low) / 8;
      subspaces = make_new_spaces_morton(diameter2, midx.low, midx.high);
      unsigned long mid = vect2morton(cg);
      int idx0 = (mid - midx.low) / stride + 1;
      subspaces[idx0]->add_particle_morton(mid, mass, cg);
      int idx = (pmid - midx.low) / stride + 1;
      subspaces[idx]->add_particle_morton(pmid, m, p);
      state = MULTIPLE_PARTICLES;
      return;
    } /* ONE_PARTICLE */
    case MULTIPLE_PARTICLES: {
      unsigned int stride = (midx.high - midx.low) / 8;
      int idx = (pmid - midx.low) / stride + 1;
      subspaces[idx]->add_particle_morton(pmid, m, p);
      return;
    } /* MULTIPLE_PARTICLES */
    default: {
      printf("ERROR: add_particle_morton\n");
    } /* default */
  }
}

void space::divide()
{
  state = MULTIPLE_PARTICLES;
  subspaces = make_new_spaces(area);
}

void space::divide_morton()
{
  state = MULTIPLE_PARTICLES;
  subspaces = make_new_spaces_morton(diameter2, midx.low, midx.high);
}

t_real particle::calc_limit()
{
  t_real x = VX(pos), y = VY(pos), z = VZ(pos);
  t_real xx = fabs(x), yy = fabs(y), zz = fabs(z);
  t_real r = xx;
  if (r < yy) r = yy;
  if (r < zz) r = zz;
  return r;
}

t_real calc_limit(particle ** particles, int n_particles)
{
  t_real r = 0.0;
  for (int i = 0; i < n_particles; i++) {
    t_real s = particles[i]->calc_limit();
    if (r < s) r = s;
  }
  return 1.01 * r;
}

/* Parallel version of tree build */
struct bt_thread_dat {
  space * tree;
  particle ** particles;
  int * subset_arr;
  int subset_size;
};

bt_thread_dat * particles_in_tree(bt_thread_dat * orig)
{
#if USE_MALLOC
  bt_thread_dat * p = (bt_thread_dat *) malloc(sizeof(bt_thread_dat));
#else
  bt_thread_dat * p = new bt_thread_dat;
#endif
  
  p->tree = orig->tree;
  p->particles = orig->particles;
#if USE_MALLOC
  int * arr = (int *) malloc(sizeof(int) * orig->subset_size);
#else
  int * arr = new int[orig->subset_size];
#endif
  p->subset_size = 0;
  for (int i = 0; i < orig->subset_size; i++) {
    if (cover_p(p->tree->area, p->particles[orig->subset_arr[i]]->pos)) {
      arr[i] = orig->subset_arr[i]; 
      p->subset_size++; 
    } else {
      arr[i] = -1;
    }
  }
  if (p->subset_size > 0) {
#if USE_MALLOC
    p->subset_arr = (int *) malloc(sizeof(int) * p->subset_size);
#else
    p->subset_arr = new int [p->subset_size];
#endif
    int j = 0;
    for (int i = 0; i < orig->subset_size; i++) {
      if (arr[i] != -1) {
        p->subset_arr[j] = arr[i];
        j++;
      }
    }
  } else {
    p->subset_arr = NULL;
    p->subset_size = 0;
  }
  return p;
}

void * build_tree_rec(void * args)
{
  int t0, t1;
#if USE_MALLOC
  pthread_t ths[N_CHILDREN];
  bt_thread_dat parr[N_CHILDREN];
#endif
  bt_thread_dat * p = (bt_thread_dat *) args;
  t0 = current_real_time_micro();
  bt_thread_dat * np = particles_in_tree(p);
  t1 = current_real_time_micro();
//  printf("%d %d %d\n", p->subset_size, np->subset_size, t1-t0);
  if (np->subset_size == 1) {
    particle * par = np->particles[np->subset_arr[0]];
    np->tree->add_particle(par->mass, par->pos);
  } else if (np->subset_size > 1) {
    void * ret;
    int n_threads = N_CHILDREN - 1;
    np->tree->divide();
#if !USE_MALLOC
    bt_thread_dat * parr = new bt_thread_dat[N_CHILDREN];
    pthread_t * ths = new pthread_t[n_threads];
#endif
    for (int i = 0; i < N_CHILDREN; i++) {
      parr[i].tree = np->tree->subspaces[i];
      parr[i].particles = np->particles;
      parr[i].subset_arr = np->subset_arr;
      parr[i].subset_size = np->subset_size;
      if (i < n_threads) {
        pthread_create(&ths[i], NULL, build_tree_rec, (void *) &parr[i]);
      } else {
        build_tree_rec(&parr[i]);
      }
    }
    for (int i = 0; i < n_threads; i++) 
      pthread_join(ths[i], (void **) ret);
  }
}

#define SUBSET_ARR_SZ 128
void particles_in_tree2(bt_thread_dat * orig,
  bt_thread_dat * parr)
{
  int t0, t1, t2, t3, t4, t5, t6;
  int *arr, _arr[SUBSET_ARR_SZ], if_free;
  
  t0 = current_real_time_micro();

  if (orig->subset_size <= SUBSET_ARR_SZ) {
    arr = _arr;
    if_free = 0;
  } else {
#if USE_MALLOC
    arr = (int *) malloc(sizeof(int) * orig->subset_size);
#else
    arr = new int[orig->subset_size];
    if_free = 1;
#endif
  }
  t1 = current_real_time_micro();
  
  orig->tree->divide();
  //printf("==return from divide (%p)\n", orig->tree->subspaces);
  t2 = current_real_time_micro();
  
  for (int i = 0; i < N_CHILDREN; i++) {
    parr[i].tree = orig->tree->subspaces[i];
    parr[i].particles = orig->particles;
    parr[i].subset_size = 0;
  }
  
  t3 = current_real_time_micro();
  for (int i = 0; i < orig->subset_size; i++) {
    int idx = select_covering_rectangle( 
      orig->particles[orig->subset_arr[i]]->pos, orig->tree->area);
    parr[idx].subset_size++;
    arr[i] = idx;
  }
  
  t4 = current_real_time_micro();
  int indices[N_CHILDREN];
  for (int i = 0; i < N_CHILDREN; i++) {
    if (parr[i].subset_size > 0) {
#if USE_MALLOC
      parr[i].subset_arr = (int *) malloc(sizeof(int) * parr[i].subset_size);
#else
      parr[i].subset_arr = new int [parr[i].subset_size];
#endif
      //printf("parr[%d] (%p)\n", i, parr[i].subset_arr);
    } else {
      parr[i].subset_arr = NULL;
    }
    indices[i] = 0;
  }
  
  t5 = current_real_time_micro();
  for (int i = 0; i < orig->subset_size; i++) {
    int idx = arr[i];
    parr[idx].subset_arr[indices[idx]] = orig->subset_arr[i];
    indices[idx]++;
  }
  t6 = current_real_time_micro();

#if USE_MALLOC
  if (if_free)
    free(arr);
#endif
  
  /*
  if (n_buildtree >= 2)
      fprintf(stderr, "%d %d %d %d %d %d %d %d\n", orig->subset_size, t6-t0, 
       t1-t0, t2-t1, t3-t2, t4-t3, t5-t4, t6-t5);
  */
}

void * build_tree_rec2(void * args)
{
  pthread_t ths[N_CHILDREN];
  bt_thread_dat parr[N_CHILDREN];
  bt_thread_dat * p = (bt_thread_dat *) args;
 
  if (p->subset_size == 1) {
    //printf("\"%p\" -> \"%d\"\n", p->tree, p->subset_arr[0]);
    particle * par = p->particles[p->subset_arr[0]];
    p->tree->add_particle(par->mass, par->pos);
  } else if (p->subset_size > 1) {
    particles_in_tree2(p, parr);
    /*
    printf("%p add ", p->tree);
    for (int i = 0; i < p->subset_size; i++)
      printf("%d ", p->subset_arr[i]);
    printf("\n");
    */
    void * ret;
    int n_threads = N_CHILDREN - 1;
    for (int i = 0; i < N_CHILDREN; i++) {
      //printf("\"%p\" -> \"%p\"\n", p->tree, p->tree->subspaces[i]);
      if (i < n_threads) {
        pthread_create(&ths[i], NULL, build_tree_rec2, (void *) &parr[i]);
      } else {
        build_tree_rec2(&parr[i]);
      }
    }
    for (int i = 0; i < n_threads; i++) 
      pthread_join(ths[i], (void **) ret);
  }
#if USE_MALLOC
  free(p->subset_arr);
#endif
}

space * build_tree(particle ** particles, int n_particles)
{
  bt_thread_dat dat;
  n_buildtree++;

  t_real limit = calc_limit(particles, n_particles);
  rectangle * rec = make_entire_rectangle(limit);
  space * tree = make_empty_space(rec);
  //g_limit_area = make_entire_rectangle(limit);
  //morton_sort(particles, n_particles); 
  //printf("make_empty_space: build_tree (%p)\n", tree);
  dat.tree = tree;
  dat.particles = particles;
  dat.subset_size = n_particles;
#if USE_MALLOC
  dat.subset_arr = (int *) malloc(sizeof(int) * dat.subset_size);
#else
  dat.subset_arr = new int[dat.subset_size];
#endif
  for (int i = 0; i < dat.subset_size; i++) dat.subset_arr[i] = i;
//  build_tree_rec(&dat);
  
  //if (n_buildtree == 5) mtrace();
  build_tree_rec2(&dat);
  //if (n_buildtree == 5) muntrace();
  return tree;
}

struct btmorton_thread_dat {
  space * tree;
  particle ** particles;
  int low, high;
};

int bin_search_low(particle ** p, int key, int imin, int imax)
{
  int imid;
  
  while (imax >= imin) {
    imid = (imin + imax) / 2;
    if (p[imid]->mid < key)
      imin = imid + 1;
    else if (p[imid]->mid > key)
      imax = imid - 1;
    else
      return imid;
  }
  // Key does not necessarily exist, so return a lower bound
  return imin;
}

int bin_search_high(particle ** p, int key, int imin, int imax)
{
  int imid;
  
  while (imax >= imin) {
    imid = (imin + imax) / 2;
    if (p[imid]->mid < key)
      imin = imid + 1;
    else if (p[imid]->mid > key)
      imax = imid - 1;
    else
      return imid;
  }
  // Key does not necessarily exist, so return a lower bound
  return imax;
}

void select_covering_range_morton(particle **p, 
  space * s, int *low, int *high)
{
  int _low, _high;
  unsigned int mx, my, mz, len;

  if (p[*high]->mid < s->midx.low || p[*low]->mid > s->midx.high) {
    *low = *high = -1;
    return;
  }

  _low = bin_search_low(p, s->midx.low, *low, *high);
  _high = bin_search_high(p, s->midx.high, *low, *high);
  if (_low == -1 || _high == -1 || _low > _high) {
    _low = -1;
    _high= -1;
  }
  //printf("[%d,%d] U [%d,%d] = [%d,%d]\n", *low, *high, s->midx.low,
  //  s->midx.high, _low, _high);
  *low = _low;
  *high = _high;
}

void particles_in_tree_morton2(btmorton_thread_dat * orig,
  btmorton_thread_dat * parr)
{
  int low, high;
  orig->tree->divide_morton();
  for (int i = 0; i < N_CHILDREN; i++) {
    parr[i].tree = orig->tree->subspaces[i];
    parr[i].particles = orig->particles;
    parr[i].low = orig->low;
    parr[i].high = orig->high;
    //select_covering_range_morton(orig->particles, parr[i].tree,
    //  &low, &high);
    //parr[i].low = low;
    //parr[i].high = high;
  }
}

void particles_in_tree_morton(btmorton_thread_dat * orig,
  btmorton_thread_dat * parr)
{
  int low, high;
  low = orig->low; high = orig->high;
  orig->tree->divide_morton();
  for (int i = 0; i < N_CHILDREN; i++) {
    parr[i].tree = orig->tree->subspaces[i];
    parr[i].particles = orig->particles;
    select_covering_range_morton(orig->particles, parr[i].tree,
      &low, &high);
    parr[i].low = low;
    parr[i].high = high;
    if (low == -1 || high == -1) 
      low = orig->low;
    else
      low = high;
    high = orig->high;
  }
}

void * build_tree_morton_rec(void * args)
{
  int t0, t1, t2, t3, t4, t5, t6;
  pthread_t ths[N_CHILDREN];
  btmorton_thread_dat parr[N_CHILDREN];
  btmorton_thread_dat * p = (btmorton_thread_dat *) args;
  int subset_size = 0;
 
  //select_covering_range_morton(p->particles, p->tree,
  //    &p->low, &p->high);
  
  if (p->high != -1 && p->low != -1)
    subset_size = p->high - p->low + 1;
  
  //t1 = t2 = t3 = t4 = 0;
  //t0 = current_real_time_micro();
  if (subset_size == 1) {
    particle * par = p->particles[p->low];
    p->tree->add_particle_morton(par->mid, par->mass, par->pos);
    //fprintf(stderr, "\"%p\" -> \"%d\"\n", p->tree, p->low);
    //t1 = current_real_time_micro();
    //t2 = t0;
  } else if (subset_size > 1) {
    particles_in_tree_morton(p, parr);
    //particles_in_tree_morton2(p, parr);
    //t2 = current_real_time_micro();
    //t1 = t0;
    void * ret;
    int n_threads = N_CHILDREN - 1;
    for (int i = 0; i < N_CHILDREN; i++) {
      //fprintf(stderr, "\"%p\" -> \"%p\"\n", p->tree, p->tree->subspaces[i]);
      if (i < n_threads) {
        pthread_create(&ths[i], NULL, build_tree_morton_rec, 
          (void *) &parr[i]);
      } else {
        build_tree_morton_rec(&parr[i]);
      }
    }
    //t3 = current_real_time_micro();
    for (int i = 0; i < n_threads; i++) 
      pthread_join(ths[i], (void **) ret);
    //t4 = current_real_time_micro();
  } else {
    t1 = t2 = t0;
  }

//  fprintf(stderr, "%d, %d, %d\n", t1-t0, t2-t0, t4-t3);
}

space * build_tree_morton(particle ** particles, int n_particles)
{
  int t0, t1, t2, t3, t4, t5, t6;
  btmorton_thread_dat dat;
  
  n_buildtree++;
  t_real limit = calc_limit(particles, n_particles);
  g_limit_area = make_entire_rectangle(limit);
  t0 = current_real_time_milli();
  morton_sort(particles, n_particles); 
  t1 = current_real_time_milli();
  space * tree = make_empty_space_morton(diameter2(g_limit_area),
    0, g_morton_len);
  dat.tree = tree;
  dat.particles = particles;
  dat.low = 0;
  dat.high = n_particles - 1;
  
  if (n_buildtree == 5) mtrace();
  t2 = current_real_time_milli();
  build_tree_morton_rec(&dat);
  t3 = current_real_time_milli();
  if (n_buildtree == 5) muntrace();

  printf("build_tree_morton breakdown:\n"
     "  sort: %d\n" 
     "  make_space: %d\n"
     "  build_tree_rec:%d\n", t1-t0, t2-t1, t3-t2);
  return tree;
}

void* free_tree_rec(void * args)
{
  space * tree = (space *) args;
  pthread_t ths[N_CHILDREN];

  if (tree == NULL) return (void *) 0;

  if (tree->state == MULTIPLE_PARTICLES) {
    void * ret;
    int n_threads = N_CHILDREN - 1;
    for (int i = 0; i < N_CHILDREN; i++) {
      //printf("---free_tree: subspace[%d] (%p), tree=(%p), area=(%p)\n", i, 
      // tree->subspaces[i], tree->subspaces);
      if (i < n_threads) {
        pthread_create(&ths[i], NULL, free_tree_rec, 
         (void *) tree->subspaces[i]);
      } else {
        free_tree_rec(tree->subspaces[i]);
      }
    }
    for (int i = 0; i < n_threads; i++) 
     pthread_join(ths[i], (void **) ret);
#if USE_MALLOC
    //printf("---free_tree: subspaces (%p)\n", tree->subspaces);
    free(tree->subspaces);
    tree->state = NO_PARTICLE;
#endif
 }
#if !UNBOX_VECT
  free(tree->cg);
#endif

#if !USE_MORTON
  free(tree->area);
#endif
  //printf("free_tree: area (%p)\n", tree->area);
  free(tree);
}

void free_tree(space * tree)
{
  //printf("root (%p)\n", tree);
  free_tree_rec((void *) tree);
}


struct btup_thd_dat {
  space * tree;
  rectangle * area;
  particle ** particles;
  int * subset_arr;
  int subset_size;
};

btup_thd_dat * particles_in_tree(btup_thd_dat * orig)
{
#if USE_MALLOC
  btup_thd_dat * p = (btup_thd_dat *) malloc(sizeof(btup_thd_dat));
#else
  btup_thd_dat * p = new btup_thd_dat;
#endif
  p->tree = orig->tree; 
  p->area = orig->area;
  p->particles = orig->particles;
#if USE_MALLOC
  int * arr = (int *) malloc(sizeof(int) * orig->subset_size);
#else
  int * arr = new int[orig->subset_size];
#endif
  p->subset_size = 0;
  for (int i = 0; i < orig->subset_size; i++) {
    if (cover_p(p->area, p->particles[orig->subset_arr[i]]->pos)) {
      arr[i] = orig->subset_arr[i]; 
      p->subset_size++; 
    } else {
      arr[i] = -1;
    }
  }
  if (p->subset_size > 0) {
#if USE_MALLOC
    p->subset_arr = (int *) malloc(sizeof(int) * p->subset_size);
#else
    p->subset_arr = new int [p->subset_size];
#endif
    int j = 0;
    for (int i = 0; i < orig->subset_size; i++) {
      if (arr[i] != -1) {
        p->subset_arr[j] = arr[i];
        j++;
      }
    }
  } else {
    p->subset_arr = NULL;
    p->subset_size = 0;
  }
  return p;
}

void * build_tree_bottomup_rec(void *args)
{
#if USE_MALLOC
  btup_thd_dat parr[N_CHILDREN];
  pthread_t ths[N_CHILDREN];
#endif
  btup_thd_dat * p = (btup_thd_dat *) args;
  btup_thd_dat * np = particles_in_tree(p);
  p->tree = make_empty_space(np->area);
  if (np->subset_size == 1) {
    particle * par = p->particles[np->subset_arr[0]];
    p->tree->add_particle(par->mass, par->pos);
  } else if (np->subset_size > 1) {
    int n_threads = N_CHILDREN - 1;
    void * ret;
#if !USE_MALLOC
    btup_thd_dat * parr = new btup_thd_dat[N_CHILDREN];
    pthread_t * ths = new pthread_t[n_threads];
#endif
    for (int i = 0; i < N_CHILDREN; i++) {
      parr[i].tree = NULL;
      parr[i].area = make_sub_rectangle(np->area, i);
      parr[i].particles = np->particles;
      parr[i].subset_arr = np->subset_arr;
      parr[i].subset_size = np->subset_size;
      if (i < n_threads) {
        pthread_create(&ths[i], NULL, build_tree_bottomup_rec, 
          (void *) &parr[i]);
      } else {
        build_tree_bottomup_rec(&parr[i]);
     }
    }
    p->tree->state = MULTIPLE_PARTICLES;
#if USE_MALLOC
    p->tree->subspaces = (space **) malloc(sizeof(space *) * N_CHILDREN);
#else
    p->tree->subspaces = new space * [N_CHILDREN];
#endif
    for (int i = 0; i < N_CHILDREN; i++) {
      if (i < n_threads) pthread_join(ths[i], (void **) ret);
      p->tree->subspaces[i] = parr[i].tree;
    }
  }
}

space * build_tree_bottomup(particle ** particles, int n_particles)
{
  btup_thd_dat dat;
	t_real limit = calc_limit(particles, n_particles);
  dat.tree = NULL;
  dat.area = make_entire_rectangle(limit);
  dat.particles = particles;
  dat.subset_size = n_particles;
#if USE_MALLOC
  dat.subset_arr = (int *) malloc(sizeof(int) * dat.subset_size);
#else
  dat.subset_arr = new int[dat.subset_size];
#endif
  for (int i = 0; i < dat.subset_size; i++) dat.subset_arr[i] = i;
  build_tree_bottomup_rec(&dat);
  return dat.tree;
}

/* Serial version of tree generation */
space * generate_tree(particle ** particles, int n_particles)
{
	t_real limit = calc_limit(particles, n_particles);
	rectangle * rec = make_entire_rectangle(limit);
	space * tree = make_empty_space(rec);
	for (int i = 0; i < n_particles; i++)
		tree->add_particle(particles[i]->mass, particles[i]->pos);
	return tree;
}

mass_momentum space::set_mass_and_cg()
{
  switch (state) {
    case NO_PARTICLE: {
      mass = 0.0;
      return mass_momentum(0.0, make_vect(0.0, 0.0, 0.0), 0);
    } /* NO_PARTICL */
    case ONE_PARTICLE: {
      return mass_momentum(mass, k_times_v(mass, cg), 1);
    } /* ONE_PARTICLE */
    case MULTIPLE_PARTICLES: {
      vect_t total_momentum = make_vect(0.0, 0.0, 0.0);
      t_real total_mass = 0.0;
      int total_n_nodes = 1;
      for (int i = 0; i < N_CHILDREN; i++) {
	      space * subspace = subspaces[i];
	      mass_momentum mm = subspace->set_mass_and_cg();
	      if (mm.n_nodes == 0) {
          free(subspaces[i]->area);
          free(subspaces[i]);
	        subspaces[i] = NULL; // !! Forget to free !!
        } else {
          total_mass += mm.mass;
          total_momentum = v_plus_v(total_momentum, mm.momentum);
          total_n_nodes += mm.n_nodes;
        }
      }
      mass = total_mass; 
      cg = v_div_k(total_mass, total_momentum);
      return mass_momentum(total_mass, total_momentum, total_n_nodes);
    } /* MULTIPLE_PARTICLES */
    default: {
      printf("invalid state tag = %d\n", state);
      return mass_momentum(0.0, make_vect(0.0, 0.0, 0.0), 0);
    } /* default */
  } 
}
