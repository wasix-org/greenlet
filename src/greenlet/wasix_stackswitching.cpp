extern "C" {
typedef unsigned int StackId;

// Switch to a new stack and start running `run` there.
//
// The function must not return.
StackId stack_start(void (*run)(void)) {
    return 1;
}
// Switch to an already running stack.
void stack_switch(StackId stack) {
    return;
}
// Cleanup the stack identified by `stack`.
// The stack must exist
void stack_stop(StackId stack) {
    return;
}
}