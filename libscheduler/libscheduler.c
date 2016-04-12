/** @file libscheduler.c
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "libscheduler.h"
#include "../libpriqueue/libpriqueue.h"


/*
  Globals for scheduling
 */

scheduler_t scheduler;

/**
  Stores information making up a job to be scheduled including any statistics.

  You may need to define some global variables or a struct to store your job queue elements. 
*/
typedef struct _job_t
{
    int time_remaining;
    int priority;
    int arrival_time;
    int run_time;
    int job_id;
    int last_ran;
} job_t;


/**
   Comparison functions for each scheme
 */

int RR_comp(__attribute__ ((unused)) const job_t *in_queue, __attribute__ ((unused))  const job_t *new_job){
    return -1;
}

int FCFS_comp(__attribute__ ((unused)) const job_t *in_queue, __attribute__ ((unused)) const job_t *new_job){
    return -1; 
}

int SJF_comp(const job_t *in_queue, const job_t *new_job){
    if (in_queue->last_ran != -1){
        return -1;
    } else {
        return in_queue->run_time - new_job->run_time;
    }
}

int PSJF_comp(const job_t *in_queue, const job_t *new_job){    
    return in_queue->time_remaining - new_job->time_remaining;
}

int PRI_comp(const job_t *in_queue, const job_t *new_job){
    if(in_queue->last_ran != -1){
      return -1;
    } else {
      return in_queue->priority - new_job->priority;
    }
}

int PPRI_comp(const job_t *in_queue, const job_t *new_job){
    return in_queue->priority - new_job->priority;
}


/**
   Helper function to determine the comparison function to use based on a user picked scheme.
   @param scheme - scheme chosen by user
   @return a function pointer for comparing two jobs
*/
comp_t get_comparer(scheme_t scheme){
    switch(scheme){
    case FCFS : return (comp_t) FCFS_comp;
    case SJF  : return (comp_t) SJF_comp ;
    case PSJF : return (comp_t) PSJF_comp;
    case PRI  : return (comp_t) PRI_comp ;
    case PPRI : return (comp_t) PPRI_comp;
    case RR   : return (comp_t) RR_comp;
    default   : return NULL;
    }
}


/**
  Initalizes the scheduler.
 
  Assumptions:
    - You may assume this will be the first scheduler function called.
    - You may assume this function will be called once once.
    - You may assume that cores is a positive, non-zero number.
    - You may assume that scheme is a valid scheduling scheme.

  @param cores the number of cores that is available by the scheduler. These cores will be known as core(id=0), core(id=1), ..., core(id=cores-1).
  @param scheme  the scheduling scheme that should be used. This value will be one of the six enum values of scheme_t
*/
void scheduler_start_up(int cores, scheme_t scheme)
{
    scheduler.num_cores = cores;
    scheduler.scheme    = scheme;
    scheduler.pri_comp  = get_comparer(scheme);
    priqueue_init(&scheduler.jobs, scheduler.pri_comp);

    //    scheduler.avg_resp_time        = 0;
    //scheduler.avg_wait_time        = 0;
    //scheduler.avg_turn_around_time = 0;
    
    scheduler.num_jobs_run      = 0;
    scheduler.num_jobs_finished = 0;
    scheduler.stat_arr_size     = MIN_SIZE_ARRS;
    scheduler.waiting_times     = (int *) malloc(MIN_SIZE_ARRS * sizeof(int));
    scheduler.turn_around_times = (int *) malloc(MIN_SIZE_ARRS * sizeof(int));
    scheduler.response_times    = (int *) malloc(MIN_SIZE_ARRS * sizeof(int));
}

void check_response_time(int time){
    job_t *job = priqueue_peek(&scheduler.jobs);
    if(job == NULL){
	return;
    } else if(job->last_ran == -1){
	scheduler.num_jobs_run++;
	
	if(scheduler.num_jobs_run == scheduler.stat_arr_size){
	    scheduler.stat_arr_size     *= SCALE_SIZE;
	    scheduler.waiting_times      = realloc(scheduler.waiting_times, scheduler.stat_arr_size * sizeof(int));
	    scheduler.turn_around_times  = realloc(scheduler.turn_around_times, scheduler.stat_arr_size * sizeof(int));
	    scheduler.response_times     = realloc(scheduler.response_times, scheduler.stat_arr_size * sizeof(int));
	}

	job->last_ran = time;
       
	scheduler.response_times[scheduler.num_jobs_run - 1] = time - job->arrival_time;
    }
}

/**
  Called when a new job arrives.
 
  If multiple cores are idle, the job should be assigned to the core with the
  lowest id.
  If the job arriving should be scheduled to run during the next
  time cycle, return the zero-based index of the core the job should be
  scheduled on. If another job is already running on the core specified,
  this will preempt the currently running job.
  Assumptions:
    - You may assume that every job wil have a unique arrival time.

  @param job_number a globally unique identification number of the job arriving.
  @param time the current time of the simulator.
  @param running_time the total number of time units this job will run before it will be finished.
  @param priority the priority of the job. (The lower the value, the higher the priority.)
  @return index of core job should be scheduled on
  @return -1 if no scheduling changes should be made. 
 
 */
int scheduler_new_job(int job_number, int time, int running_time, int priority)
{
    int cur_running_job     = -1 ; 

    // update the remaining time if needed
    job_t *cur_job = priqueue_peek(&scheduler.jobs);
    if(cur_job != NULL){
        cur_running_job         = cur_job->job_id;
	cur_job->time_remaining = cur_job->time_remaining - (time - cur_job->last_ran) ;
    }
    
    job_t *new_job = (job_t *) malloc(1 * sizeof(job_t));
    new_job->job_id         = job_number;
    new_job->arrival_time   = time;
    new_job->run_time       = running_time;
    new_job->priority       = priority;
    new_job->time_remaining = running_time;
    new_job->last_ran       = -1;


    priqueue_offer(&scheduler.jobs, new_job);
    check_response_time(time);

    // tentative, update this if multiple core option is done
    if(((job_t *)priqueue_peek(&scheduler.jobs))->job_id != cur_running_job){
	//      new_job->last_ran = time;
      return 0;
    } else { 
      return -1;
    }
}


/**
  Called when a job has completed execution.
 
  The core_id, job_number and time parameters are provided for convenience. You may be able to calculate the values with your own data structure.
  If any job should be scheduled to run on the core free'd up by the
  finished job, return the job_number of the job that should be scheduled to
  run on core core_id.
 
  @param core_id the zero-based index of the core where the job was located.
  @param job_number a globally unique identification number of the job.
  @param time the current time of the simulator.
  @return job_number of the job that should be scheduled to run on core core_id
  @return -1 if core should remain idle.
 */
int scheduler_job_finished(__attribute__ ((unused)) int core_id, __attribute__ ((unused)) int job_number, int time)
{
    job_t *job_cursor = priqueue_poll(&scheduler.jobs);
    scheduler.num_jobs_finished++;
    int turn_around_time, wait_time;
    if(job_cursor != NULL){
	// get turnaround time updates
	turn_around_time = time - job_cursor->arrival_time;
	wait_time        = time - job_cursor->arrival_time - job_cursor->run_time;

	scheduler.waiting_times[scheduler.num_jobs_finished     - 1] = wait_time;
	scheduler.turn_around_times[scheduler.num_jobs_finished - 1] = turn_around_time;

	free(job_cursor);
    }
    // still assuming single core
    if(scheduler.jobs.size == 0){
	return -1;
    } else {
	check_response_time(time);

	job_cursor = priqueue_peek(&scheduler.jobs);
	job_cursor->last_ran = time;
	return job_cursor->job_id;
    }    
}

/**
  When the scheme is set to RR, called when the quantum timer has expired
  on a core.
 
  If any job should be scheduled to run on the core free'd up by
  the quantum expiration, return the job_number of the job that should be
  scheduled to run on core core_id.

  @param core_id the zero-based index of the core where the quantum has expired.
  @param time the current time of the simulator. 
  @return job_number of the job that should be scheduled on core cord_id
  @return -1 if core should remain idle
 */
int scheduler_quantum_expired(__attribute__ ((unused)) int core_id, int time)
{
    // no jobs, remain idle
    if(scheduler.jobs.size == 0){
	return -1;
    }

    job_t *job = priqueue_poll(&scheduler.jobs);
    priqueue_offer(&scheduler.jobs, job);
    check_response_time(time);
    job        = priqueue_peek(&scheduler.jobs);
    return job->job_id;
}


float get_avg(int *buffer){
    int i;
    float avg = 0;
    for(i = 0; i < scheduler.num_jobs_run; i++){
	avg += buffer[i];
    }
    return avg / scheduler.num_jobs_run;
}

/**
  Returns the average waiting time of all jobs scheduled by your scheduler.

  Assumptions:
    - This function will only be called after all scheduling is complete (all jobs that have arrived will have finished and no new jobs will arrive).
  @return the average waiting time of all jobs scheduled.
 */
float scheduler_average_waiting_time()
{
    return get_avg(scheduler.waiting_times);
}


/**
  Returns the average turnaround time of all jobs scheduled by your scheduler.

  Assumptions:
    - This function will only be called after all scheduling is complete (all jobs that have arrived will have finished and no new jobs will arrive).
  @return the average turnaround time of all jobs scheduled.
 */
float scheduler_average_turnaround_time()
{
    return get_avg(scheduler.turn_around_times);
}


/**
  Returns the average response time of all jobs scheduled by your scheduler.

  Assumptions:
    - This function will only be called after all scheduling is complete (all jobs that have arrived will have finished and no new jobs will arrive).
  @return the average response time of all jobs scheduled.
 */
float scheduler_average_response_time()
{
    return get_avg(scheduler.response_times);
}


/**
  Free any memory associated with your scheduler.
 
  Assumptions:
    - This function will be the last function called in your library.
*/
void scheduler_clean_up()
{
    job_t *job_cursor;
    while(scheduler.jobs.size > 0){
	job_cursor = priqueue_poll(&scheduler.jobs);
	free(job_cursor);
    }
    priqueue_destroy(&scheduler.jobs);
    
    free(scheduler.waiting_times);
    free(scheduler.turn_around_times);
    free(scheduler.response_times);
}

void debug_print(const job_t *job){
    printf("  %14d | %14d | %14d | %14d | %14d \n",
	   job->job_id, job->priority, job->arrival_time, job->run_time, job->time_remaining);
}

/**
  This function may print out any debugging information you choose. This
  function will be called by the simulator after every call the simulator
  makes to your scheduler.
  In our provided output, we have implemented this function to list the jobs in the order they are to be scheduled. Furthermore, we have also listed the current state of the job (either running on a given core or idle). For example, if we have a non-preemptive algorithm and job(id=4) has began running, job(id=2) arrives with a higher priority, and job(id=1) arrives with a lower priority, the output in our sample output will be:

    2(-1) 4(0) 1(-1)  
  
  This function is not required and will not be graded. You may leave it
  blank if you do not find it useful.
 */
void scheduler_show_queue()
{
    printf("\n             Job |       Priority |        Arrival |       Run-Time | Time-Remaining\n");
    printf("-----------------|----------------|----------------|----------------|---------------\n");
    priqueue_mut_map(&scheduler.jobs, (map_apply_t) debug_print);
}
