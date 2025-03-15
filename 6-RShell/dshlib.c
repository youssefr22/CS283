#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <stdbool.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/wait.h>

#include "dshlib.h"



#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <stdbool.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/wait.h>

#include "dshlib.h"

/**** 
 **** FOR REMOTE SHELL USE YOUR SOLUTION FROM SHELL PART 3 HERE
 **** THE MAIN FUNCTION CALLS THIS ONE AS ITS ENTRY POINT TO
 **** EXECUTE THE SHELL LOCALLY
 ****
 */

/*
 * Implement your exec_local_cmd_loop function by building a loop that prompts the 
 * user for input.  Use the SH_PROMPT constant from dshlib.h and then
 * use fgets to accept user input.
 * 
 *      while(1){
 *        printf("%s", SH_PROMPT);
 *        if (fgets(cmd_buff, ARG_MAX, stdin) == NULL){
 *           printf("\n");
 *           break;
 *        }
 *        //remove the trailing \n from cmd_buff
 *        cmd_buff[strcspn(cmd_buff,"\n")] = '\0';
 * 
 *        //IMPLEMENT THE REST OF THE REQUIREMENTS
 *      }
 * 
 *   Also, use the constants in the dshlib.h in this code.  
 *      SH_CMD_MAX              maximum buffer size for user input
 *      EXIT_CMD                constant that terminates the dsh program
 *      SH_PROMPT               the shell prompt
 *      OK                      the command was parsed properly
 *      WARN_NO_CMDS            the user command was empty
 *      ERR_TOO_MANY_COMMANDS   too many pipes used
 *      ERR_MEMORY              dynamic memory management failure
 * 
 *   errors returned
 *      OK                     No error
 *      ERR_MEMORY             Dynamic memory management failure
 *      WARN_NO_CMDS           No commands parsed
 *      ERR_TOO_MANY_COMMANDS  too many pipes used
 *   
 *   console messages
 *      CMD_WARN_NO_CMD        print on WARN_NO_CMDS
 *      CMD_ERR_PIPE_LIMIT     print on ERR_TOO_MANY_COMMANDS
 *      CMD_ERR_EXECUTE        print on execution failure of external command
 * 
 *  Standard Library Functions You Might Want To Consider Using (assignment 1+)
 *      malloc(), free(), strlen(), fgets(), strcspn(), printf()
 * 
 *  Standard Library Functions You Might Want To Consider Using (assignment 2+)
 *      fork(), execvp(), exit(), chdir()
 */
int exec_local_cmd_loop() {
    char input[SH_CMD_MAX];
    command_list_t clist;
    int status = OK;

    while (1) {
        printf(SH_PROMPT);

        if (!fgets(input, SH_CMD_MAX, stdin)) {
            break;
        }

        input[strcspn(input, "\n")] = 0;

        if (build_cmd_list(input, &clist) == OK) {
            if (clist.num == 1) {
                if (exec_built_in_cmd(&clist.commands[0]) == BI_CMD_EXIT) {
                    status = 0;
                    break;
                }
                status = exec_cmd(&clist.commands[0]);
            } else {
                status = execute_pipeline(&clist);
            }
        }
    }
    return status;
}


int build_cmd_list(char *cmd_line, command_list_t *clist) {
    char *command;
    int commandCount = 0;
    clist->num = 0;
    memset(clist->commands, 0, sizeof(clist->commands));

    if (cmd_line == NULL || strlen(cmd_line) == 0 || strspn(cmd_line, " ") == strlen(cmd_line)) {
        return WARN_NO_CMDS;
    }

    command = strtok(cmd_line, PIPE_STRING);

    while (command != NULL) {
        char *endOfString = command + strlen(command) - 1;

        while (*command != '\0' && isspace(*command)) {
            command++;
        }

        while (endOfString > command && isspace(*endOfString)) {
            endOfString--;
        }
        *(endOfString + 1) = '\0';

        if (commandCount >= CMD_MAX) {
            return ERR_TOO_MANY_COMMANDS;
        }

        if (alloc_cmd_buff(&clist->commands[commandCount])) {
            return ERR_MEMORY;
        }

        if (build_cmd_buff(command, &clist->commands[commandCount])) {
            return ERR_CMD_OR_ARGS_TOO_BIG;
        }

        commandCount++;
        command = strtok(NULL, PIPE_STRING);
    }

    clist->num = commandCount;
    return OK;
}

int alloc_cmd_buff(cmd_buff_t *cmd_buff) {
    cmd_buff->_cmd_buffer = malloc(SH_CMD_MAX);
    if (cmd_buff->_cmd_buffer == NULL) {
        return ERR_MEMORY;
    }
    cmd_buff->argc = 0;
    for (int i = 0; i < CMD_ARGV_MAX; i++) {
        cmd_buff->argv[i] = NULL;
    }
    return OK;
}

int free_cmd_buff(cmd_buff_t *cmd_buff) {
    free(cmd_buff->_cmd_buffer);
    cmd_buff->_cmd_buffer = NULL;
    return OK;
}

int clear_cmd_buff(cmd_buff_t *cmd_buff) {
    cmd_buff->argc = 0;
    for (int i = 0; i < CMD_ARGV_MAX; i++) {
        cmd_buff->argv[i] = NULL;
    }
    return OK;
}

int build_cmd_buff(char *cmd_line, cmd_buff_t *cmd_buff) {
    clear_cmd_buff(cmd_buff);
    strcpy(cmd_buff->_cmd_buffer, cmd_line);

    char *ptr = cmd_buff->_cmd_buffer;
    int argc = 0;

    while (*ptr) {
        while (*ptr == ' ') ptr++;  
        if (*ptr == '\0') break;

        if (*ptr == '"') {
            ptr++; 
            cmd_buff->argv[argc++] = ptr;
            while (*ptr && *ptr != '"') ptr++;  
            if (*ptr == '"') *ptr++ = '\0'; 
        } else {
            cmd_buff->argv[argc++] = ptr;
            while (*ptr && *ptr != ' ') ptr++;
            if (*ptr == ' ') *ptr++ = '\0';
        }

        if (argc >= CMD_ARGV_MAX - 1) {
            return ERR_CMD_OR_ARGS_TOO_BIG;
        }
    }

    cmd_buff->argv[argc] = NULL;
    cmd_buff->argc = argc;

    return (argc == 0) ? WARN_NO_CMDS : OK;
}

Built_In_Cmds match_command(const char *input) {
    if (strcmp(input, "exit") == 0) {
        return BI_CMD_EXIT;
    } else if (strcmp(input, "cd") == 0) {
        return BI_CMD_CD;
    }
    return BI_NOT_BI;
}

Built_In_Cmds exec_built_in_cmd(cmd_buff_t *cmd) {
    if (cmd->argc == 0) {
        return BI_NOT_BI;
    }

    switch (match_command(cmd->argv[0])) {
        case BI_CMD_EXIT:
            printf("exiting...\n");
            return BI_CMD_EXIT;
        case BI_CMD_CD:
            if (cmd->argc > 1 && chdir(cmd->argv[1]) != 0) {
                return ERR_EXEC_CMD;
            }
            return BI_EXECUTED;
        default:
            return BI_NOT_BI;
    }
}

void execute_process(cmd_buff_t *cmd) {
    handle_input_redirection(cmd);
    handle_output_redirection(cmd);
    execvp(cmd->argv[0], cmd->argv);
    exit(ERR_EXEC_CMD);
}

int exec_cmd(cmd_buff_t *cmd) {
    Built_In_Cmds result = exec_built_in_cmd(cmd);
    if (result == BI_EXECUTED || result == BI_CMD_EXIT) {
        return OK;
    }
    
    pid_t pid = fork();
    if (pid == 0) {
        execute_process(cmd);
    } else if (pid > 0) {
        int status;
        waitpid(pid, &status, 0);
        return WIFEXITED(status) ? WEXITSTATUS(status) : ERR_EXEC_CMD;
    }
    return ERR_EXEC_CMD;
}


int execute_pipeline(command_list_t *clist) {
    int numberOfCommands = clist->num;
    int pipeFileDescriptors[2];
    int previousPipeRead = -1;
    pid_t pids[numberOfCommands];

    for (int currentCommandIndex = 0; currentCommandIndex < numberOfCommands; currentCommandIndex++) {
        if (currentCommandIndex < numberOfCommands - 1) {
            if (pipe(pipeFileDescriptors) == -1) {
                return ERR_EXEC_CMD;
            }
        }

        pid_t pid = fork();
        if (pid == 0) {
            if (currentCommandIndex > 0) {
                dup2(previousPipeRead, STDIN_FILENO);
                close(previousPipeRead);
            }

            if (currentCommandIndex < numberOfCommands - 1) {
                dup2(pipeFileDescriptors[1], STDOUT_FILENO);
                close(pipeFileDescriptors[1]);
                close(pipeFileDescriptors[0]);
            }

            execvp(clist->commands[currentCommandIndex].argv[0], clist->commands[currentCommandIndex].argv);
            exit(ERR_EXEC_CMD);
        } else if (pid > 0) {
            pids[currentCommandIndex] = pid;
            if (currentCommandIndex > 0) {
                close(previousPipeRead);
            }
            if (currentCommandIndex < numberOfCommands - 1) {
                previousPipeRead = pipeFileDescriptors[0];
                close(pipeFileDescriptors[1]);
            }
        } else {
            return ERR_EXEC_CMD;
        }
    }

    for (int currentChildProcess = 0; currentChildProcess < numberOfCommands; currentChildProcess++) {
        int status;
        waitpid(pids[currentChildProcess], &status, 0);
    }
    return OK;
}

int handle_input_redirection(cmd_buff_t *cmd) {
    if (cmd->input_file) {
        int fd = open(cmd->input_file, O_RDONLY);
        if (fd < 0) {
            perror("Error opening input file");
            exit(ERR_EXEC_CMD);
        }
        
        dup2(fd, STDIN_FILENO);
        close(fd);
    }
    return OK;
}

int handle_output_redirection(cmd_buff_t *cmd) {
    if (cmd->output_file != NULL) {
        int outputFileMode = O_WRONLY | O_CREAT;
        
        if (cmd->output_append) {
            outputFileMode |= O_APPEND;
        } else {
            outputFileMode |= O_TRUNC;
        }
        
        int outputFileDirection = open(cmd->output_file, outputFileMode, 0644);
        if (outputFileDirection < 0) {
            perror("Error opening output file");
            exit(ERR_EXEC_CMD);
        }
        
        dup2(outputFileDirection, STDOUT_FILENO);
        close(outputFileDirection);
    }
    return OK;
}