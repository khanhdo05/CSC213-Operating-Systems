#define _XOPEN_SOURCE
#define _XOPEN_SOURCE_EXTENDED

#include "scheduler.h"

#include <assert.h>
#include <curses.h>
#include <ucontext.h>

#include "util.h"

// This is an upper limit on the number of tasks we can create.
#define MAX_TASKS 128

// This is the size of each task's stack memory
#define STACK_SIZE 65536

// States that a task can be in
// Use enum so we don't have to flip multiple 1s 0s
typedef enum { READY, RUNNING, EXITED, SLEEPING, WAITING, BLOCKED } task_state_t;

// This struct will hold the all the necessary information for each task
typedef struct task_info {
  // This field stores all the state required to switch back to this task
  ucontext_t context;

  // This field stores another context. This one is only used when the task
  // is exiting.
  ucontext_t exit_context;

  // Keep track of this task's state.
  task_state_t state;

  // If the task is sleeping, when should it wake up?
  size_t wake_time;

  // If the task is waiting for another task, which task is it waiting for?
  task_t wait_task;

  // Was the task blocked waiting for user input? Once you successfully
  // read input, you will need to save it here so it can be returned.
  char user_input;

  // TODO: Add fields here so you can:
  //   a. Keep track of this task's state.
  //   b. If the task is sleeping, when should it wake up?
  //   c. If the task is waiting for another task, which task is it waiting for?
  //   d. Was the task blocked waiting for user input? Once you successfully
  //      read input, you will need to save it here so it can be returned.
} task_info_t;

int current_task = 0;          //< The handle of the currently-executing task
int num_tasks = 1;             //< The number of tasks created so far
task_info_t tasks[MAX_TASKS];  //< Information for every task

/**
 * Initialize the scheduler. Programs should call this before calling any other
 * functiosn in this file.
 */
void scheduler_init() {
  // Initialize the state of the scheduler
  // Always start with tasks[0] running
  tasks[0].state = RUNNING;
}

/**
 * Scheduler select and start another task to run. To be invoked when a task issues a blocking
 * operation or exits.
 */
void scheduler_run_next_task() {
  // TODO: Round robin manner, check if task ready
}

/**
 * Check if a task is ready or not. Also responsible for updating states.
 *
 * \param index The index into tasks that we want to find out if it's ready
 *
 * \return true if its state is READY, false if otherwise
 */
bool is_ready(task_t index) {
  // use switch case for clean code
  switch (tasks[index].state) {
    case SLEEPING:
      // Check if it's sleeping, has the time passed
      if (time_ms() > tasks[index].wake_time) {
        tasks[index].state = READY;
      }
      break;
    case WAITING:
      // Check if waiting for a task, is that waiting task in exited state
      if (tasks[(tasks[index].wait_task)].state == EXITED) {
        tasks[index].state = READY;
      }
      break;
    case BLOCKED:
      // Check if it's blocked, whether an input has been recorded
      if (tasks[index].user_input != ERR) {
        tasks[index].state = READY;
      }
      break;
    default:  // TODO: did we cover all cases? do we need to handle running and exited?
      break;
  }

  return tasks[index].state == READY;
}

/**
 * This function will execute when a task's function returns. This allows you
 * to update scheduler states and start another task. This function is run
 * because of how the contexts are set up in the task_create function.
 */
void task_exit() {
  // Update the current task's state as exited
  tasks[current_task].state = EXITED;

  // Call the scheduler
  scheduler_run_next_task();

  // TODO: do we need to make sure we never let an exited task come here?
}

/**
 * Create a new task and add it to the scheduler.
 *
 * \param handle  The handle for this task will be written to this location.
 * \param fn      The new task will run this function.
 */
void task_create(task_t* handle, task_fn_t fn) {
  // Claim an index for the new task
  int index = num_tasks;
  num_tasks++;

  // Set the task handle to this index, since task_t is just an int
  *handle = index;

  // We're going to make two contexts: one to run the task, and one that runs at the end of the task
  // so we can clean up. Start with the second

  // Set up initial states
  tasks[index].state = READY;
  tasks[index].user_input = ERR;
  tasks[index].wait_task = -1;  // because it's not waiting for any task yet
  tasks[index].wake_time = 0;

  // First, duplicate the current context as a starting point
  getcontext(&tasks[index].exit_context);

  // Set up a stack for the exit context
  tasks[index].exit_context.uc_stack.ss_sp = malloc(STACK_SIZE);
  tasks[index].exit_context.uc_stack.ss_size = STACK_SIZE;

  // Set up a context to run when the task function returns. This should call task_exit.
  makecontext(&tasks[index].exit_context, task_exit, 0);

  // Now we start with the task's actual running context
  getcontext(&tasks[index].context);

  // Allocate a stack for the new task and add it to the context
  tasks[index].context.uc_stack.ss_sp = malloc(STACK_SIZE);
  tasks[index].context.uc_stack.ss_size = STACK_SIZE;

  // Now set the uc_link field, which sets things up so our task will go to the exit context when
  // the task function finishes
  tasks[index].context.uc_link = &tasks[index].exit_context;

  // And finally, set up the context to execute the task function
  makecontext(&tasks[index].context, fn, 0);
}

/**
 * Wait for a task to finish. If the task has not yet finished, the scheduler should
 * suspend this task and wake it up later when the task specified by handle has exited.
 *
 * \param handle  This is the handle produced by task_create
 */
void task_wait(task_t handle) {
  // TODO: do we assume handle is valid index?

  // Return early if the task specified by handle has exited
  if (tasks[handle].state == EXITED) {
    return;
  }

  // Block this task until the specified task has exited.
  tasks[current_task].state = WAITING;
  tasks[current_task].wait_task = handle;

  // Call the scheduler to figure out and run next task
  scheduler_run_next_task();
}

/**
 * The currently-executing task should sleep for a specified time. If that time is larger
 * than zero, the scheduler should suspend this task and run a different task until at least
 * ms milliseconds have elapsed.
 *
 * \param ms  The number of milliseconds the task should sleep.
 */
void task_sleep(size_t ms) {
  // Block this task (make it sleep) until the requested time has elapsed.
  tasks[current_task].state = SLEEPING;
  // Record the time the task should wake up instead of the time left for it to sleep
  tasks[current_task].wake_time = time_ms() + ms;

  // Call scheduler to run a different task
  scheduler_run_next_task();
}

/**
 * Read a character from user input. If no input is available, the task should
 * block until input becomes available. The scheduler should run a different
 * task while this task is blocked.
 *
 * \returns The read character code
 */
int task_readchar() {
  // Block this task until there is input available.
  tasks[current_task].state = BLOCKED;

  // Call the scheduler
  scheduler_run_next_task();

  // To check for input, call getch(). If it returns ERR, no input was available.
  // Otherwise, getch() will returns the character code that was read.
  int c = getch();
  if (c != ERR) {
    tasks[current_task].user_input = c;
    return c;
  }

  return tasks[current_task].user_input;
}
