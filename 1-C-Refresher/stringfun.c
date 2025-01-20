#include <stdio.h>
#include <string.h>
#include <stdlib.h>


#define BUFFER_SZ 50

//prototypes
void usage(char *);
void print_buff(char *, int);
int  setup_buff(char *, char *, int);

//prototypes for functions to handle required functionality
int  count_words(char *, int);
//add additional prototypes here
int reverse_string(char*, int);
int word_print(char*, int);

int setup_buff(char *buff, char *user_str, int len){
	//TODO: #4:  Implement the setup buff as per the directions
	 char *src = user_str;
	 char *dest = buff;
	 int real_len = 0;			// number of real characters added (not including dots)
	 int bytes_used = 0;		// number of bytes used in buff (including dots)
	 int in_whitespace = 1;		// since start is whitespace, skip leading spaces

	 while (*src != '\0') {
	 	// activate if current character is space or tab
	 	if (*src == ' ' || *src == '\t') {
			// space is added if not in a whitespace block
			if (!in_whitespace) {
				if (bytes_used >= len) return -1;
				*dest++ = ' ';
				bytes_used++;
				real_len++;
				in_whitespace = 1;
			}
		} else {
			// non-whitespace character
			if (bytes_used >= len) return -1;
			*dest++ = *src;
			bytes_used++;
			real_len++;
			in_whitespace = 0;
		}
		src++;
	}
	
	// if string ends in whitespace, remove trailing space
	if (in_whitespace && real_len > 0) {
		dest--;
		bytes_used--;
		real_len--;
	}

	while (bytes_used < len) {
		*dest++ = '.';
		bytes_used++;
	}

	return real_len;
}
	 
void print_buff(char *buff, int len){
    printf("Buffer:  [");
    for (int i=0; i<len; i++){
        putchar(*(buff + i));
    }
    putchar(']');
	 putchar('\n');
}

void usage(char *exename){
    printf("usage: %s [-h|c|r|w|x] \"string\" [other args]\n", exename);

}

int count_words(char *buff, int str_len){
	//YOU MUST IMPLEMENT
	 int count = 0;
	 int in_word = 0;

	 for (int i = 0; i < str_len; i++) {
	 	if (*(buff + i) == ' ') {
			if (in_word) {
				count++;
				in_word = 0;
			}
		} else {
			in_word = 1;
		}
	}
	
	if (in_word) count++;
	return count;
}

//ADD OTHER HELPER FUNCTIONS HERE FOR OTHER REQUIRED PROGRAM OPTIONS

int reverse_string(char *buff, int str_len) {
	int left = 0;
	int right = str_len - 1;
	
	while (buff[right] == '.') right --;
	while (left < right) {
		char tmp = *(buff + left);
		*(buff + left) = *(buff + right);
		*(buff + right) = tmp;
		left++;
		right--;
	}
	return 0;
}

int word_print(char *buff, int str_len) {
	int word_count = 0;
	int in_word = 0;
	int word_start = 0;
	int i = 0;

	printf("Word Print\n----------\n");
	while (i < str_len && *(buff + i) != '.') {
		char c = *(buff + i);
		if (c == ' ') {
			if (in_word){
				int word_len = i - word_start;
				word_count++;
				printf("%d. ", word_count);
				for (int j = word_start; j < i; j++) {
					putchar(*(buff + j));
				}
				printf("(%d)\n", word_len);
				in_word = 0;
			}
		} else {
			if (!in_word) {
				in_word = 1;
				word_start = i;
			}
		}
		i++;
	}
	// If ends while still in a word, print final word
	if (in_word) {
		int word_len = i - word_start;
		word_count++;
		printf("%d. ", word_count);
		for (int j = word_start; j < i; j++) {
			putchar(*(buff + j));
		}
		printf("(%d)\n", word_len);
	}
	printf("\nNumber of words returned: %d\n", word_count);
	return word_count;
}

int main(int argc, char *argv[]){

    char *buff;             //placehoder for the internal buffer
    char *input_string;     //holds the string provided by the user on cmd line
    char opt;               //used to capture user option from cmd line
    int  rc;                //used for return codes
    int  user_str_len;      //length of user supplied string

    //TODO:  #1. WHY IS THIS SAFE, aka what if arv[1] does not exist?
	//		This is safe because the program first checks argc < 2. 
	// 		If this condition is true, argv[1] is never accessed, 
	// 		avoiding out-of-bounds access. The dereferencing of argv[1] 
	// 		happens only when there are at least two arguments provided, 
	// 		ensuring the program doesn't crash due to invalid memory access.
    if ((argc < 2) || (*argv[1] != '-')){
        usage(argv[0]);
        exit(1);
    }

    opt = (char)*(argv[1]+1);   //get the option flag

    //handle the help flag and then exit normally
    if (opt == 'h'){
        usage(argv[0]);
        exit(0);
    }

    //WE NOW WILL HANDLE THE REQUIRED OPERATIONS

    //TODO:  #2 Document the purpose of the if statement below
	//		The purpose of this if statement is to ensure that the program 
	// 		has at least three arguments: the executable name, the option flag,
	//		and the user string. Without the user string (third argument), the 
	//		program cannot proceed and exits after displaying the usage instructions.
    if (argc < 3){
        usage(argv[0]);
        exit(1);
    }

    input_string = argv[2]; //capture the user input string
	
	//TODO:  #3 Allocate space for the buffer using malloc and
    //          handle error if malloc fails by exiting with a 
    //          return code of 99
	 buff = (char*)malloc(BUFFER_SZ);
	 if (buff == NULL) {
	 	fprintf(stderr, "Error: Memory Allocation failed\n");
		exit(2);
	}
   

    user_str_len = setup_buff(buff, input_string, BUFFER_SZ);     
    if (user_str_len < 0){
        printf("Error setting up buffer, error = %d\n", user_str_len);
		  free(buff);
        exit(3);
    }

    switch (opt){
        case 'c':
            rc = count_words(buff, user_str_len);  
            if (rc < 0){
                printf("Error counting words, rc = %d\n", rc);
					 free(buff);
					 exit(3);
            }
            printf("Word Count: %d\n", rc);
            break;

		//TODO:  #5 Implement the other cases for 'r' and 'w' by extending
        //       the case statement options
			case 'r':
				rc = reverse_string(buff, user_str_len);
					if (rc < 0) {
						printf("Error reversing string, rc = %d\n", rc);
						free(buff);
						exit(3);
					}
					break;
			case 'w':
				rc = word_print(buff, user_str_len);
				if (rc < 0) {
					printf("Error printing words, rc = %d\n", rc);
					free(buff);
					exit(3);
				}
				break;
			case 'x':
				if (argc < 5) {
					usage(argv[0]);
					free(buff);
					exit(1);
				}
				printf("Not Implemented!\n");
				free(buff);
				exit(0);
			default:
				usage(argv[0]);
				free(buff);
				exit(1);
	}

    //TODO:  #6 Dont forget to free your buffer before exiting
	print_buff(buff, BUFFER_SZ);
	free(buff);
	exit(0);
}


//TODO:  #7  Notice all of the helper functions provided in the 
//          starter take both the buffer as well as the length.  Why
//          do you think providing both the pointer and the length
//          is a good practice, after all we know from main() that 
//          the buff variable will have exactly 50 bytes?
//  	
//			I think providing both the pointer and the length is good practice because 
//			it decouples the buffer's memory size from its logical content size. 
//			Even though main() knows the buffer is 50 bytes, helper functions shouldn't 
//			make assumptions about the buffer size. Explicitly passing the length ensures 
//			functions operate safely within the defined boundaries, making the code more 
//			modular and robust to changes.