# Practice Exercises

6.1 The clock gets updated at each clock interrupt. Disabling interrupts for a large amount of time can make the clock lose 
the correct time. This can be minimized by disabling interrupts only for very short amounts of time.  

6.2 When a process is halting on an infinite loop that is waiting for a condition to be asserted by another process to proceed. 
This means that it does not do useful work but it consumes CPU cycles. Busy waiting can be avoided altogether by providing the
ability to suspend a process and the ability to wake it up by the OS. This introduces the overhead of switching from and to the process.  

6.3 In a single-core system, it's not appropriate because it would waste irreplaceable clock cycles that can be used to do more useful work.
Also, it would not allow other processes (that would assert the condition on which the busy waiting breaks.)
On multicore systems, more than one process can run simultaneously so it's more acceptable to waste maybe-unused CPU cycles on a different core.

6.4 If two `wait()` calls get interleaved, it's possible a number of  processes more than allowed would decrement the value of the semaphore and hence more than the allowed number of processes will be executing the critical section, which violates mutual exclusion.

6.5 Having all the processes compete for locking the mutex (binary semaphore).


```
sem s;
init_sem(s, 1);

// n processes
do {
	// entry section
	wait(x);  
    // critical section
    signal(x);
	// remainder section
} while (true);
```

6.6 The previous balance amount would be loaded in both withdraw() and deposit(), withdraw() would decrement its local value,
deposit() would increase its local value of the balance, and it's undeterministic whether the value calculated after either
fucntion will be the eventual value stored as the balance. This can be solved by locking the balance, so not allowing two
operations to update it contemporaneously.
