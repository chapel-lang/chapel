/*
  Copyright 2002-2003 John Plevyak, All Rights Reserved
*/

#include "d.h"

uint d_prime2[] = {
  1, 3, 7, 13, 31, 61, 127, 251, 509, 1021, 2039, 4093, 8191,
  16381, 32749, 65521, 131071, 262139, 524287, 1048573, 2097143,
  4194301, 8388593, 16777213, 33554393, 67108859, 134217689,
  268435399, 536870909
};

int d_verbose_level = 0;
int d_debug_level = 0;
int test_level = 0;

char *
d_dup_pathname_str(char *s) {
  char *e = s;
  if (!s)
    return strdup("");
  if (*e == '"') {
    e++; while (*e && *e != '"') e++;
    return dup_str(s + 1, e);
  } else
    return dup_str(s, s+strlen(s));
}

char *
dup_str(char *s, char *e) {
  int l = e-s;
  char *ss = (char*)MALLOC(l+1);
  memcpy(ss, s, l);
  ss[l] = 0;
  return ss;
}

uint 
strhashl(char *s, int l) {
  uint h = 0, g;
  int i = 0;
                
  for (;i < l;i++,s++) {
    h = (h << 4) + *s;
    if ((g = h & 0xf0000000))
      h = (h ^ (g >> 24)) ^ g;
  }
  return h;
}

int
buf_read(char *pathname, char **buf, int *len) {
  struct stat sb;
  int fd;

  *buf = 0;
  *len = 0;
  fd = open(pathname, O_RDONLY);
  if (fd <= 0) 
    return -1;
  memset(&sb, 0, sizeof(sb));
  fstat(fd, &sb);
  *len = sb.st_size;
  *buf = (char*)MALLOC(*len + 2);
  (*buf)[*len] = 0;		/* terminator */
  (*buf)[*len + 1] = 0;		/* sentinal */
  read(fd, *buf, *len);
  close(fd);
  return *len;
}

char *
sbuf_read(char *pathname) {
  char *buf;
  int len;

  if (buf_read(pathname, &buf, &len) < 0)
    return NULL;
  return buf;
}

void
d_fail(char *str, ...) {
  char nstr[256];
  va_list ap;
  va_start(ap, str);
  snprintf(nstr, 255, "fail: %s\n", str);
  vfprintf(stderr, nstr, ap);
  va_end(ap);
  exit(1);
}

void
vec_add_internal(void *v, void *elem) {
  AbstractVec *av = (AbstractVec*)v;
  if (!av->n) {
    av->v = av->e;
  } else if (av->v == av->e) {
    av->v = (void**)MALLOC(INITIAL_VEC_SIZE * sizeof(void *));
    memcpy(av->v, av->e, av->n * sizeof(void *));
  } else
    if ((av->n & (INITIAL_VEC_SIZE - 1)) == 0) {
      int l = av->n, nl = (1 + INITIAL_VEC_SHIFT);
      l = l >> INITIAL_VEC_SHIFT;
      while (!(l&1)) { l = l >> 1; nl++; }
      l = l >> 1;
      if (!av->n || !l) {
	nl = 1 << nl;
	av->v = (void**)REALLOC(av->v, nl * sizeof(void *));
      }
    }
  av->v[av->n++] = elem;
}

int
vec_eq(void *v, void *vv) {
  AbstractVec *av = (AbstractVec*)v;
  AbstractVec *avv = (AbstractVec*)vv;
  uint i;

  if (av->n != avv->n)
    return 0;
  for (i = 0; i < av->n; i++)
    if (av->v[i] != avv->v[i])
      return 0;
  return 1;
}

void *
stack_push_internal(AbstractStack *s, void *elem) {
  int n = s->cur - s->start;
  if (s->start == s->initial) {
    s->cur = (void**)MALLOC(n * 2 * sizeof(void*));
    memcpy(s->cur, s->start, n * sizeof(void*));
  } else
    s->cur = (void**)REALLOC(s->start, n * 2 * sizeof(void*));
  s->end = s->start = s->cur;
  s->cur += n;
  s->end += n * 2;
  *s->cur++ = elem;
  return elem;
}

int
set_add(void *av, void *t) {
  AbstractVec *v = (AbstractVec*)av, vv;
  int j, n = v->n;
  uint i;
  if (n) {
    uint h = ((uint)t);
    h = h % n;
    for (i = h, j = 0; 
	 i < v->n && j < SET_MAX_SEQUENTIAL; 
	 i = ((i + 1) % n), j++) 
    {
      if (!v->v[i]) {
	v->v[i] = t;
	return 1;
      } else if (v->v[i] == t)
	return 0;
    }
  }
  if (!n) {
    vv.v = NULL;
    v->i = INITIAL_SET_SIZE_INDEX;
  } else {
    vv.v = v->v;
    vv.n = v->n;
    v->i = v->i + 1;
  }
  v->n = d_prime2[v->i];
  v->v = (void**)MALLOC(v->n * sizeof(void *));
  memset(v->v, 0, v->n * sizeof(void *));
  if (vv.v) {
    set_union(av, &vv);
    FREE(vv.v);
  }
  return set_add(v, t);
}

void *
set_add_fn(void *av, void *t, hash_fns_t *fns) {
  AbstractVec *v = (AbstractVec*)av, vv;
  uint32 tt = fns->hash_fn(t, fns);
  int j, n = v->n;
  uint i;
  if (n) {
    uint h = tt % n;
    for (i = h, j = 0; 
	 i < v->n && j < SET_MAX_SEQUENTIAL; 
	 i = ((i + 1) % n), j++) 
    {
      if (!v->v[i]) {
	v->v[i] = t;
	return t;
      } else {
	if (!fns->cmp_fn(v->v[i], t, fns))
	  return v->v[i];
      }
    }
  }
  if (!n) {
    vv.v = NULL;
    v->i = INITIAL_SET_SIZE_INDEX;
  } else {
    vv.v = v->v;
    vv.n = v->n;
    v->i = v->i + 1;
  }
  v->n = d_prime2[v->i];
  v->v = (void**)MALLOC(v->n * sizeof(void *));
  memset(v->v, 0, v->n * sizeof(void *));
  if (vv.v) {
    set_union_fn(av, &vv, fns);
    FREE(vv.v);
  }
  return set_add_fn(v, t, fns);
}

int
set_union(void *av, void *avv) {
  AbstractVec *vv = (AbstractVec*)avv;
  uint i, changed = 0;
  
  for (i = 0; i < vv->n; i++)
    if (vv->v[i])
      changed = set_add(av, vv->v[i]) || changed;
  return changed;
}

void
set_union_fn(void *av, void *avv, hash_fns_t *fns) {
  AbstractVec *vv = (AbstractVec*)avv;
  uint i;
  
  for (i = 0; i < vv->n; i++)
    if (vv->v[i])
      set_add_fn(av, vv->v[i], fns);
}

void
set_to_vec(void *av) {
  AbstractVec *v = (AbstractVec*)av, vv;
  uint i;

  vv.n = v->n;
  vv.v = v->v;
  if (v->v == v->e) {
    memcpy(vv.e, v->e, sizeof(v->e));
    vv.v = vv.e;   
  }
  v->n = 0;
  v->v = 0;
  for (i = 0; i < vv.n; i++)
    if (vv.v[i])
      vec_add_internal(v, vv.v[i]);
  FREE(vv.v);
}

void
int_list_diff(int *a, int *b, int *c) {
  while (1) {
    if (*b < 0)
      break;
  Lagainc:
    if (*c < 0) {
      while (*b >= 0)
	*a++ = *b++;
      break;
    }
  Lagainb:
    if (*b == *c) {
      b++;
      c++;
      continue;
    }
    if (*b < *c) {
      *a++ = *b++;
      if (*b < 0)
	break;
      goto Lagainb;
    }
    if (*c < *b) {
      c++;
      goto Lagainc;
    }
  }
  *a++ = -1;
}

void
int_list_intersect(int *a, int *b, int *c) {
  while (1) {
    if (*b < 0)
      break;
  Lagainc:
    if (*c < 0)
      break;
  Lagainb:
    if (*b == *c) {
      *a++ = *b++;
      c++;
      continue;
    }
    if (*b < *c) {
      b++;
      if (*b < 0)
	break;
      goto Lagainb;
    }
    if (*c < *b) {
      c++;
      goto Lagainc;
    }
  }
  *a++ = -1;
}

int *
int_list_dup(int *aa) {
  int *a = aa, *b, *bb;
  while (*a != -1) { a++; }
  bb = b = (int*)MALLOC((a - aa + 1) * sizeof(int));
  a = aa;
  while (*a != -1) { *b++ = *a++; }
  *b++ = -1;
  return bb;
}

#define ESC(_c) *ss++ = '\\'; *ss++ = _c; break;
char *
escape_string(char *s) {
  char *ss = (char*)MALLOC((strlen(s) + 1) * 4), *sss = ss;
  for (; *s; s++) {
    switch (*s) {
      case '\b': ESC('b');
      case '\f': ESC('f');
      case '\n': ESC('n');
      case '\r': ESC('r');
      case '\t': ESC('t');
      case '\v': ESC('v');
      case '\a': ESC('a');
      case '\\': 
      case '\"':
	*ss++ = '\\';
	*ss++ = *s; break;
		
      default:
	if (isprint(*s)) {
	  *ss++ = *s;
	} else {
	  *ss++ = '\\';
	  *ss++ = 'x';
	  *ss++ = tohex2(*s);
	  *ss++ = tohex1(*s);
	}
	break;
    }
  }
  *ss = 0;
  return sss;
}

void d_free(void *x) { FREE(x); }
