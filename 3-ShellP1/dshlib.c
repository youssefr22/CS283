#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <ctype.h>

#include "dshlib.h"

/*
 *  build_cmd_list
 *    cmd_line:     the command line from the user
 *    clist *:      pointer to clist structure to be populated
 *
 *  This function builds the command_list_t structure passed by the caller
 *  It does this by first splitting the cmd_line into commands by spltting
 *  the string based on any pipe characters '|'.  It then traverses each
 *  command.  For each command (a substring of cmd_line), it then parses
 *  that command by taking the first token as the executable name, and
 *  then the remaining tokens as the arguments.
 *
 *  NOTE your implementation should be able to handle properly removing
 *  leading and trailing spaces!
 *
 *  errors returned:
 *
 *    OK:                      No Error
 *    ERR_TOO_MANY_COMMANDS:   There is a limit of CMD_MAX (see dshlib.h)
 *                             commands.
 *    ERR_CMD_OR_ARGS_TOO_BIG: One of the commands provided by the user
 *                             was larger than allowed, either the
 *                             executable name, or the arg string.
 *
 *  Standard Library Functions You Might Want To Consider Using
 *      memset(), strcmp(), strcpy(), strtok(), strlen(), strchr()
 */
int build_cmd_list(char *cmd_line, command_list_t *clist) {
    if (!cmd_line || !clist) {
        return WARN_NO_CMDS;
    }
    
    memset(clist, 0, sizeof(command_list_t));
    int cmd_count = 0;
    char *token = strtok(cmd_line, PIPE_STRING);
    
    if (!token) {
        return WARN_NO_CMDS;
    }
    
    while (token) {
        if (cmd_count >= CMD_MAX) {
            return ERR_TOO_MANY_COMMANDS;
        }
        
        while (isspace((unsigned char)*token)) {
            token++;
        }
        
        if (strlen(token) >= ARG_MAX) {
            return ERR_CMD_OR_ARGS_TOO_BIG;
        }
        
        char *space_ptr = strchr(token, SPACE_CHAR);
        
        if (space_ptr) {
            size_t exe_len = space_ptr - token;
            if (exe_len >= EXE_MAX) {
                return ERR_CMD_OR_ARGS_TOO_BIG;
            }
            
            strncpy(clist->commands[cmd_count].exe, token, exe_len);
            clist->commands[cmd_count].exe[exe_len] = '\0';
            
            space_ptr++;
            while (isspace((unsigned char)*space_ptr)) {
                space_ptr++;
            }
            
            strncpy(clist->commands[cmd_count].args, space_ptr, ARG_MAX);
            clist->commands[cmd_count].args[ARG_MAX - 1] = '\0';
        } else {
            if (strlen(token) >= EXE_MAX) {
                return ERR_CMD_OR_ARGS_TOO_BIG;
            }
            
            strncpy(clist->commands[cmd_count].exe, token, EXE_MAX);
            clist->commands[cmd_count].exe[EXE_MAX - 1] = '\0';
        }
        
        cmd_count++;
        token = strtok(NULL, PIPE_STRING);
    }
    
    clist->num = cmd_count;
    return OK;
}