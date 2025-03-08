1. Your shell forks multiple child processes when executing piped commands. How does your implementation ensure that all child processes complete before the shell continues accepting user input? What would happen if you forgot to call waitpid() on all child processes?

_My implementation ensures that all child processes complete before the shell continues accepting user input by using waitpid() to ensure that the shell waits for each child process to finish before continuing. After each fork() the parent process callids waitpid() to allow the child process to to finish execuitng. If you forgot to call waitpid() on all child processes, the shell would not properly wait for the children to terminate which could lead to zombie processes._

2. The dup2() function is used to redirect input and output file descriptors. Explain why it is necessary to close unused pipe ends after calling dup2(). What could go wrong if you leave pipes open?

_It is necessary to close unused pipe ends because they are no longer needed and if you leave the unused pipe ends open then the file descriptors remain open in the process. This will cause the process to not be able to detect the end of the pipe correctly and could cause issues such as improper pipe handling, or hanging processes._

3. Your shell recognizes built-in commands (cd, exit, dragon). Unlike external commands, built-in commands do not require execvp(). Why is cd implemented as a built-in rather than an external command? What challenges would arise if cd were implemented as an external process?

_The cd command is implemented as a built-in command because it changes the current working directory of the shell process itself. If cd were implemented as an external command, it would execute in a child process, and the change in directory would only affect the child process, not the shell itself. Implementing cd as a built-in command ensures that the change affects the environment directly and allow consistency across shell sessions._

4. Currently, your shell supports a fixed number of piped commands (CMD_MAX). How would you modify your implementation to allow an arbitrary number of piped commands while still handling memory allocation efficiently? What trade-offs would you need to consider?

_I would modify the command_list_t structure to dynamically allocate memory for the list of commands instead of using a fixed-size array. This approach would allow the shell to handle any number of piped commands. The trade-off would be that dynamic memory allocation introduces overhead, and if memory management is not handled carefully, there could be memory leaks or fragmentation._
