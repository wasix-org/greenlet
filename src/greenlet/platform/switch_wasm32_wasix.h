#define STACK_REFPLUS 1

#if defined(SLP_EVAL) || 1
#define STACK_MAGIC 0

// #define REGS_TO_SAVE "s1", "s2", "s3", "s4", "s5", \
// 		     "s6", "s7", "s8", "s9", "s10", "s11", "fs0", "fs1", \
// 		     "fs2", "fs3", "fs4", "fs5", "fs6", "fs7", "fs8", "fs9", \
// 		     "fs10", "fs11"

#define WASIX_STACK_SWITCH() \
do {                                                    \
    assert(switching_thread_state);  \
    assert(switching_thread_state->_stack_id);  \
    printf("Switching to stack id %i\n", switching_thread_state->_stack_id); \
    wasix_continuation_switch(switching_thread_state->_stack_id); \
} while (0)

[[gnu::optimize("O0")]]
static int __attribute__((optimize("O0")))
slp_switch(void)
{
  int ret;
  // long fp;
  long *stackref, stsizediff;

  // __asm__ volatile ("" : : : REGS_TO_SAVE);
  // __asm__ volatile ("mv %0, fp" : "=r" (fp) : );
  // __asm__ volatile ("mv %0, sp" : "=r" (stackref) : );
  __asm__ volatile ("global.get __stack_pointer" : : );
  __asm__ volatile ("local.set %0" : "=r" (stackref) : );
    // printf("Ooo stackpointer: %p\n", stackref);


  {
    // Switches away
      SLP_SAVE_STATE(stackref, stsizediff);
      // printf("Stsize diff: %i\n", stsizediff);
      // printf("Stsize diff: %p\n", stsizediff);
    //   __asm__ volatile (
	  // "add sp, sp, %0\n"
	  // "add fp, fp, %0\n"
	  // : /* no outputs */
	  // : "r" (stsizediff)
	  // );
      __asm__ volatile (
	  "local.get %0\n"
    "global.get __stack_pointer\n"
    "i32.add\n"
    "global.set __stack_pointer\n"
	  : /* no outputs */
	  : "r" (stsizediff)
	  );
      SLP_RESTORE_STATE();
    }
    // __asm__ volatile ("global.get __stack_pointer" : : );
    // __asm__ volatile ("local.set %0" : "=r" (stackref) : );
    // printf("New stackpointer: %p\n", stackref);
//   __asm__ volatile ("" : : : REGS_TO_SAVE);
// #if __riscv_xlen == 32
//   __asm__ volatile ("lw fp, %0" : : "m" (fp));
// #else
//   __asm__ volatile ("ld fp, %0" : : "m" (fp));
// #endif
//   __asm__ volatile ("mv %0, zero" : "=r" (ret) : );

  // TODO: Call switch here

  WASIX_STACK_SWITCH();

  __asm__ volatile (
    "i32.const 0\n"
    "local.set %0" : "=r" (ret) : );
  return ret;
}

#endif

