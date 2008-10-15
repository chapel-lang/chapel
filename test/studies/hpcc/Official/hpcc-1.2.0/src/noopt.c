/* -*- mode: C; tab-width: 2; indent-tabs-mode: nil; fill-column: 79; coding: iso-latin-1-unix -*- */

double
HPCC_dweps() {
  double eps, one, half;

  one = 1.0;
  half = one / 2.0;
  eps = one;

  while (one + eps != one)
    eps *= half;

  return eps;
}

float
HPCC_sweps() {
  float eps, one, half;

  one = 1.0f;
  half = one / 2.0f;
  eps = one;

  while (one + eps != one)
    eps *= half;

  return eps;
}

