/** @file libscheduler.h
 */

#ifndef LIBSCHEDULER_H_
#define LIBSCHEDULER_H_

#include "../libpriqueue/libpriqueue.h"

#include <stdbool.h>

#define MIN_SIZE_ARRS 1000
#define SCALE_SIZE    2

/**
  Constants which represent the different scheduling algorithms
*/
typedef enum {FCFS = 0, SJF, PSJF, PRI, PPRI, RR} scheme_t;

typedef struct _scheduler_t_ {
    priqueue_t jobs;
    int num_cores;
    scheme_t scheme;
    comp_t pri_comp;
    // float avg_resp_time;
    float avg_wait_time;
        float avg_turn_around_time;
    // statistics
    int num_jobs_run; 
    int num_jobs_finished;
    int stat_arr_size;
    int *waiting_times;
    int *turn_around_times;
    int *response_times;
} scheduler_t;

void  scheduler_start_up               (int cores, scheme_t scheme);
int   scheduler_new_job                (int job_number, int time, int running_time, int priority);
int   scheduler_job_finished           (int core_id, int job_number, int time);
int   scheduler_quantum_expired        (int core_id, int time);
float scheduler_average_turnaround_time();
float scheduler_average_waiting_time   ();
float scheduler_average_response_time  ();
void  scheduler_clean_up               ();

void  scheduler_show_queue             ();

#endif /* LIBSCHEDULER_H_ */
