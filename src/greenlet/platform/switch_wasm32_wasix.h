/**
 * this is the internal transfer function for WASIX contexts
 * 
 * As we cannot really manipulate the call stack/program counter in WASM,
 * we need to use helper functions provided by the WASIX context switching API.
 */
#define STACK_REFPLUS 1

#if defined(SLP_EVAL)

#define STACK_MAGIC 0

#define WASIX_CONTEXT_SWITCH() \
    do { \
        assert(switching_thread_state); \
        assert(switching_thread_state->_context_id); \
        int _ = wasix_context_switch(switching_thread_state->_context_id); \
    } while (0)

static int
slp_switch(void)
{
    int ret;
    long *stackref, stsizediff;

    // Save current C stack pointer into stackref.
    __asm__ volatile(
        "global.get __stack_pointer\n"
        "local.set %0"
        : "=r"(stackref)
        :);

    {
        SLP_SAVE_STATE(stackref, stsizediff);
        // Move the C stack pointer to the new stack
        __asm__ volatile(
            "local.get %0\n"
            "global.get __stack_pointer\n"
            "i32.add\n"
            "global.set __stack_pointer\n"
            : /* no outputs */
            : "r"(stsizediff));
        SLP_RESTORE_STATE();
    }

    // We now have the C stack pointer pointing to the correct C stack
    // Now we can switch the WASM callstack
    WASIX_CONTEXT_SWITCH();

    __asm__ volatile(
        "i32.const 0\n"
        "local.set %0"
        : "=r"(ret)
        :);
    return ret;
}

#endif
