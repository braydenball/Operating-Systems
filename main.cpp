#include <atomic>
#include <iostream>
#include <csignal>
#include <cstdlib>
#include <cstring>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
//The author acknowledges the use of ChatGPT in the preparation of this assignment for brainstorming and citation.
std::atomic_int signal_terminate(0);
std::atomic_int counter(0);
std::atomic_int sleep_value(1);

// Signal handler function for SIGINT
void handleSIGINT(int signal)
{
    // TODO:
    // - Report the receipt of interrupt signal
    // - Report the number of forks counter
    // - Set signal_terminate to one
	std::cout << "Received SIGINT signal." << std::endl;
	std::cout << "Number of fork: " << counter.load() << std::endl;
	signal_terminate.store(1);
}

// Signal handler function for SIGUSR1
void handleSIGUSR1(int signal)
{
    // TODO:
    // - Report the receipt of user signal 1
    // - Report the number of forks counter
    // - Add one to sleep_value
    // - Report the increased thread sleep value
	std::cout << "Received SIGUSR1." << std::endl;
	std::cout << "Number of forks: " << counter.load() << std::endl;
	sleep_value.fetch_add(1);
	std::cout << "Increased thread sleep value: " << sleep_value.load() << std::endl;
}

// Signal handler function for SIGALRM
void handleSIGALRM(int signal)
{
    // TODO: 
    // - Report the receipt of the alarm signal
    // - Subtract one from sleep_value
    // - Report the decreased thread sleep value
	std::cout << "Received SIGALRM signal." << std::endl;
	sleep_value.fetch_sub(1);
	std::cout << "Decreased thread sleep value: " << sleep_value.load() << std::endl;
}

int main()
{
    auto pid = getpid();

    // Register signal handlers
    signal(SIGINT, handleSIGINT);
    signal(SIGUSR1, handleSIGUSR1);
    signal(SIGALRM, handleSIGALRM);

    // Main program logic
    std::cout << "(Processs ID: " << pid << "). Running the program. Press Ctrl+C to send SIGINT." << std::endl;

    // Simulating a long-running process

    while (signal_terminate.load() != 1)
    {
        pid_t pid = fork();

        if (pid < 0)
        {
            std::cerr << "Failed to fork a child process." << std::endl;
            exit(EXIT_FAILURE);
        }
        else if (pid == 0)
        {
            // Child process
            // TODO:
            // - Report the child process identification number using getpid()
	    // - Have the child process sleep for X seconds. X = sleep_value
            // - Terminate child process using exit()
		std::cout << "Child process ID: " << getpid() << std::endl;
		sleep(sleep_value.load());
		exit(EXIT_SUCCESS);
        }
        else
        {
            // Parent process
            int status;
            waitpid(pid, &status, 0);
            std::cout << "Child process " << pid << " has exited with status: " << WEXITSTATUS(status) << std::endl;
        }

        counter.fetch_add(1);
    }

    return 0;
}

