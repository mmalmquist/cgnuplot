#include <stdio.h>
#include <stdlib.h>

#include "../src/cgpfigure.h"
#include "../src/cgpentry.h"
#include "../src/cgpstack.h"
#include "../src/cgnuplot.h"

#define CNRM  "\x1B[0m"
#define CRED  "\x1B[31m"
#define CGRN  "\x1B[32m"
#define CYEL  "\x1B[33m"
#define CBLU  "\x1B[34m"
#define CMAG  "\x1B[35m"
#define CCYN  "\x1B[36m"
#define CWHT  "\x1B[37m"

int cgpstack_test(void);
int cgpplot_test(void);
double sqr(double x);

int main()
{

  int stacktest = cgpstack_test();
  fprintf(stdout, "cgpstack test complete.\nStatus: %s" CNRM "\n",
	  stacktest ? CGRN "PASS" : CRED "FAILED");
  if (!stacktest) {
    return EXIT_FAILURE;
  }

  int plottest = cgpplot_test();
  fprintf(stdout, "cgnuplot test complete.\nStatus: %s" CNRM "\n",
	  plottest ? CGRN "PASS" : CRED "FAILED");
  if (!plottest) {
    return EXIT_FAILURE;
  }

  fprintf(stdout, CGRN "All tests PASSED\n" CNRM);
  return EXIT_SUCCESS;
}

int cgpstack_test(void)
{
  fprintf(stdout, "Starting test unit for " CCYN "cgpstack.c" CNRM "\n");
  
  fprintf(stdout, CGRN ">" CNRM "Initializing stack root... \n");
  cgpstack *root = NULL;
  fprintf(stdout, CGRN "OK" CNRM "\n");

  fprintf(stdout, CGRN ">" CNRM "Pushing 3 entries of type (char *) onto the stack... \n");
  char *stack_content[] = {"Entry 0", "Entry 1", "Entry 2"};
  for (int i = 0; i < 3; ++i) {
    fprintf(stdout, "\tPush: '%s'\n", *(stack_content+i));
    cgpstack_push(&root, *(stack_content+i));
  }
  fprintf(stdout, CGRN "OK" CNRM "\n");
  
  fprintf(stdout, CGRN ">" CNRM "Popping the last 4 entries from the "
	          "stack and printinng them to standard output... \n");
  for (int i = 0; i < 4; ++i) {
    fprintf(stdout, "\tPop: '%s'\n", (char *)cgpstack_pop(&root));
  }
  fprintf(stdout, CGRN "OK" CNRM "\n");

  return 1;
  
}

int cgpplot_test(void)
{
  fprintf(stdout, "Starting test unit for " CCYN "cgnuplot.c" CNRM "\n");

  fprintf(stdout, "Initializing figure\n");
  cgpfigure *fig = NULL;
  cgpfigure_ctor(&fig);
  fprintf(stdout, CGRN "OK" CNRM "\n");
  
  fprintf(stdout, "Configuring figure axis\n");
  double xmin = -5, xmax = 5;
  double ymin = -1, ymax = 10;
  fig->set_xaxis(fig, 1.0, xmin, xmax);
  fig->set_yaxis(fig, 1.0, ymin, ymax);
  fig->set_legend(fig, 1);
  fprintf(stdout, CGRN "OK" CNRM "\n");
  
  fprintf(stdout, "Initializing figure entry\n");
  cgpentry *entry = NULL;
  cgpentry_ctor(&entry);
  fprintf(stdout, CGRN "OK" CNRM "\n");

  cgppoint dpoints[11];
  fprintf(stdout, "Creating linspace with %d points in region [%.2f, %.2f]"
	  " (abstol: %.2g)...\n", 11, -2.5, 2.5, 1e-6);
  cgppoint_linspace(dpoints, -2.5, 2.5, 11);
  double x = -2.5;
  for (int i = 0; i < 11; ++i) {
    fprintf(stdout, "Target: % 2.1f; Actual: % 2.1f\n", x, dpoints[i].x);
    if (dpoints[i].x - x < -1e-6 || dpoints[i].x - x > 1e-6) {
      return 0;
    }
    x += .5;
  }
  fprintf(stdout, CGRN "OK" CNRM "\n");

  fprintf(stdout, "Creating f(x)=x*x from linspace (abstol: %.2g)...\n", 1e-6);
  cgppoint_ffill(dpoints, 11, &sqr);
  x = -2.5;
  for (int i = 0; i < 11; ++i) {
    fprintf(stdout, "Target: % 2.1f; Actual: % 2.1f\n", (x*x), dpoints[i].y);
    if (dpoints[i].y - (x*x) < -1e-6 || dpoints[i].y - (x*x) > 1e-6) {
      return 0;
    }
    x += .5;
  }
  fprintf(stdout, CGRN "OK" CNRM "\n");

  fprintf(stdout, "Saving data to file...\n");
  entry->save_data(entry, dpoints, 11);
  fprintf(stdout, CGRN "OK" CNRM "\n");

  fprintf(stdout, "Setting up entry\n");
  entry->set_filepath(entry, "data.dat");
  entry->set_label(entry, "Test data");
  entry->set_color(entry, "#0000ff");
  entry->set_linestyle(entry, "--");
  entry->set_linewidth(entry, 1.9);
  entry->set_dottype(entry, "fdtriangle");
  entry->set_dotsize(entry, 5.6);
  fprintf(stdout, CGRN "OK" CNRM "\n");
  
  fprintf(stdout, "Adding entry to figure\n");
  fig->add_entry(fig, entry);
  fprintf(stdout, CGRN "OK" CNRM "\n");
  
  fprintf(stdout, "Plotting line\n");
  cgnuplot_plot2d(fig);
  fprintf(stdout, "If plot is visible, press return to close figure\n");
  fgetc(stdin);
  cgnuplot_close();
  fprintf(stdout, CGRN "OK" CNRM "\n");

  fprintf(stdout, "Deallocating figure\n");
  cgpfigure_dtor(&fig);
  fprintf(stdout, CGRN "OK" CNRM "\n");
  
  return 1;
}

double sqr(double x)
{
  return x * x;
}
