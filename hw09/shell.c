#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <fcntl.h>
#include <limits.h>
#include <signal.h>
#include <stddef.h>

#define MAX_CMD 1024
#define MAX_PROC 256
#define MAX_TOK 100

// fields
/* command types for readability */
typedef enum {EXIT, MY_INFO, CD, GET_ENV, SET_ENV, NONE} cmdtype;
/* the ID of the shell process for singling it out */
pid_t shell_id;
/* to tell if sigint */
int sigint;
/* to tell if the command was called with '&' */
int daemon;
/* pids for all background processes */
pid_t *children;
/* keep track of # of background processes */
int num_children = 0;



void displaydir(char *buf);
/*
 * Function meant to handle SIGINT
 * Kills all children, preserves shell process
 */
void siginthandler(int sig) {
    // to avoid gcc warnings
    sig += 1;

    // kill children
    if (getpid() != shell_id) {
	       char *cwd = malloc(sizeof(char) * 256);
	       displaydir(cwd);
	       free(cwd);
	       exit(0);
    }
    //char *cwd = malloc(sizeof(char) * 256);
    printf("\n");
    //displaydir(cwd);
    //free(cwd);
    sigint = 1;
}



/*
 * Sets up a handler to catch sigints
 */
void setup_sighandler(void) {
    struct sigaction setup_action;
    sigset_t block_mask;

    // gonna black dem other signals
    sigemptyset(&block_mask);
    sigaddset(&block_mask, SIGINT);
    sigaddset(&block_mask, SIGQUIT);

    // set up the signal handler
    setup_action.sa_handler = siginthandler;
    setup_action.sa_mask = block_mask;
    setup_action.sa_flags = 0;

    // deploy listener
    sigaction(SIGINT, &setup_action, NULL);
}



/*
 * Formats a token to cut out new lines left in by strtok
 */
char *formattoken(char *tok) {
    // the length if '\n' is cut off
    size_t real_len = strcspn(tok, "\n");
    // if we need to cut off a '\n'
    if (strlen(tok) > real_len)
	   tok[(int) real_len] = '\0';
    return tok;
}


/*
 * Made for parsing simple built-in integrity
 */
cmdtype parse_singleword(char *cmd) {
    // no switch statements for strings :(
    if (!strcmp(cmd, "exit"))
	   return EXIT;
    else if (!strcmp(cmd, "myinfo"))
	   return MY_INFO;
    else if (!strcmp(cmd, "cd"))
	   return CD;
    else if (!strcmp(cmd, "getenv"))
	   return GET_ENV;
    else if (!strcmp(cmd, "setenv"))
	   return SET_ENV;
    else
	   return NONE;
}



/*
 * Clears the args for the next command call
 */
void nullify_args(char **args, int len) {
    for (int i = 0; i < len; i++)
	   args[i] = NULL;
}



/*
 * Remove a dead child from the background processes list
 * Doesn't work lol
 */
void removechild(pid_t dedkid) {
    for (int i = 0; i < num_children; i++)
	if (children[i] == dedkid) {
	    for (int j = i; j < num_children - 1; j++) {
		  children[j] = children[j + 1];
	    }
	    break;
	}
    children[--num_children] = 0;
}




/*
 * Checks if any zombie processes exist and reaps them
 */
void check_zombies(void) {
    int status;
    if (getpid() == shell_id) {
		pid_t killed = waitpid(-1, &status, WNOHANG);
	if (killed > 1)
	    removechild(killed);
    }
}



/*
 * Kills all children for on exit
 */
void kill_zombies(void) {
    // debugging
    /* printf("Child processes: \n");
    for (int i = 0; i < num_children; i++)
	printf("%d\n", children[i]); */

    for (int i = 0; i < num_children; i++) {
	       int status = kill(children[i], SIGKILL);
	          if (status == -1)
	             printf("Couldn't kill process %d\n", children[i]);
    }
}



/*
 * Makes a pretty display for the prompt
 */
void displaydir(char *buf) {
    getcwd(buf, MAX_CMD);
    // find where $HOME ends in the cwd
    char *home = getenv("HOME");
    size_t homelen = strlen(home);
    size_t cwdlen = strlen(buf);
    if (cwdlen >= homelen) {
	// cut off the home dir
	   buf = &buf[homelen - 1];
	   buf[0] = '~';
    }
    strcat(buf, "$ ");
    printf("%s", buf);
}



/*
 * Built in function for cd
 */
void cd(char *dir) {
    // debugging
    /* printf("cd was called\n"); */

    // if call is just cd with no dir
    if (dir == NULL) {
        // debugging
	/* printf("no dir\n"); */

	// dir = $HOME
	   dir = getenv("HOME");
    }

    // cd <dir>
    int error = chdir(formattoken(dir));

    if (error)
	   printf("cd: directory not found\n");
}



/*
 * Built in function for getenv
 */
void get_env(char *var) {
    printf("%s\n", getenv(var));
}



/*
 * Built in function for setenv
 */
void set_env(char *var, char *val, int overwrite) {
    int error = setenv(var, val, overwrite);
    if (error)
	   printf("setenv: couldn't set %s to %s\n", var, val);
}



/*
 * Calls a command on a child thread
 */
void call_command(char *cmd, char **args) {
    // this is probably unnecessary
    if (getpid() == shell_id)
	   printf("Cannot exec over shell");

    // if no args are given
    if (args[0] == NULL) {
	// debugging
	// printf("args[0] = NULL\n");
	   args[0] = "";
    }

    // for debugging
    /*printf("args: \n");
    int i = 0;
    char *arg = args[0];
    while (arg != NULL) {
	printf("%s\n", arg);
	arg = args[++i];
    } */

    // call the command with the arguments
    int error = execvp(cmd, args);
    if (error)
	   printf("-shell: command \'%s\' not found\n", cmd);
}



/*
 * Emulates a shell
 */
int main() {

    // for catching SIGINT
    setup_sighandler();

    // for determining which process is the parent
    shell_id = getpid();

    // exact input from user
    char *input = malloc(MAX_CMD);
    // tokenized args from input
    char **args = malloc(sizeof(char *) * MAX_TOK);
    // for storing tokens
    char *token;
    // current working directory
    char *cwd = malloc(MAX_CMD);
    children = malloc(sizeof(pid_t) * MAX_PROC);

    // just collect commands forever
    while (1) {
	sigint = 0;
	daemon = 0;

	check_zombies();

	// print working directory (just like the real thing!)
	if (getpid() == shell_id) {
	    cwd = realloc(cwd, MAX_CMD);
	    displaydir(cwd);
	}

	// store command in input
	fgets(input, MAX_CMD, stdin);

	// if input is SIGINT
	if (sigint)
	    continue;

	// index of args
	int i = 0;
	// parse the first arg
	token = strtok(input, " ");

	// fill args array
	while (token != NULL) {
	    args[i++] = token;
	    token = strtok(NULL, " ");
	}

	// check for built-in single-word commands
	cmdtype cmd = parse_singleword(formattoken(args[0]));

	// check for daemon calls
	if (!strcmp(args[i - 1], "&\n")) {
	    daemon = 1;
	    args[i - 1] = NULL;
	    i--;
	}

	// built in command for exit
	if (cmd == EXIT) {
	    kill_zombies();
	    break;
	}

	// built in command for cd
	if (cmd == CD && getpid() == shell_id)
	    cd(args[1]);

	// create child process for command execution
	pid_t fork_status = fork();

	// get pids for this process group
	pid_t pid = getpid();

	// wait patiently and politely until the children are dead
	// waitpid didn't work properly for some reason
	if (pid == shell_id) {
	    int status;
	    if (!daemon)
		wait(&status);

	    // keep track of background procs
	    if (daemon) {
		// debugging
		// printf("daemon process logged\n");

		if (num_children < MAX_PROC)
		    children[num_children++] = fork_status;
		else {
		    printf("-shell: cannot execute more than %d processes\n", MAX_PROC);
		    exit(1);
		}
	    }
	}


	// if fork error
	if (fork_status == -1) {
	    printf("-shell: Couldn't fork subprocess\n");
	    exit(1);
	}

	// built in command for myinfo
	if (cmd == MY_INFO && pid != shell_id) {
	    printf("PID:  %d\n", pid);
	    printf("PPID: %d\n", getppid());
	}

	// built in command for getenv
	if (cmd == GET_ENV && pid != shell_id) {
	    get_env(formattoken(args[1]));
	}

	// built in command for setenv
	if (cmd == SET_ENV && pid != shell_id) {
	    int overwrite = args[3] != NULL ? atoi(formattoken(args[3])) : 0;
	    set_env(formattoken(args[1]), formattoken(args[2]), overwrite);
	}

	// get rid of the \n on the last arg (ugh)
	if (i > 0) // SIGINT could mess this up
	    args[i - 1] = formattoken(args[i - 1]);

	// handle all other standard shell commands
	if (cmd == NONE && pid != shell_id)
	    call_command(formattoken(args[0]), &args[0]);

	// have this last for killing children
	if (pid != shell_id)
	    exit(0);

	// clear args
	nullify_args(args, i);
    }

    free(args);
    free(input);
    free(cwd);
    free(children);

    // in case this is reached from a child
    exit(0);
}
