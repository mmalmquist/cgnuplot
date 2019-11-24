#ifndef CGPENTRY_H
#define CGPENTRY_H

#include "cgppoint.h"

typedef struct cgp_entry_t    cgpentry;
typedef struct cgp_entryvar_t cgpentryvar;

struct cgp_entry_t {
  cgpentryvar *evar;
  
  void (*set_filepath)(
        cgpentry *entry,
	const char *filepath);
  void (*set_label)(
        cgpentry *entry,
	const char *label);
  void (*set_color)(
        cgpentry *entry,
	const char *color);
  void (*set_linestyle)(
        cgpentry   *entry,
	const char *style);
  void (*set_linewidth)(
        cgpentry   *entry,
	double      width);
  void (*set_dottype)(
        cgpentry   *entry,
	const char *style);
  void (*set_dotsize)(
        cgpentry   *entry,
	double      size);
  void (*plot)(
        cgpentry   *entry,
	FILE       *gnuplot);
  void (*save_data)(
        cgpentry   *entry,
	cgppoint   *data,
	size_t      len);
};

void cgpentry_ctor(
        cgpentry  **entry);
void cgpentry_dtor(
        cgpentry  **entry);

#endif //CGPENTRY_H
