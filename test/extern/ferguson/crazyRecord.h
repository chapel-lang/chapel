#include <stdio.h>

/* Here we include a variety of structure types
   that would be promoted into registers in different
   ways, including registers containing part of
   a structure, or registers containing padding.

   The naming convention is that uppercase fields
   are structures (eg. iAbc means it contains 3
   ints, but the first is in a nested structure).
  */
struct s_a {
  char a;
};
struct s_Abc {
  struct s_a sa;
  char b;
  char c;
};
struct s_A_BCde {
  struct s_Abc sabc;
  short d;
  char e;
};

typedef struct s_A_BCde A_BCde;

struct s_ia {
  int a;
};
struct s_iAbc {
  struct s_ia sa;
  int b;
  int c;
};

typedef struct s_iAbc iAbc;

struct s_ib {
  int b;
};
struct s_iaBc {
  int a;
  struct s_ib sb;
  int c;
};

typedef struct s_iaBc iaBc;

struct s_ic {
  int c;
};
struct s_iabC {
  int a;
  int b;
  struct s_ic sc;
};

typedef struct s_iabC iabC;

struct s_iab {
  int a;
  int b;
};
struct s_iABc {
  struct s_iab sab;
  int c;
};

typedef struct s_iABc iABc;

struct s_ibc {
  int b;
  int c;
};
struct s_iaBC {
  int a;
  struct s_ibc sbc;
};

typedef struct s_iaBC iaBC;

struct s_iaBCd {
  int a;
  struct s_ibc sbc;
  int d;
};

typedef struct s_iaBCd iaBCd;

static inline
A_BCde return_A_BCde(void) {
  A_BCde r;
  r.sabc.sa.a = 1;
  r.sabc.b = 2;
  r.sabc.c = 3;
  r.d = 4;
  r.e = 5;
  return r;
}

static inline
void print_A_BCde(A_BCde r) {
  printf("%i\n", r.sabc.sa.a);
  printf("%i\n", r.sabc.b);
  printf("%i\n", r.sabc.c);
  printf("%i\n", r.d);
  printf("%i\n", r.e);
}

static inline
iAbc return_iAbc(void)
{
  iAbc r;
  r.sa.a = 1;
  r.b = 2;
  r.c = 3;
  return r;
}

static inline
void print_iAbc(iAbc r)
{
  printf("%i\n", r.sa.a);
  printf("%i\n", r.b);
  printf("%i\n", r.c);
}

static inline
iaBc return_iaBc(void)
{
  iaBc r;
  r.a = 1;
  r.sb.b = 2;
  r.c = 3;
  return r;
}
  
static inline
void print_iaBc(iaBc r)
{
  printf("%i\n", r.a);
  printf("%i\n", r.sb.b);
  printf("%i\n", r.c);
}

static inline
iabC return_iabC(void)
{
  iabC r;
  r.a = 1;
  r.b = 2;
  r.sc.c = 3;
  return r;
}
  
static inline
void print_iabC(iabC r)
{
  printf("%i\n", r.a);
  printf("%i\n", r.b);
  printf("%i\n", r.sc.c);
}

static inline
iABc return_iABc(void)
{
  iABc r;
  r.sab.a = 1;
  r.sab.b = 2;
  r.c = 3;
  return r;
}
  
static inline
void print_iABc(iABc r)
{
  printf("%i\n", r.sab.a);
  printf("%i\n", r.sab.b);
  printf("%i\n", r.c);
}

static inline
iaBC return_iaBC(void)
{
  iaBC r;
  r.a = 1;
  r.sbc.b = 2;
  r.sbc.c = 3;
  return r;
}
  
static inline
void print_iaBC(iaBC r)
{
  printf("%i\n", r.a);
  printf("%i\n", r.sbc.b);
  printf("%i\n", r.sbc.c);
}

static inline
iaBCd return_iaBCd(void)
{
  iaBCd r;
  r.a = 1;
  r.sbc.b = 2;
  r.sbc.c = 3;
  r.d = 4;
  return r;
}
  
static inline
void print_iaBCd(iaBCd r)
{
  printf("%i\n", r.a);
  printf("%i\n", r.sbc.b);
  printf("%i\n", r.sbc.c);
  printf("%i\n", r.d);
}

