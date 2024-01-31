/*
 *  Advanced Programming Course Thread Exercise
 *  Copyright (C) 2024  Roy Simanovich
 *
 *  This program is free software: you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation, either version 3 of the License, or
 *  (at your option) any later version.
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with this program.  If not, see <https://www.gnu.org/licenses/>.
 */

#define _GNU_SOURCE
#include "../include/prime_counter.h"
#include <sched.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

PQueue queue;

int nprocs() {
	cpu_set_t cs;
	CPU_ZERO(&cs);
	sched_getaffinity(0, sizeof(cs), &cs);
	return CPU_COUNT(&cs);
}

bool isPrime(int n) {
	if (n <= 1)
		return false;
	
	for (int i = 2; i * i <= n; i++)
	{
		if (n % i == 0)
			return false;
	}

	return true;
}

void *threadTask(void *arg) {
	int *thread_data = (int *)arg;

	while (true)
	{
		PTask task = (PTask) queueDequeue(queue);

		// Safe fail - should never happen.
		if (task == NULL)
			break;

		// If the task is -1, it means that the thread should exit
		else if (task->num == -1)
		{
			free(task);
			break;
		}

		else if (isPrime(task->num))
			(*thread_data)++;

		free(task);
	}

	return NULL;
}

int main() {
	int num;
	int total_counter = 0;

	// Get the number of processors in the system
	// 0.5 is used to avoid using all the processors and is optimal for this program.
	int proc_count = (nprocs() * 0.5);

	queue = queueCreate();

	if (queue == NULL)
	{
		fprintf(stderr, "Error creating queue.\n");
		exit(1);
	}

	pthread_t *threads = calloc(proc_count, sizeof(pthread_t)); 
	
	if (threads == NULL)
	{
		fprintf(stderr, "Error allocating memory for threads.\n");
		free(queue);
		exit(1);
	}

	int *thread_data = calloc(proc_count, sizeof(int));

	if (thread_data == NULL)
	{
		fprintf(stderr, "Error allocating memory for thread data.\n");
		free(threads);
		free(queue);
		exit(1);
	}

	for (int i = 0; i < proc_count; i++)
	{
		pthread_create((threads + i), NULL, &threadTask, (thread_data + i));
	}

	// Read numbers from stdin until end of file
	while (scanf("%d", &num) != EOF)
	{
		// Add number to queue
		PTask task = malloc(sizeof(Task));

		if (task == NULL)
		{
			fprintf(stderr, "Error allocating memory for task.\n");
			free(thread_data);
			free(threads);
			queueDestroy(queue);
			exit(1);
		}

		task->num = num;

		queueEnqueue(queue, task);
	}

	for (int i = 0; i < proc_count; i++)
	{
		PTask task = malloc(sizeof(Task));

		if (task == NULL)
		{
			fprintf(stderr, "Error allocating memory for task.\n");
			free(thread_data);
			free(threads);
			queueDestroy(queue);
			exit(1);
		}

		task->num = -1;

		queueEnqueue(queue, task);
	}

	for (int i = 0; i < proc_count; i++)
	{
		pthread_join(*(threads + i), NULL);

		total_counter += *(thread_data + i);
	}

	free(thread_data);
	free(threads);
	queueDestroy(queue);

	fprintf(stdout, "%d total primes.\n", total_counter);

	return 0;
}