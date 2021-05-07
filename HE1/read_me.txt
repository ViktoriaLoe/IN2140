I didn't have time to clean up and implement everything. After being stuck on memory issues for several days.


The **NewFSP server** description reads "It has one main loop, in which it ... 2.tries to deliver the next packet (or the final empty packet) to each connected RDP client," 
This description sounds like it will go though all connected clients, and check which ones are ready to receive data. Rather, my program never unneceasarily parses through the connections and only attempts to send data in the two cases it is possible
  1. After recevied ACK on previous data payload
  2. After a connection accpet
. Since it is only when an ACK is received, it is in the question to send more data/an empty packet. 


**select vs. recvfrom vs. poll**

Since only ack_packets 0x08 and data packets 0x04 can be dropped, I chose to only send connection accpets once and not ack the connections accept packet on the client side. Therefore the output_packet in my rdp_accept creates the packet locally, and doesn't allocate space for it in the memory.
