1. How does the remote client determine when a command's output is fully received from the server, and what techniques can be used to handle partial reads or ensure complete message transmission?

The remote client determines the end of a command's output using application-level protocols, such as predefined message delimiters like newline or null characters. To handle partial reads, the client continuously reads from the socket in a loop until the complete message is received, checking for expected delimiters or verifying the received data length against the expected size.

2. This week's lecture on TCP explains that it is a reliable stream protocol rather than a message-oriented one. Since TCP does not preserve message boundaries, how should a networked shell protocol define and detect the beginning and end of a command sent over a TCP connection? What challenges arise if this is not handled correctly?

A networked shell protocol should define message boundaries using techniques like length-prefix encoding, special delimiters \n or EOF markers, or application-layer framing. If message boundaries are not correctly defined, the client may read incomplete commands, concatenate multiple messages incorrectly, or experience deadlocks if it waits indefinitely for missing data.

3. Describe the general differences between stateful and stateless protocols.

Stateful protocols maintain session information between client and server across multiple interactions. Stateless protocols treat each request as independent, without memory of past interactions. Stateful protocols offer continuity but require more resources, while stateless protocols scale better and are easier to recover from failures.

4. Our lecture this week stated that UDP is "unreliable". If that is the case, why would we ever use it?

UDP is used for applications that prioritize low latency over reliability, such as real-time streaming, VoIP, DNS, and gaming. Its lack of connection overhead and retransmissions makes it faster and more efficient for time-sensitive communication where occasional packet loss is acceptable.

5. What interface/abstraction is provided by the operating system to enable applications to use network communications?

The OS provides the sockets API, which allows applications to create, send, receive, and manage network connections. Sockets support both TCP (stream-based) and UDP (datagram-based) communication and abstract low-level networking details, enabling developers to focus on protocol implementation.
