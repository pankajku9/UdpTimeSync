#  Time Synchronization over UDP
This is implementation (in C/C++) of a simple client server system that uses UDP UNIX sockets to synchronize time between two machines. The client will connect to the server, and every second will print out the difference between its time and the server time.

#Description

1. First clients request the server timestamp.
2. Server sends its time stamp. When that packet is received at the client, it stores the difference between the received timestamp and its own clock as t1.
3. The client then sends a packet to the server with its own timestamp. The server sends the difference between the received timestamp and its own clock back to the client as t2.
4. t1 and t2 both include the "travel time" t of the packet plus the time difference between the clocks d.
5. Assuming that the travel time is the same in both derive, we can derive time difference(d) from these equation

        t1 = t - d   
        t2 = t + d   
        t1 + d = t2 – d  
        d = (t2 - t1)/2    
