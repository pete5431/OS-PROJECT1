Discrete-Event-Simulator
________________________________________

This program's goal to simulate how processes move through a simple computer system in a very watered down approach.

The simple computer system described will only stimulate a single-core CPU, meaning only one process can be in the CPU at a time.

In addition to the CPU, there will be two disks, disk1 and disk2, and also a network. These will be the components of this

simple computer system that will simulate I/O or network related tasks a process might do. The so called system will be as follows.

For simplicity, a process on the CPU will be referred to as a job, like something that must be done.

When a job arrives in the system, the job will head directly to the CPU. Since the CPU can only process one job at a time, if the 

CPU is busy, the job will have to wait in line for its turn to be executed by the CPU. After a job is done processing in the CPU,

the job can continue further computations or leave the system because it is done. In order to simplify the mechanism, there will

be a probability that the job exits, and will be determined through a generated pseudo-random number. If the job is determined to

continue in the system, it will either go to the network or the disks. Again, there will be a probability to enter the network

in a similar manner as the exit probability. In a similar manner to the CPU, a job must wait if the network is busy. If the job 

instead went to disk, then it will go into the disk that is least busy, or choose one at random if equal. Same as before, if either

disk is busy, the job has to wait. After a job is done at the disks or network it goes back to cpu, and the cycles continues until

the simulation ends.

CODE:

There will be read constants:
SEED - sets the seed for the random number generation in the program.
INIT_TIME - the time the system time starts from, will usually be 0.
FIN_TIME - the time the system ends.
ARRIVE_MIN - the minimum time for the generation of interarrival time
ARRIVE_MAX - the maximum time for the generation of interarrival time
QUIT_PROB - the probability the process quits.
NETWORK_PROB - the probability the process goes to network.
CPU_MIN - the min time for the generation of cpu process time
CPU_MAX - the max time for the generation of cpu process time
DISK1_MIN - the minimum time for the generation of disk1 process time
DISK1_MAX - the maximum time for the generation of disk1 process time
DISK2_MIN - the minimum time for the generation of disk2 process time
DISK2_MAX - the max time for the generation of disk2 process time
NETWORK_MIN - the min time for network process time
NETWORK_MAX - the max time for network process time

These constants will be read from a "config.txt", and will set the parameters for the simulation.

I have made a separate file called FileParam.cpp and FileParam.h to include a FileParam struct that basically contains these

constants and also holds statistics values as will be explained later. That way the FileParam object can be passed around and

obtain information from the handlers.

To handle the order of events in the proper time, a priority_queue is used. The one used in this program is the built in 

priority_queue from queue in the C++ library. The priority_queue will hold Event objects. 

The Event struct will hold three important variables, the time the event happens, what type the event is, and the process id or

job id. It will also override the operator < so that the priority_queue can recognize to order events based off their time so that

the lowest time is always at the top. Files "Event.cpp" and "Event.h" contain the Event struct and functions regarding

generating random numbers, creating new event, and print the event to the log file.

In order to simulate waiting for each component, a FIFO queue is used for each component. In the code a global queue for 

CPU, DISK1, DISK2, and NETWORK are declared. They will be accessed by the handler functions throughout. The queues used will also

be the built in queues from <queue> in the C++ library. 

In order to indicate if the queue is busy, we use bool variables, CPU_BUSY, DISK1_BUSY, DISK2_BUSY, and NETWORK_BUSY. They will

be defaulted to false, and changed to true once a job arrives at the respective component, and changed back to false once a job

leaves the respective components.

The system time will be a global int variable because it should be known by every function handler, and makes sense because

the system will govern all of the components. After all once system time reaches FIN_TIME, everything ends no matter what.

At the start of the program, the FileParam object is created and the constants are read in from file, the seed is set using

srand(), and event simulation_finish and the first job are added to the priority_queue.

There will be one main while loop that pops events off the p_queue, and then handles or calls the appropriate handler function

based off the event type. It will also calculate the statistics for like queue size for each component through each occurence.

job_arrival_handler - Sets the event to CPU_ARRV and push to p_queue if CPU isn't busy otherwise push to CPU queue to wait.
		      Also determines next arrival time, and add to p_queue.
cpu_arrival_handler - Sets the CPU_BUSY to true, and determines cpu finish time for the job and push to p_queue.
cpu_finish_handler - Sets the CPU_BUSY to false, and determines next cpu event if CPU queue is not empty, and push to p_queue.
		     Also determines the destination of finished cpu event, either exit, network, or to disks and push to p_queue.
disk1_arrival_handler - same as cpu_arrival_handler, but for DISK1.
disk1_finish_handler - set DISK1_BUSY to false, and then set event type to CPU_ARRV to go back to cpu, if CPU is busy, push to 
		       CPU queue, else push to p_queue. Get next disk1 event if DISK1 is not empty.
disk2_arrival_handler - same as disk1 except for disk2.
disk2_finish_handler - same as disk1 except for disk2.
network_arrival_handlet - same as disk2 except for network.
network_finish_handler - same as disk2 except for network.

On job exit or system finishes, it is simply print to log, as nothing else is necessary.

At the end of the while loop, the finalized statistics are calculated.

HOW I TESTED MY PROGRAM:

During the coding process I started by figuring out a way to read constants, but since these constant's values are set during run

time, they aren't really constant per se. So I made a struct FileParam that is passed around that holds constants and also contains

statistics values that are updated throughout. 

Then I made the Event struct, and the other tool functions I need to get started like the rand and create event functions, and

of course a print_log to see what happens.

After completion of the main while loop, I did each handler function in pairs for their respective components to make sure they 

worked logically before going on to the next. I experimented with multiple values for the constants at each step to make sure

they were working as intended. 

Finally I incorporated the statistics which did take a while because I decided it was better to calculate average queue size based

off per unit time instead of when the queue changed.


