#include <string.h>
#include <stdio.h>
#include <stdlib.h>

#include "cgppoint.h"
#include "cgpentry.h"

typedef struct cgp_line_t   cgpline;
typedef struct cgp_dot_t    cgpdot;

struct cgp_line_t {
  int *type;              /* dashed, dotted etc.     */
};

struct cgp_dot_t {
  int *type;              /* start, circle, dot etc  */
  double *size;           /* size of dot             */
};

struct cgp_entryvar_t {
  char     *filepath;     /* path to file with data  */
  cgpline  *line;         /* line settings           */
  cgpdot   *dot;          /* dot settings            */
  char     *color;        /* name or hex ('#FF00FF') */
  double   *width;        /* thickniess of line      */
  char     *label;        /* legend label            */
};

int lt_default = 1;       /* >0                      */
int dt_default = -1;
double dz_default = 1;
double lw_default = 1;

static void cgpentry_init_vtbl(
        cgpentry  *entry);
static void cgpentry_dot_default(
        cgpdot    **dot);
static void cgpentry_line_default(
        cgpline   **line);

static void cgpentry_set_filepath(
        cgpentry   *entry,
	const char *filepath);
static void cgpentry_set_label(
        cgpentry   *entry,
	const char *label);
static void cgpentry_set_color(
        cgpentry   *entry,
	const char *color);
static void cgpentry_save_data(
        cgpentry   *entry,
	cgppoint   *data,
	size_t      len);
static void cgpentry_plot(
        cgpentry   *entry,
	FILE       *gnuplot);
static void cgpentry_set_str(
        char          **dst,
	const char     *src);
static void cgpentry_set_linestyle(
        cgpentry   *entry,
	const char *style);
static void cgpentry_set_linewidth(
        cgpentry   *entry,
	double      width);
static void cgpentry_set_dottype(
        cgpentry   *entry,
	const char *style);
static void cgpentry_set_dotsize(
        cgpentry   *entry,
	double      size);

void cgpentry_ctor(
        cgpentry  **entry)
{
  *entry = (cgpentry *) malloc(sizeof(cgpentry));

  (*entry)->evar = (cgpentryvar *) malloc(sizeof(cgpentryvar));
  cgpentryvar *evar = (*entry)->evar;
  evar->filepath = NULL;
  cgpentry_line_default(&evar->line);
  evar->dot = NULL;
  evar->color = NULL;
  evar->width = (double *) malloc(sizeof(double));
  *evar->width = lw_default;
  evar->label = NULL;

  cgpentry_init_vtbl(*entry);
}

void cgpentry_dtor(
        cgpentry  **entry)
{
  cgpentryvar *evar = (*entry)->evar;
  
  if (evar->line != NULL) {
    if (evar->line->type != NULL) { free(evar->line->type); }
    free(evar->line);
  }
  if (evar->dot != NULL) {
    if (evar->dot->type != NULL) { free(evar->dot->type); }
    if (evar->dot->size != NULL) { free(evar->dot->size); }
    free(evar->dot);
  }
  if (evar->filepath != NULL) { free(evar->filepath); }
  if (evar->color != NULL) { free(evar->color); }
  if (evar->width != NULL) { free(evar->width); }
  if (evar->label != NULL) { free(evar->label); }

  free((*entry)->evar); (*entry)->evar = NULL;
  free (*entry); *entry = NULL;
}

static void cgpentry_init_vtbl(
        cgpentry  *entry)
{
  entry->set_filepath = cgpentry_set_filepath;
  entry->set_label = cgpentry_set_label;
  entry->set_color = cgpentry_set_color;
  entry->set_linestyle = cgpentry_set_linestyle;
  entry->set_linewidth = cgpentry_set_linewidth;
  entry->set_dottype = cgpentry_set_dottype;
  entry->set_dotsize = cgpentry_set_dotsize;
  entry->plot = cgpentry_plot;
  entry->save_data = cgpentry_save_data;
}

static void cgpentry_dot_default(
        cgpdot    **dot)
{
  *dot = (cgpdot *) malloc(sizeof(cgpdot));
  (*dot)->type = (int *) malloc(sizeof(int));
  *(*dot)->type = dt_default;
  (*dot)->size = (double *) malloc(sizeof(double));
  *(*dot)->size = dz_default;
}

static void cgpentry_line_default(
        cgpline   **line)
{
  *line = (cgpline *) malloc(sizeof(cgpline));
  (*line)->type = (int *) malloc(sizeof(int));
  *(*line)->type = lt_default;
}

static void cgpentry_set_filepath(
        cgpentry *entry,
	const char *filepath)
{
  cgpentry_set_str(&entry->evar->filepath, filepath);
}

static void cgpentry_set_color(
        cgpentry *entry,
	const char *color)
{
  cgpentry_set_str(&entry->evar->color, color);
}

static void cgpentry_set_label(
        cgpentry *entry,
	const char *label)
{
  cgpentry_set_str(&entry->evar->label, label);
}

static void cgpentry_save_data(
        cgpentry   *entry,
	cgppoint   *data,
	size_t      len)
{
  cgpentryvar *evar = entry->evar;
  if (evar->filepath == NULL) {
    fprintf(stderr, "Warning: entry has no filepath. Data was not saved.\n");
    return;
  }
  FILE *plotdata;
  if ((plotdata = fopen(evar->filepath, "w")) == NULL) {
    fprintf(stderr, "Warning: Could not open file '%s'. Data was not saved.\n",
	    evar->filepath);
    return;
  }
  for (size_t i = 0; i < len; i++) {
    fprintf(plotdata, "%g %g\n", (data+i)->x, (data+i)->y);
  }
  fflush(plotdata);
  fclose(plotdata);
}

static void cgpentry_plot(
        cgpentry   *entry,
	FILE       *gnuplot)
{
  cgpentryvar *evar = entry->evar;
  fprintf(gnuplot, "\'%s\' with ", evar->filepath);
    
  if (evar->line != NULL || evar->dot == NULL) {
    fprintf(gnuplot, "lines");
  }
  if (evar->dot != NULL) {
    fprintf(gnuplot, "points");
  }

  if (evar->line != NULL) {
    cgpline *line = evar->line;
    if (line->type != NULL) {
      fprintf(gnuplot, " dashtype %d ", *line->type);
    }
  }
    
  if (evar->dot != NULL) {
    cgpdot *dot = evar->dot;
    if (dot->type != NULL) {
      fprintf(gnuplot, " pointtype %d ", *dot->type);
    }
    if (dot->size != NULL) {
      fprintf(gnuplot, " pointsize %lf ", *dot->size);
    }
  }
    
  if (evar->color != NULL) {
    fprintf(gnuplot, " linecolor rgb '%s' ", evar->color);
  }
    
  if (evar->width != NULL) {
    fprintf(gnuplot, " linewidth %lf ", *evar->width);
  }

  if (evar->label != NULL) {
    fprintf(gnuplot, " title '%s' ", evar->label);
  }
}

static void cgpentry_set_str(
        char          **dst,
	const char     *src)
{
  size_t size = strlen(src) + 1;
  *dst = (char *) realloc(*dst, size);
  strcpy(*dst, src);
}

static void cgpentry_set_linestyle(
        cgpentry   *entry,
	const char *style)
{
  cgpentryvar *evar = entry->evar;
  if (evar->line == NULL) {
    evar->line = (cgpline *) calloc(1, sizeof(cgpline));
  }
  if (evar->line->type == NULL) {
    evar->line->type = (int *) malloc(sizeof(int));
  }
  
  if (strcmp(style, "-..") == 0) {
    *evar->line->type = 5;
  }
  else if (strcmp(style, "-.") == 0) {
    *evar->line->type = 4;
  }
  else if (strcmp(style, "--") == 0) {
    *evar->line->type = 2;
  }
  else if (strcmp(style, ".") == 0) {
    *evar->line->type = 3;
  }
  else  {             /* "-" */
    *evar->line->type = 1;
  }
}

static void cgpentry_set_linewidth(
        cgpentry   *entry,
	double      width)
{
  cgpentryvar *evar = entry->evar;
  if (evar->width == NULL) {
    evar->width = (double *) malloc(sizeof(double));
  }
  
  *evar->width = width;
}

static void cgpentry_set_dottype(
        cgpentry   *entry,
	const char *style)
{
  cgpentryvar *evar = entry->evar;
  if (evar->dot == NULL) {
    evar->dot = (cgpdot *) calloc(1, sizeof(cgpdot));
  }
  if (evar->dot->type == NULL) {
    evar->dot->type = (int *) malloc(sizeof(int));
  }
  
  if      (strcmp(style,  "plus")      == 0) { *evar->dot->type =  1; }
  else if (strcmp(style,  "cross")     == 0) { *evar->dot->type =  2; }
  else if (strcmp(style,  "asterisk")  == 0) { *evar->dot->type =  3; }
  else if (strcmp(style,  "square")    == 0) { *evar->dot->type =  4; }
  else if (strcmp(style, "fsquare")    == 0) { *evar->dot->type =  5; }
  else if (strcmp(style,  "circle")    == 0) { *evar->dot->type =  6; }
  else if (strcmp(style, "fcircle")    == 0) { *evar->dot->type =  7; }
  else if (strcmp(style,  "utriangle") == 0) { *evar->dot->type =  8; }
  else if (strcmp(style, "futriangle") == 0) { *evar->dot->type =  9; }
  else if (strcmp(style,  "dtriangle") == 0) { *evar->dot->type = 10; }
  else if (strcmp(style, "fdtriangle") == 0) { *evar->dot->type = 11; }
  else if (strcmp(style,  "diamond")   == 0) { *evar->dot->type = 12; }
  else if (strcmp(style, "fdiamond")   == 0) { *evar->dot->type = 13; }
  else if (strcmp(style,  "pentagon")  == 0) { *evar->dot->type = 14; }
  else if (strcmp(style, "fpentagon")  == 0) { *evar->dot->type = 15; }
  else                                       { *evar->dot->type =  0; }
}

static void cgpentry_set_dotsize(
        cgpentry   *entry,
	double      size)
{
  cgpentryvar *evar = entry->evar;
  if (evar->dot == NULL) {
    evar->dot = (cgpdot *) calloc(1, sizeof(cgpdot));
  }
  if (evar->dot->size == NULL) {
    evar->dot->size = (double *) malloc(sizeof(double));
  }

  *evar->dot->size = size;
}
