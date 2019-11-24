#ifndef CGPPOINT_H
#define CGPPOINT_H

typedef struct cgp_point_t  cgppoint;

struct cgp_point_t {
  double x;
  double y;
};

void cgppoint_linspace(
        cgppoint   *point,
	double      start,
	double      stop,
	size_t      length);
void cgppoint_ffill(
        cgppoint   *point,
	size_t      length,
	double    (*f)(double));

#endif // CGPPOINT_H
