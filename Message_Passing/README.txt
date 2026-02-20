When generating the different lists, the range of randomly generated numbers was set to ensure that at least one number would 
appear in all of the lists. For example, when testing with an array of 20 elements, the range was set to 1–39. This 
guaranteed that the lists would have at least one shared number, according to the pigeonhole principle.

The problem was about finding common elements between three different lists, IBM, Columbia, and Welfare, which are 
distributed across three different processes in an MPI program. Each process sends its values to the other process,
which then checks if there are any common elements in the other lists. The goal is to find all common elements 
between all three lists and print them. Process 0 generates a list IBM with 20 random elements and sends each 
element to Process 1 (Columbia) to check if it exists in Columbia. Process 1 generates a list called Columbia with 20
random elements, receives a number from Process 0, checks if it exists in Columbia, and sends it to Process 2 (Welfare).
It also sends a flag valued 1 if the number was found in the list and 0 if it wasn't.
Process 2 generates a list called Welfare with 20 random elements, receives a number and a flag from Process 1 (Columbia),
and the flag that indicates if the number exists in Columbia, it checks if that 
number also exists in Welfare. If the number exists in Welfare, it prints the common number. If not, a flag 0 is
sent back to Process 1 to indicate the number doesn’t exist in Welfare. If process 1 receives a 1, it prints the number
and sends a flag to process 0 that prints the number if the flag is 1. The whole algorithm is therefore based in sending 
messages back and forth between the different processes to check the common values in the different lists.
