#include <stdlib.h>

#include "cgppoint.h"

void cgppoint_linspace(
        cgppoint   *point,
	double      start,
	double      stop,
	size_t      length)
{
  double da = (stop - start) / (length - 1);
  double val = start;
  for (size_t i = 0; i < length; ++i) {
    (point++)->x = val;
    val += da;
  }
}

void cgppoint_ffill(
        cgppoint   *point,
	size_t      length,
	double    (*f)(double))
{
  for (size_t i = 0; i < length; ++i) {
    point->y = (*f)(point->x);
    point++;
  }
}
