#ifndef WASIX_STACKSWITCHING_H
#define WASIX_STACKSWITCHING_H

#ifdef __cplusplus
extern "C" {
#endif

typedef unsigned int StackId;

// Problem: stack_start cannot be like switch if it returns the stackid, because then we will only get the stackid after switching back, which is too late.
// Solution 1: Have stack_start return void and never return.
// Solution 2: Have stack_start not actually start the stack, but just allocate it and return an id. Then have a separate stack_switch function that switches to an already started stack
// Solution 3: Have stack_start not return the stack id, but give it a second parameter to an address where it can store the stack id
// Solution 4: Give the stack id as a parameter to the run function

// Switch to a new stack and start running `run` there.
//
// The function must not return.
// StackId stack_start(void (*run)(void));
// void stack_start(void (*run)(StackId));
// void stack_start(void (*run)(void), StackId* out_id);
// StackId stack_create(void (*run)(void*), void* arg);
int stack_create(StackId* out_id, void (*run)(void));
// Switch to an already running stack.
int stack_switch(StackId stack);
// Cleanup the stack identified by `stack`.
// The stack must exist
int stack_stop(StackId stack);

#ifdef __cplusplus
}
#endif

#endif /* WASIX_STACKSWITCHING_H */