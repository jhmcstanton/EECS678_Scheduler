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
    bool responded_prev;
} job_t;


/**
   Comparison functions for each scheme
 */

int RR_comp(const job_t *in_queue, const job_t *new_job){
    return -1;
}

int FCFS_comp(const job_t *in_queue, const job_t *new_job){
    return -1; 
}

int SJF_comp(const job_t *in_queue, const job_t *new_job){
    return in_queue->run_time >= new_job->run_time;
}

int PSJF_comp(const job_t *in_queue, const job_t *new_job){
    return in_queue->time_remaining >= new_job->time_remaining;
}

int PRI_comp(const job_t *in_queue, const job_t *new_job){
    return in_queue->priority <= new_job->priority;
}

int PPRI_comp(const job_t *in_queue, const job_t *new_job){
    return -1; // fix this
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

    scheduler.avg_resp_time        = 0;
    scheduler.avg_wait_time        = 0;
    scheduler.avg_turn_around_time = 0;
}

void check_response_time(int time){
    job_t *job = priqueue_peek(&scheduler.jobs);
    if(job == NULL){
	return;
    } else if(!job->responded_prev){
	job->responded_prev = true;
	if(scheduler.avg_resp_time == 0){
	    scheduler.avg_resp_time = time - job->arrival_time;
	} else {
	    scheduler.avg_resp_time = (scheduler.avg_resp_time + (time - job->arrival_time)) / 2;
	}
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
    job_t *new_job = (job_t *) malloc(1 * sizeof(job_t));
    new_job->job_id         = job_number;
    new_job->arrival_time   = time;
    new_job->run_time       = running_time;
    new_job->priority       = priority;
    new_job->time_remaining = running_time;
    new_job->responded_prev = false;

    priqueue_offer(&scheduler.jobs, &new_job);
    check_response_time(time);
    // tentative, update this if multiple core option is done
    return 0;
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
int scheduler_job_finished(int core_id, int job_number, int time)
{
    job_t *job_cursor = priqueue_poll(&scheduler.jobs);
    int turn_around_time, wait_time;
    if(job_cursor != NULL){
	// get turnaround time updates
	turn_around_time = time - job_cursor->arrival_time;
	wait_time        = time - job_cursor->arrival_time - job_cursor->run_time;
	if(scheduler.avg_turn_around_time == 0){
	    scheduler.avg_turn_around_time = turn_around_time;
	} else {
	    scheduler.avg_turn_around_time  = (scheduler.avg_turn_around_time + turn_around_time) / 2;
	}

	// get wait time updates
	if(scheduler.avg_wait_time == 0){
	    scheduler.avg_wait_time = wait_time;
	} else {
	    scheduler.avg_wait_time = (scheduler.avg_wait_time + wait_time) / 2;
	}
	free(job_cursor);
    }
    // still assuming single core
    if(scheduler.jobs.size == 0){
	return -1;
    } else {
	check_response_time(time);

	job_cursor = priqueue_peek(&scheduler.jobs);
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
int scheduler_quantum_expired(int core_id, int time)
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


/**
  Returns the average waiting time of all jobs scheduled by your scheduler.

  Assumptions:
    - This function will only be called after all scheduling is complete (all jobs that have arrived will have finished and no new jobs will arrive).
  @return the average waiting time of all jobs scheduled.
 */
float scheduler_average_waiting_time()
{
    return scheduler.avg_wait_time;
}


/**
  Returns the average turnaround time of all jobs scheduled by your scheduler.

  Assumptions:
    - This function will only be called after all scheduling is complete (all jobs that have arrived will have finished and no new jobs will arrive).
  @return the average turnaround time of all jobs scheduled.
 */
float scheduler_average_turnaround_time()
{
    return scheduler.avg_turn_around_time;
}


/**
  Returns the average response time of all jobs scheduled by your scheduler.

  Assumptions:
    - This function will only be called after all scheduling is complete (all jobs that have arrived will have finished and no new jobs will arrive).
  @return the average response time of all jobs scheduled.
 */
float scheduler_average_response_time()
{
    return scheduler.avg_resp_time;
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
}

void debug_print(job_t *job){
    printf("Job [%d]: Priority: [%d], Arrival : [%d], Run-Time: [%d], Time-Remaining: [%d]\n",
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
    //    priqueue_mut_map(&scheduler.jobs, debug_print);
    node_t *cursor = scheduler.jobs.head;
    
    while(cursor != NULL){
	debug_print(cursor->element);
	cursor = cursor->next;
    }
}
