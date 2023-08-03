#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>

int main()
{
	printf("Content-type: text/html\r\n");
	printf("Status: 418\r\n\r\n");
	printf("Hello from Fucked CGI!\n");
	printf("PID is: %d\n", getpid());
	fflush(stdout);

	close(STDOUT_FILENO);

	dprintf(STDERR_FILENO, "fucked-cgi closed stdout\n");

}