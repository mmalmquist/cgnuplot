#ifndef CGPFIGURE_H
#define CGPFIGURE_H

#include <stdio.h>
#include "cgpentry.h"

typedef struct cgp_figure_t cgpfigure;
typedef struct cgp_figvar_t cgpfigvar;

struct cgp_figure_t {
  cgpfigvar        *fvar;
  void (*add_entry)(
        cgpfigure  *fig,
	cgpentry   *entry);
  void (*write_config)(
        cgpfigure  *fig,
	FILE       *gnuplot);
  void (*plot)(
        cgpfigure  *fig,
	FILE       *gnuplot);
  
  void (*set_xaxis)(
        cgpfigure  *fig,
        double      scale,
        double      xmin,
        double      xmax);
  void (*set_xscale)(
        cgpfigure   *fig,
	double      scale);
  void (*set_xrange)(
        cgpfigure  *fig,
	double      xmin,
	double      xmax);
  void (*set_xmax)(
        cgpfigure  *fig,
	double      xmax);
  void (*set_xmin)(
        cgpfigure  *fig,
	double      xmin);
  
  void (*set_yaxis)(
        cgpfigure  *fig,
        double      scale,
        double      ymin,
        double      ymax);
  void (*set_yscale)(
        cgpfigure   *fig,
	double      scale);
  void (*set_yrange)(
        cgpfigure  *fig,
	double      ymin,
	double      ymax);
  void (*set_ymax)(
        cgpfigure  *fig,
	double      ymax);
  void (*set_ymin)(
        cgpfigure  *fig,
	double      ymin);
  
  void (*set_legend)(
        cgpfigure  *fig,
	int         enabled);
};

void cgpfigure_ctor(
        cgpfigure **fig);
void cgpfigure_dtor(
        cgpfigure **fig);

#endif //CGPFIGURE_H
