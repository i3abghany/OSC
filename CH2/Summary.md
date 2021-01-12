# 2.11 Summary

* An OS provides an environment for the execution of programs by providing services for users and programs.
* The three primary approaches for interacting with an OS are:
  * Command interpreters
  * Graphical user interfaces
  * Touch screen interfaces
* System calls provide an interface to the services made available by the OS. Programmers use system call's application programming interface API for accessing system-call services.
* System calls can ve divided into 6 groups
  * Process control
  * File management
  * Device management
  * Information maintenance
  * Communication
  * Protection
* The standard C library provides the system-call interface for UNIX and Linux systems.
* OSs include a collection of system programs that provide utilities to users.
* Applications are OS-specific because of the binary object formats, different instruction sets for different CPU architectures, and system calls that vary from one OS to another.
* Monolithic kernel:
  * It has no structure.
  * All functionality is provided in a single, static binary file.
  * It all runs in the context of one process.
  * All in the same address space.
  * Efficiency advantage over other structures.
* Layered kernel:
  * Divided into several discrete layers
  * The bottom-most layer is the hardware
  * The upper-most layer is the user interface
  * Degraded performance
  * Hard to define each layer's functionality.
* Microkernel:
  * The kernel is as minimal as possible
  * Implements the most basic functionalities
  * Most services are implemented as user-space programs.
  * Communication is done through the kernel using message-passing.
* Modular Approach
  * Minimal kernel design
  * Make the kernel extensible
  * Load modules dynamically as needed.
  * Combination of the monolithic kernel and microkernel approaches.
* A boot loader loads an OS into memory, can perform initialization and diagnosis.
* The performance of an OS can be monitored using either counters or tracing.
  * Counters are a collection of system-wide or per-process statistics.
  * Tracing follows the occurance of a specific event.
  