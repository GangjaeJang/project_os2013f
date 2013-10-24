static thread_func wfq_fairness_thread;
static int64_t wake_time;
static struct semaphore wait_sema;

struct test_data
{
  int tid;
  long runtime;
  long prev_runtime;
  int terminate;
};

static const int priority_table[5][10] = {
  {-1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
  {-4, -1, -1, -1, -1, -1, -1, -1, -1},
  {-6, -1, -1, -1, -1, -1, -1, -1},
  {-7, -1, -1, -1, -1, -1, -1},
  {-8, -1, -1, -1, -1, -1},
};

void test_wfq_fairness(void);
void test_fairness(int fairness, int iteration);
static void wfq_fairness_thread(void *_test);