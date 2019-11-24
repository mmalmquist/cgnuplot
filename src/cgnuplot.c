#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

#include "cgpfigure.h"
#include "cgnuplot.h"

static FILE *gnuplot = NULL;

void cgnuplot_plot2d(cgpfigure *fig)
{
  if (!cgnuplot_open()) {
    return;
  }
  fig->write_config(fig, gnuplot);
  fig->plot(fig, gnuplot);

  fflush(gnuplot);
}

int cgnuplot_open(void)
{
  if (gnuplot != NULL) {
    return 1;
  }
  return (gnuplot = popen("gnuplot", "w")) != NULL ? 1 : 0;
}

int cgnuplot_close(void)
{
  return gnuplot != NULL ? pclose(gnuplot) : 0;
}
