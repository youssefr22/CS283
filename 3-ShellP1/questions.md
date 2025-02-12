1. In this assignment I suggested you use `fgets()` to get user input in the main while loop. Why is `fgets()` a good choice for this application?

    > **Answer**:  _fgets() is a good choice for this application because it reads a full line of input and prevents buffer overflow by limiting the number of characters actually read. It stops at a new line or when the buffer limit is reach, which makes sure that input is correctly handled._

2. You needed to use `malloc()` to allocte memory for `cmd_buff` in `dsh_cli.c`. Can you explain why you needed to do that, instead of allocating a fixed-size array?

    > **Answer**:  _Using malloc() to allocate memory allows for dynamic allocation, which allows flexibility in the program and allows the shell to adjust based on the amount needed. A fixed array could either waste memory if it is too big or fail to accomadate the scope of the program if it is too small._


3. In `dshlib.c`, the function `build_cmd_list(`)` must trim leading and trailing spaces from each command before storing it. Why is this necessary? If we didn't trim spaces, what kind of issues might arise when executing commands in our shell?

    > **Answer**:  _Triming leading and trailing spaces from each command is necessary because it ensure that commands are properly parsed. If we didn't trim spaces, issues like passing empty strings, not recognizing known commands, and mishandling of pipes and arguments. Trimming spaces ensures that commands and arguments will be parsed and executed on as intended._

4. For this question you need to do some research on STDIN, STDOUT, and STDERR in Linux. We've learned this week that shells are "robust brokers of input and output". Google _"linux shell stdin stdout stderr explained"_ to get started.

- One topic you should have found information on is "redirection". Please provide at least 3 redirection examples that we should implement in our custom shell, and explain what challenges we might have implementing them.

    > **Answer**:  _One redeirection example is output redirection, which uses > and >>. A challenge we might have implementing it is that we would have to open a file in write mode and ensure we redirect STDOUT properly. Another redeirection example is input redirection, which uses >. A challenge we might have implementing it is that the shell must open a file and ensure we redirect STDIN properly.Another redeirection example is error redirection, which uses 2>. A challenge we might have implementing it is that we must ensure that normal output is not affected and that we redirect STDERR properly._

- You should have also learned about "pipes". Redirection and piping both involve controlling input and output in the shell, but they serve different purposes. Explain the key differences between redirection and piping.

    > **Answer**:  _The difference between redirection and piping is that redirection modifes where input and output are read from or written to, while piping connects output from one command to another by using it as input. Redirection largel deals with files while piping passes information and chains together commands._

- STDERR is often used for error messages, while STDOUT is for regular output. Why is it important to keep these separate in a shell?

    > **Answer**:  _It is important to keep STDERR and STDOUT seperate in a shell because we do not want to confuse error outputs with normal ouputs. If there was not a distinction between them, then it would be harder to differentiate between valid results and errored results._

- How should our custom shell handle errors from commands that fail? Consider cases where a command outputs both STDOUT and STDERR. Should we provide a way to merge them, and if so, how?

    > **Answer**:  _The custom shell should handle errors by using return codes and printing out logical and descriptive error messages using STDERR, ensuring that we are avoiding exiting the shell. We should provide a way to merge outputs form STDOUT and STDERR, which can be done using redirection._