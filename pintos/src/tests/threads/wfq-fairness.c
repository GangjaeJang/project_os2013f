/* Checks that when the alarm clock wakes up threads, the
   higher-priority threads run first. */

#include <stdio.h>
#include "tests/threads/tests.h"
#include "threads/init.h"
#include "threads/malloc.h"
#include "threads/synch.h"
#include "threads/thread.h"
#include "devices/timer.h"
#include "tests/threads/wfq-fairness.h"
#include "threads/sched.h"

void
test_wfq_fairness (void) 
{
  /* Can configure test data.
   * fairness(threads) : 10(10), 20(9), 30(8), 40(7), 50(6)
   * iteration : positive integer */
  test_fairness(40, 10);
}

void
test_fairness (int fairness, int iteration)
{
  int i, j;
  int priority_index = (fairness / 10) - 1;
  int num_of_threads = 11 - (fairness / 10);
  struct test_data test[num_of_threads];
  
  /* This test does not work with the MLFQS. */
  ASSERT (!thread_mlfqs);

  wake_time = timer_ticks () + 1 * TIMER_FREQ;
  sema_init (&wait_sema, 0);

  /* Prints test specs. */
  msg("Test start--------------------");
  msg("fairness : %d, iteration : %d, threads : %d.", fairness, iteration, num_of_threads);

  printf("(wfq-fairness) weight : ");
  for (i = 0; i < num_of_threads; i++) 
    {
      test[i].terminate = 0;
      int priority = priority_table[priority_index][i];
      printf("%d ", prio_to_weight[priority + 20]);
      char name[16];
      snprintf (name, sizeof name, "priority %d", priority);
      /* Makes thread. */
      thread_create (name, priority, wfq_fairness_thread, &test[i]);
    }
  printf("\n");

  thread_set_priority (PRI_MIN);

  /* Wakes same time with child threads. */
  timer_sleep (wake_time - timer_ticks ());

  /* Checks exec time and writes. */
  for(i = 0; i <= iteration; i++) {
    if(i != 0)
      msg("----------Cycles : %d----------", i);
    for(j = 0; j < num_of_threads; j++) {
      if(i != 0) {
        msg("Thread %d's execution time : %d.", j + 1, test[j].runtime - test[j].prev_runtime);
      }
      test[j].prev_runtime = test[j].runtime;
      if(i == iteration)
        test[j].terminate = 1;
    }
    timer_sleep(1000);
  }

  for (i = 0; i < num_of_threads; i++)
    sema_down (&wait_sema);
}

static void
wfq_fairness_thread (void *_test) 
{
  struct test_data *test = _test;

  /* Busy-wait until the current time changes. */
  int64_t start_time = timer_ticks ();
  while (timer_elapsed (start_time) == 0)
    continue;

  /* Now we know we're at the very beginning of a timer tick, so
     we can call timer_sleep() without worrying about races
     between checking the time and a timer interrupt. */
  timer_sleep (wake_time - timer_ticks ());

  test->tid = thread_tid();
  while(test->terminate != 1)
    test->runtime = thread_runtime();

  sema_up (&wait_sema);
}
