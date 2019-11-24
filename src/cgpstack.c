#include <stdlib.h>
#include <stdio.h>

#include "cgpstack.h"

struct cgp_stack_t {
  void *entry;
  cgpstack *next;
};

void cgpstack_push(cgpstack **stack, void *entry)
{
  cgpstack *new_entry = (cgpstack *) calloc(1, sizeof(cgpstack));
  new_entry->entry = entry;
  new_entry->next = *stack;
  *stack = new_entry;
}

void *cgpstack_pop(cgpstack **stack)
{
  if (*stack == NULL) return NULL;
  void *entry = (*stack)->entry;
  
  cgpstack *tmp = *stack;
  *stack = (*stack)->next;
  free(tmp);
  return entry;
}
