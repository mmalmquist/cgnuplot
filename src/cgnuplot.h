#ifndef CGNUPLOT_H 
#define CGNUPLOT_H

#include "cgpfigure.h"

int cgnuplot_open(void);
int cgnuplot_close(void);
void cgnuplot_plot2d(cgpfigure     *fig);

#endif // CGNUPLOT_H 
