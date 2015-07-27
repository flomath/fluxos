---
layout: page
title: "Inter-Process Communication"
category: doc
date: 2014-01-08 12:30:00
---

## Inter-Process Communication (IPC)

IPC is needed for processes to communicate with each other and still have good decoupling of processes. An example for its use is resource sharing and process synchronization.

Currently we have implemented a [Semaphore][@semaphore], which is used by our console and the UART driver. The relationship between these two parts can be best described as the producer-consumer problem. We have a producer, which increases a resource that must be *consumed* by some consumer. The consumer has to wait for the resource to be increased.
The console process is the consumer as it has to wait for input data to process. The UART driver is the producer as it processes the input signals to *consumable* data.

The console process is blocked and therefore won't receive any process time as long there is no input coming from the UART driver.

### Semaphore

Our semaphore is always initialized with `0`. Whenever a process needs a resource the semaphore is decreased. If the semaphore value falls below `0`, the processes is set to blocked and is added to a waiting queue. Then another process continues.

When a process increases the semaphore (for example because the resource is not needed anymore) and there are waiting processes the longest waiting is retrieved from the queue and can continue.

Below you can see some parts of our implementation.

```c
void sem_wait(sem_t* sem) {
  // Ensure you don't get scheduled away
  atom_begin();

  // Decrease the semaphore value
  sem->value--;
  if (sem->value < 0) {
    // Get the current process
    int process = scheduler_getCurrentProcessId();

    // Enqueue the process
    sem_enqueue(sem, process);

    // Suspend the process by setting its state to BLOCKED
    scheduler_suspend(process);

    // Ensure that the scheduler can continue
    atom_end();

    // Yield is not implemented, we wait until we get scheduled away
    //yield();

    // Get the process context block
    PCB_t* pcb = scheduler_getCurrentProcess();

    // Wait until state has changed again
    while (pcb->state == PROCESS_BLOCKED);
    // Dirty fix: May happen when there is only one process so that the scheduler won't schedule away
    if (pcb->state == PROCESS_READY) {
      pcb->state = PROCESS_RUNNING;
    }

    return;
  }

  // Ensure that the scheduler can continue
  atom_end();
}

void sem_post(sem_t* sem) {
  // Ensure you don't get scheduled away
  atom_begin();

  // Increase the semaphore
  sem->value++;

  // Deque a sleeping process if there is one waiting
  if (sem->value <= 0) {
    int process = sem_dequeue(sem);

    // Let the process continue by setting its state to READY
    scheduler_continue(process);
  }

  // Ensure that the scheduler can continue
  atom_end();
}
```

[@semaphore]: http://www.cs.utexas.edu/users/EWD/transcriptions/EWD00xx/EWD74.html "Over seinpalen, Dijkstra, Edsger W., Transcription"
