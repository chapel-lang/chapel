#ifndef TEST_TYPE_ONLY_HEADER_H
#define TEST_TYPE_ONLY_HEADER_H

typedef struct baz {
  int a;
  int b;
  int c;
  int d;
} baz;

#define FOOBAR_PADDING 32
typedef struct foobar {
  double a;
  double b;
  double c;
  double d;
  double e;
  double f;
  double g;
  double h;
  baz x;
  int padding[FOOBAR_PADDING];
} foobar;

#endif
