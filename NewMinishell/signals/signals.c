#include "../organize.h"

volatile sig_atomic_t g_sig = 0;
// ─── handlers ────────────────────────────────────────────────────────────────

// Interactive prompt: Ctrl+C → new line, redisplay empty prompt.
// Must NOT call printf/malloc – only async-signal-safe functions.
static void sigint_interactive(int sig)
{
	g_sig = sig;
	write(STDOUT_FILENO, "\n", 1);
	rl_on_new_line();
	rl_replace_line("", 0);
	rl_redisplay();
}

// Heredoc: Ctrl+C → close readline's fd so read_full_input returns NULL.
static void sigint_heredoc(int sig)
{
	g_sig = sig;
	write(STDOUT_FILENO, "\n", 1);
	rl_on_new_line();
	rl_replace_line("", 0);
	// Don't redisplay — heredoc will be cancelled by the NULL return
}

// Child processes inherit default disposition, so the kernel already handles
// SIGINT/SIGQUIT for them. The parent just needs to NOT die itself.
static void sigint_child(int sig)
{
	g_sig = sig;		  // parent records it; child already dead/stopped
	write(STDOUT_FILENO, "\n", 1);
}

static void sigquit_child(int sig)
{
	g_sig = sig;
	write(STDERR_FILENO, "Quit: 3\n", 8);
}

// ─── public setup functions ───────────────────────────────────────────────────

// Call this in the main read-loop (interactive prompt, no child running).
void signals_interactive(void)
{
	struct sigaction sa_int;
	struct sigaction sa_quit;

	sigemptyset(&sa_int.sa_mask);
	sa_int.sa_flags   = SA_RESTART;   // restart rl_readline after signal
	sa_int.sa_handler = sigint_interactive;
	sigaction(SIGINT, &sa_int, NULL);
	sigemptyset(&sa_quit.sa_mask);
	sa_quit.sa_flags   = 0;
	sa_quit.sa_handler = SIG_IGN;	 // Ctrl+\ does nothing at the prompt
	sigaction(SIGQUIT, &sa_quit, NULL);
}

// Call this while waiting for a child process (after fork, before waitpid).
void signals_child(void)
{
	struct sigaction sa_int;
	struct sigaction sa_quit;

	sigemptyset(&sa_int.sa_mask);
	sa_int.sa_flags   = 0;
	sa_int.sa_handler = sigint_child;
	sigaction(SIGINT, &sa_int, NULL);
	sigemptyset(&sa_quit.sa_mask);
	sa_quit.sa_flags   = 0;
	sa_quit.sa_handler = sigquit_child;
	sigaction(SIGQUIT, &sa_quit, NULL);
}

// Call this inside the child process right after fork(), before execve().
// Restore default disposition so the child dies normally on Ctrl+C / Ctrl+\.
void signals_reset_child(void)
{
	signal(SIGINT,  SIG_DFL);
	signal(SIGQUIT, SIG_DFL);
}

// Call this at the start of a heredoc read loop.
void signals_heredoc(void)
{
	struct sigaction sa_int;
	struct sigaction sa_quit;

	sigemptyset(&sa_int.sa_mask);
	sa_int.sa_flags   = 0;
	sa_int.sa_handler = sigint_heredoc;
	sigaction(SIGINT, &sa_int, NULL);
	sigemptyset(&sa_quit.sa_mask);
	sa_quit.sa_flags   = 0;
	sa_quit.sa_handler = SIG_IGN;	 // Ctrl+\ ignored in heredoc too
	sigaction(SIGQUIT, &sa_quit, NULL);
}