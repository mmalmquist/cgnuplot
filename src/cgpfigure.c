#include <stdlib.h>
#include <stdio.h>

#include "cgpfigure.h"
#include "cgpstack.h"
#include "cgpentry.h"

#define CGPFIGURE_ASSERT(ptr, type) \
  if ((ptr)==NULL){(ptr)=(type *)calloc(1, sizeof(type));}

typedef struct cgp_axis_t   cgpaxis;
typedef struct cgp_legend_t cgplegend;

struct cgp_figvar_t {
  cgpaxis *yaxis, *xaxis; /* axis settings */
  cgplegend *legend;      /* figure legend */
  cgpstack *entry;        /* list of figure plots */
};

struct cgp_axis_t {
  double *scale;
  double *max;
  double *min;
};

struct cgp_legend_t {
  int enabled;
};

static void cgpfigure_init_vtbl(
        cgpfigure  *fig);
static void cgpfigure_axis_dtor(
        cgpaxis   **ax);
static void cgpfigure_legend_dtor(
        cgplegend **legend);

static void cgpfigure_set_scale(
        cgpaxis    *ax,
	double      scale);
static void cgpfigure_set_range(
        cgpaxis    *ax,
	double      min,
	double      max);
static void cgpfigure_set_max(
        cgpaxis    *ax,
	double      max);
static void cgpfigure_set_min(
        cgpaxis    *ax,
	double      min);
static void cgpfigure_add_entry(
        cgpfigure  *fig,
	cgpentry   *entry);
static void cgpfigure_set_xaxis(
        cgpfigure  *fig,
        double      scale,
        double      xmin,
        double      xmax);
static void cgpfigure_set_xscale(
        cgpfigure  *fig,
	double      scale);
static void cgpfigure_set_xrange(
        cgpfigure  *fig,
	double      xmin,
	double      xmax);
static void cgpfigure_set_xmax(
        cgpfigure  *fig,
	double      xmax);
static void cgpfigure_set_xmin(
        cgpfigure  *fig,
	double      xmin);
static void cgpfigure_set_yaxis(
        cgpfigure  *fig,
        double      scale,
        double      ymin,
        double      ymax);
static void cgpfigure_set_yscale(
        cgpfigure  *fig,
	double      scale);
static void cgpfigure_set_yrange(
        cgpfigure  *fig,
	double      ymin,
	double      ymax);
static void cgpfigure_set_ymax(
        cgpfigure  *fig,
	double      ymax);
static void cgpfigure_set_ymin(
        cgpfigure  *fig,
	double      ymin);
static void cgpfigure_set_legend(
        cgpfigure  *fig,
	int         enabled);
static void cgpfiure_write_axis_config(
        cgpaxis    *ax,
	FILE       *gnuplot,
	char        axis);
static void cgpfigure_write_config(
        cgpfigure  *fig,
	FILE       *gnuplot);
static void cgpfigure_plot(
        cgpfigure  *fig,
	FILE       *gnuplot);

void cgpfigure_ctor(
        cgpfigure **fig)
{
  *fig = (cgpfigure *) malloc(sizeof(cgpfigure));

  (*fig)->fvar = (cgpfigvar *) malloc(sizeof(cgpfigvar));
  cgpfigvar *fvar = (*fig)->fvar;
  fvar->yaxis = NULL;
  fvar->xaxis = NULL;
  fvar->entry = NULL;
  fvar->legend = (cgplegend *) malloc(sizeof(cgplegend));
  fvar->legend->enabled = 0;

  cgpfigure_init_vtbl(*fig);
}

void cgpfigure_dtor(
        cgpfigure **fig)
{
  cgpfigvar *fvar = (*fig)->fvar;

  if (fvar->xaxis != NULL) { cgpfigure_axis_dtor(&fvar->xaxis); }
  if (fvar->yaxis != NULL) { cgpfigure_axis_dtor(&fvar->yaxis); }
  if (fvar->legend != NULL) { cgpfigure_legend_dtor(&fvar->legend); }

  while (fvar->entry != NULL) {
    cgpentry *e = ((cgpentry *) cgpstack_pop(&fvar->entry));
    cgpentry_dtor(&e);
  }
  
  free((*fig)->fvar); (*fig)->fvar = NULL;  
  free(*fig); *fig = NULL;
}

static void cgpfigure_init_vtbl(
        cgpfigure *fig)
{
  fig->add_entry = cgpfigure_add_entry;
  fig->write_config = cgpfigure_write_config;
  fig->plot = cgpfigure_plot;
  
  fig->set_xaxis = cgpfigure_set_xaxis;
  fig->set_xscale = cgpfigure_set_xscale;
  fig->set_xrange = cgpfigure_set_xrange;
  fig->set_xmax = cgpfigure_set_xmax;
  fig->set_xmin = cgpfigure_set_xmin;
  
  fig->set_yaxis = cgpfigure_set_yaxis;
  fig->set_yscale = cgpfigure_set_yscale;
  fig->set_yrange = cgpfigure_set_yrange;
  fig->set_ymax = cgpfigure_set_ymax;
  fig->set_ymin = cgpfigure_set_ymin;

  fig->set_legend = cgpfigure_set_legend;
}

static void cgpfigure_axis_dtor(
        cgpaxis   **ax)
{
    if ((*ax)->scale != NULL) { free((*ax)->scale); }
    if ((*ax)->min != NULL) { free((*ax)->min); }
    if ((*ax)->max != NULL) { free((*ax)->max); }
    free(*ax); *ax = NULL;
}

static void cgpfigure_legend_dtor(
        cgplegend **legend)
{
    free(*legend);
    *legend = NULL;
}


static void cgpfigure_add_entry(
        cgpfigure  *fig,
	cgpentry   *entry)
{
  cgpstack_push(&fig->fvar->entry, entry);
}

static void cgpfigure_set_xaxis(
        cgpfigure  *fig,
        double      scale,
        double      xmin,
        double      xmax)
{
  cgpfigvar *fvar = fig->fvar;
  CGPFIGURE_ASSERT(fvar->xaxis, cgpaxis);
  cgpfigure_set_scale(fvar->xaxis, scale);
  cgpfigure_set_range(fvar->xaxis, xmin, xmax);
}

static void cgpfigure_set_xscale(
        cgpfigure  *fig,
	double      scale)
{
  cgpfigvar *fvar = fig->fvar;
  CGPFIGURE_ASSERT(fvar->xaxis, cgpaxis);
  cgpfigure_set_scale(fvar->xaxis, scale);
}

static void cgpfigure_set_xrange(
        cgpfigure  *fig,
	double      xmin,
	double      xmax)
{
  cgpfigvar *fvar = fig->fvar;
  CGPFIGURE_ASSERT(fvar->xaxis, cgpaxis);
  cgpfigure_set_range(fvar->xaxis, xmin, xmax);
}

static void cgpfigure_set_xmax(
        cgpfigure  *fig,
	double      xmax)
{
  cgpfigvar *fvar = fig->fvar;
  CGPFIGURE_ASSERT(fvar->xaxis, cgpaxis);
  cgpfigure_set_max(fvar->xaxis, xmax);
}

static void cgpfigure_set_xmin(
        cgpfigure  *fig,
	double      xmin)
{
  cgpfigvar *fvar = fig->fvar;
  CGPFIGURE_ASSERT(fvar->xaxis, cgpaxis);
  cgpfigure_set_min(fvar->xaxis, xmin);
}

static void cgpfigure_set_yaxis(
        cgpfigure  *fig,
        double      scale,
        double      ymin,
        double      ymax)
{
  cgpfigvar *fvar = fig->fvar;
  CGPFIGURE_ASSERT(fvar->yaxis, cgpaxis);
  cgpfigure_set_scale(fvar->yaxis, scale);
  cgpfigure_set_range(fvar->yaxis, ymin, ymax);
}

static void cgpfigure_set_yscale(
        cgpfigure  *fig,
	double      scale)
{
  cgpfigvar *fvar = fig->fvar;
  CGPFIGURE_ASSERT(fvar->yaxis, cgpaxis);
  cgpfigure_set_scale(fvar->yaxis, scale);
}

static void cgpfigure_set_yrange(
        cgpfigure  *fig,
	double      ymin,
	double      ymax)
{
  cgpfigvar *fvar = fig->fvar;
  CGPFIGURE_ASSERT(fvar->yaxis, cgpaxis);
  cgpfigure_set_range(fvar->yaxis, ymin, ymax);
}

static void cgpfigure_set_ymax(
        cgpfigure  *fig,
	double      ymax)
{
  cgpfigvar *fvar = fig->fvar;
  CGPFIGURE_ASSERT(fvar->yaxis, cgpaxis);
  cgpfigure_set_max(fvar->yaxis, ymax);
}

static void cgpfigure_set_ymin(
        cgpfigure  *fig,
	double      ymin)
{
  cgpfigvar *fvar = fig->fvar;
  CGPFIGURE_ASSERT(fvar->yaxis, cgpaxis);
  cgpfigure_set_min(fvar->yaxis, ymin);
}

static void cgpfigure_set_legend(
        cgpfigure  *fig,
	int         enabled)
{
  cgpfigvar *fvar = fig->fvar;
  CGPFIGURE_ASSERT(fvar->legend, cgplegend);
  fvar->legend->enabled = enabled;
}


static void cgpfigure_set_scale(
        cgpaxis    *ax,
	double      scale)
{
  if (scale > 1.0) {
    CGPFIGURE_ASSERT(ax->scale, double);
    *ax->scale = scale;
  }
}

static void cgpfigure_set_range(
        cgpaxis    *ax,
	double      min,
	double      max)
{
  if (min > max) {
    double tmp = min;
    min = max;
    max = tmp;
  }
  cgpfigure_set_max(ax, max);
  cgpfigure_set_min(ax, min);
}

static void cgpfigure_set_max(
        cgpaxis    *ax,
	double      max)
{
  CGPFIGURE_ASSERT(ax->max, double);
  *ax->max = max;
}

static void cgpfigure_set_min(
        cgpaxis    *ax,
	double      min)
{
  CGPFIGURE_ASSERT(ax->min, double);
  *ax->min = min;
}

static void cgpfiure_write_axis_config(
        cgpaxis    *ax,
	FILE       *gnuplot,
	char        axis)
{
    if (ax->scale != NULL) {
      fprintf(gnuplot, "set logscale %c %lf\n", axis, *ax->scale);
      fprintf(gnuplot, "set format %c \"%lf^{%%T}\"\n", axis, *ax->scale);
    }
    
    char min[64], max[64];
    if (ax->max != NULL) {
      sprintf(max, "%g", *ax->max);
    }
    else {
      sprintf(max, "*");
    }
    if (ax->min != NULL) {
      sprintf(min, "%g", *ax->min);
    }
    else {
      sprintf(min, "*");
    }
    fprintf(gnuplot, "set %crange [%s:%s]\n", axis, min, max);
}

static void cgpfigure_write_config(
        cgpfigure  *fig,
	FILE       *gnuplot)
{
  cgpfigvar *fvar = fig->fvar;
  
  if (fvar->xaxis != NULL) {
    cgpfiure_write_axis_config(fvar->xaxis, gnuplot, 'x');
  }

  if (fvar->yaxis != NULL) {
    cgpfiure_write_axis_config(fvar->yaxis, gnuplot, 'y');
  }

  fprintf(gnuplot, "%s key\n", fvar->legend->enabled ? "set" : "unset");
}

static void cgpfigure_plot(
        cgpfigure  *fig,
	FILE       *gnuplot)
{
  fprintf(gnuplot, "plot ");
  cgpfigvar *fvar = fig->fvar;
  while (fvar->entry != NULL) {
    cgpentry *entry = (cgpentry *) cgpstack_pop(&fvar->entry);
    entry->plot(entry, gnuplot);
    if (fvar->entry != NULL) {
      fprintf(gnuplot, ", ");
    }
    cgpentry_dtor(&entry);
  }
  fprintf(gnuplot, "\n");
}
