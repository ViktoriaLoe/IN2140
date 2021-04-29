


The **NewFSP server** description reads "It has one main loop, in which it ... 
2.tries to deliver the next packet (or the final empty packet) to each connected RDP client,"
This description sounds like it will go though all connected clients, and check which ones are ready to receive data.
Rather, my program never unneceasarily parses through the connections and only attempts to send data after 
and ACK is received from a client. Since it is only when an ACK is received, it is in the question to send more data/an empty packet. 


**select vs. recvfrom vs. poll**


