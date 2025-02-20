1. Can you think of why we use `fork/execvp` instead of just calling `execvp` directly? What value do you think the `fork` provides?

    > **Answer**:  _We use fork/execvp instead of calling execvp directory because fork creates a child process which runs seperately from the parent shell process. If we called execvp directly then the program would be realced entirely, which would prevent the user from using the shell once that process finishes. Forking keeps the shell alive and active, allowing the child process to handle execution of commands._

2. What happens if the fork() system call fails? How does your implementation handle this scenario?

    > **Answer**:  _If the fork() system call fails, it would return -1. This is > My implementation handles this scenarios by checking if pid is less than 0._

3. How does execvp() find the command to execute? What system environment variable plays a role in this process?

    > **Answer**:  _execvp() finds the command to execute by searching in directories listed in the PATH environment varaible, which looks in a colon seperated list of directories. If the command is found then it is executed, and if it is not found the the command fails and returns an error code._

4. What is the purpose of calling wait() in the parent process after forking? What would happen if we didn’t call it?

    > **Answer**:  _The purpose of calling wait() in the parent process after forking is so that the the shell pauses until the child process completes. If wait() is not called then the shell would accept input while the child process runs. This could cause multiple process to run at the same time and potentially lead to a zombie process. wait() helps to manage process handling and prevent errors._

5. In the referenced demo code we used WEXITSTATUS(). What information does this provide, and why is it important?

    > **Answer**:  _WEXITSTATUS() provides the exit status of the child process from the status code returned by calling wait(). The value returned is either the value passed to exit() by the child or teh return value of main(). This information is important because it helps the shell determine the success of a child process, which is usally indicated by an exit 0 status for success or non zero value for failure._

6. Describe how your implementation of build_cmd_buff() handles quoted arguments. Why is this necessary?

    > **Answer**:  _My implementation of build_cmd_buff() checks if an argument starts with a double quote, and treats everything except the second double quotation as part of one single argument. This is necessary because some arguments with spaces need to be treated as one stirng, not multiple._

7. What changes did you make to your parsing logic compared to the previous assignment? Were there any unexpected challenges in refactoring your old code?

    > **Answer**:  _Compared to the previous parsing logic in assignment 3, I added more checking for quoted arguments and ensured my error handling was correct. There weren't any particular chalanges that were unexpected,_

8. For this quesiton, you need to do some research on Linux signals. You can use [this google search](https://www.google.com/search?q=Linux+signals+overview+site%3Aman7.org+OR+site%3Alinux.die.net+OR+site%3Atldp.org&oq=Linux+signals+overview+site%3Aman7.org+OR+site%3Alinux.die.net+OR+site%3Atldp.org&gs_lcrp=EgZjaHJvbWUyBggAEEUYOdIBBzc2MGowajeoAgCwAgA&sourceid=chrome&ie=UTF-8) to get started.

- What is the purpose of signals in a Linux system, and how do they differ from other forms of interprocess communication (IPC)?

    > **Answer**:  _The purpose of signals in a Linux system is in order to notify proceses of differnet events like terminations requests, errors, or user interpurtions.  They are mainly used for control and notifications. They differ from other forms of interprocess communication in that they caa occur at any time, independently of processes. They also do not handle data transfer._

- Find and describe three commonly used signals (e.g., SIGKILL, SIGTERM, SIGINT). What are their typical use cases?

    > **Answer**:  _Three commonly used signals are SIGKILL, SIGINT, and SIGTERM. SIGKILL focefullt terminates a processs and is used when a process is unresponsive. SIGINT is called when the user presses Ctrl and C when they want to stop a running process, interrupting a process and usally terminating it. SIGTERM requests a process to terminate, which gives it a chance to clear up resources before finishing._

- What happens when a process receives SIGSTOP? Can it be caught or ignored like SIGINT? Why or why not?

    > **Answer**:  _When a process recieves SIGSTOP, it pauses. IT cannot be caught or ignored like SIGINT because it is designed to allow a user or system to suspend a process no matter what. It does not kill the process and allows it to be resumed at a later point using SIGCONT._
