Discrete-Event-Simulator
_______________________________________________________________________

This program's goal to simulate how processes move through a simple computer system in a very watered down approach.

The simple computer system described will only stimulate a single-core CPU, meaning only one process can be in the CPU at a time.

In addition to the CPU, there will be two disks, disk1 and disk2, and also a network, which also can only handle one process at a time. 

These will be the components of this simple computer system that will simulate I/O or network related tasks a process might do. 

The so called system will be as follows. For simplicity, a process on the CPU will be referred to as a job, like something that must be done.

When a job arrives in the system, the job will head directly to the CPU. Since the CPU can only process one job at a time, if the 

CPU is busy, the job will have to wait in line for its turn to be executed by the CPU. After a job is done processing in the CPU,

the job can continue further computations or leave the system because it is done. In order to simplify the mechanism, there will

be a probability that the job exits, and will be determined through a generated pseudo-random number. If the job is determined to

continue in the system, it will either go to the network or the disks. Again, there will be a probability to enter the network

in a similar manner as the exit probability. In a similar manner to the CPU, a job must wait if the network is busy. If the job 

instead went to disk, then it will go into the disk that is least busy, or choose one at random if equal. Same as before, if either

disk is busy, the job has to wait. After a job is done at the disks or network it goes back to cpu, and the cycles continues until

the simulation ends.

_______________________________________________________________________

CODE:

There will be read constants:
SEED - sets the seed for the random number generation in the program.
INIT_TIME - the time the system time starts from, will usually be 0.
FIN_TIME - the time the system ends.
ARRIVE_MIN - the minimum time for the generation of job interarrival time system
ARRIVE_MAX - the maximum time for the generation of job interarrival time into the system
QUIT_PROB - the probability the process quits.
NETWORK_PROB - the probability the process goes to network.
CPU_MIN - the min time for the generation of cpu process time (how long the job spends in cpu)
CPU_MAX - the max time for the generation of cpu process time
DISK1_MIN - the minimum time for the generation of disk1 process time (how long the job spends in disk1)
DISK1_MAX - the maximum time for the generation of disk1 process time
DISK2_MIN - the minimum time for the generation of disk2 process time (how long the job spends in disk2)
DISK2_MAX - the max time for the generation of disk2 process time
NETWORK_MIN - the min time for network process time (how long the job spends in network);
NETWORK_MAX - the max time for network process time

These constants will be read from a "config.txt", and will set the parameters for the simulation.

I have made a separate file called FileParam.cpp and FileParam.h to include a FileParam struct that basically contains these

constants and also holds statistics values as will be explained later. That way the FileParam object can be passed around and

obtain information from the handlers. I choose to do this way because it allows me to bunch all the constants into one struct object.

The constants aren't really constant in terms of the program because they change upon calling the read_file function for the FileParam struct,

which reads in all the constants from a config file. It turned out ok because FileParam ended serving another purpose since I passed it to

every handler function, which was to calculate the statistics by gathering information from the handlers.

To handle the order of events in the proper time, a priority_queue is used. The one used in this program is the built in 

priority_queue from queue in the C++ library. The priority_queue will hold Event objects. I figured instead of writing it all in c and

writing a queue, I can use c++ and use the built in queues. This ended saving me some time and hassle with making a queue from scratch.

The Event struct will hold three important variables, the time the event happens, what type the event is, and the process id or

job id. It will also override the operator < so that the priority_queue can recognize to order events based off their time so that

the lowest time is always at the top. Files "Event.cpp" and "Event.h" contain the Event struct and functions regarding

generating random numbers, creating new event, and print the event to the log file. I put them into a separate file because I didn't want to

bunch all the code in one file, but in the end it was easier to write all the handlers in the main.cpp.

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

based off the event type. This is done using a switch statement. Also at each event pop, the current system time is set equal to

the event time, because this way it avoids going over each incremenet of time where nothing happens.

job_arrival_handler - Sets the event to CPU_ARRV and push to p_queue if CPU isn't busy otherwise push to CPU queue to wait.
		      Also determines next arrival time, and add to p_queue.
cpu_arrival_handler - Sets the CPU_BUSY to true, and determines cpu finish time for the job and push to p_queue.
cpu_finish_handler - Sets the CPU_BUSY to false, and determines next cpu event if CPU queue is not empty, and push to p_queue.
		     Also determines the destination of finished cpu event, either exit, network, or to disks and push to p_queue.
disk1_arrival_handler - Sets disk1 to busy, and determine time the job spends in disk1 and push that as a DISK_FIN event to queue.
disk1_finish_handler - set DISK1_BUSY to false, and then set event type to CPU_ARRV to go back to cpu, if CPU is busy, push to 
		       CPU queue, else push to p_queue. Get next disk1 event if DISK1 is not empty.
disk2_arrival_handler - Sets disk2 to busy, and push DISK_FIN event onto queue after calculating the time required.
disk2_finish_handler - Sets disk2 to not busy, and sets event type to CPU_ARRV, and pushes to p_queue or CPU queue depending on if CPU is busy.
		       Then sees if DISK2 queue is not empty, and pops next DISK2 arrival event onto p_queue.
network_arrival_handlet - same as disk2 except for network.
network_finish_handler - same as disk2 except for network.

On job exit or system finishes, it is simply print to log, as nothing else is necessary.

At the end of the while loop, the finalized statistics are calculated.

The statistics calculated are :

Average and Max Queue Size for each component: I calculated average per unit time. I summed up the queue size at each unit of time, and then divided by FIN_TIME.

Average and Max Response Time for each component: Calculated by subtracting time job arrives in component from time job finishes in component.

Utilization of each component (time component is busy / total time): I calculated this by counting up time for each component when they were busy.

Throughput (number of jobs completed per unit time): I counted jobs finished for each component when a fin handler was called.

The FileParam files contains the variables for these statistics.

_______________________________________________________________________

HOW I TESTED MY PROGRAM and WORKFLOW:

The workflow for this assignment was first figuring out a way to read the constants from file, and thats when I came up with FileParam.cpp and FileParam.h

to create a FileParam struct to contain all the constants in one object and for easy access for all handler functions.

I then decided to use C++ because that way I can skip writing a queue, and the language was very similar in addition to allowing me to use

C libraries in my code along with C++ libraries. 

Then I made Event.cpp and Event.h that contained the main Event objects that will be used by the queues. I made random number generator functions

in the files because they were easy to incorporate. I added a print_log function later to print the events onto a file.

I thought I could put a bunch of separate functions on different files to server there purpose, and I was gonna make one for the handlers, but

I decided it was easier to keep track of all the handler functions in one file.

After completion of the main while loop, I did each handler function in pairs for their respective components to make sure they 

worked logically before going on to the next. I experimented with multiple values for the constants at each step to make sure

they were working as intended. I did alot of printing onto console, things like the entire log, the random numbers generated, individual randomly generated numbers,

the entire priority queue order to see if its in the right order, etc. By working through each part step by step I was able to avoid doing major changes to the code.

Finally I incorporated the statistics, and used FileParam to do so. 


