// The Following are our next steps for the minishell we should immediately begin with efter finishing the youtube playlist

// IMPORTANT NOTE:
//  I AM NOT QUITE SURE ABOUT THE FUNCTIONS RETURNED DATA TYPES AS WE MIGHT CHANGE THEM ACCORDINGLY

void    is_simple_exe(char *line)
{
	// checking for pipelines and/or redirections here.
	// if none were found then the line should be executed in a simple execution (One line only)
}

void    are_we_gonna_expand(char *line)
{
	// this function is meant to be for checking the input in order to decide wheather there is an expansion
	// or not, like (echo "'$PATH'", echo '"$PATH"', echo "'"$PATH'"') study it more carefully
}

void    redirection_setup(char *line)
{
	//  If the command includes:
	//  1.    > (output redirection)
	//  2.    < (input redirection)
	//  3.    >> (append)
	//  4.    2> (stderr redirection)
	// The shell prepares file descriptors before running the command
}

void    is_built_in(char *cmd)
{
	// The shell checks:
	// Is it a built-in?
	//      Example: cd, exit, export
	//      Executed directly inside the shell process.
	// If not built-in:
	//      The shell searches directories in $PATH
	//      Finds executable file like->(/bin/ls)
}


// The following:
// For external commands:
// The shell calls fork()
// Creates a child process.
// The child process calls exec()
// Replaces itself with the requested program.
// The parent shell:
// Waits for the child to finish (unless background &).

// Then:
// The program runs.
// It produces:
// Standard output (stdout)
// Standard error (stderr)

// After that:
// If no redirection -> output goes to terminal.
// If redirected -> goes to file or pipe.
// If piped (|) -> output becomes input of next command.
// Example:
// ls | grep txt
// ls output -> pipe -> grep

// Finally:
// When the program finishes, it returns an exit code.
// 0 -> success
// Non-zero > error
// Stored in $?

// Then loop back

//----------------------------------------------------------
// Input
//   |
// Tokenize
//   |
// Parse
//   |
// Expand
//   |
// Redirection setup
//   |
// Built-in? -> Yes -> Execute
//           |> No -> fork() -> exec()
//   |
// Wait & Collect Exit Status
//   |
// Display Output
//   |
// Prompt Again


int main()
{


	return 0;
}



// ls -la -m | grep "txt" > output.txt
// cmd1 -> ls -la -m  // process1
// cmd2 -> grep "txt" // process2
// cmd3 -> output.txt // process3
// TO do LIST :

// 1.  Has complex function: checks if the command is simple or complex (pipes, redirections, etc.) in order to decide how to execute it
// 1.1 If simple -> organize() -> execute_command()
// 1.2 If complex -> are_we_gonna_split() -> split_into_processes() -> parse_to_cmds()
// 1.3 the final shape of the tokens should be a list of char*** (array of commands, each command is an array of strings) in order to be ready for the execution phase
// 2. apply redirections if existed
// 3. 