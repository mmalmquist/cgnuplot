#ifndef CGPSTACK_H
#define CGPSTACK_H

typedef struct cgp_stack_t cgpstack;

void cgpstack_push(cgpstack **stack, void *entry);
void *cgpstack_pop(cgpstack **stack);

#endif // CGPSTACK_H
