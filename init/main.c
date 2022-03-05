#include <stdio.h>
#include <sys/types.h>

#include <stdlib.h>
#include "readline/readline.h"
#include "readline/history.h"

extern HIST_ENTRY **history_list ();

int main (int argc, char *argv[])
{
	char *temp, *prompt;
	int done;

	temp = (char *)NULL;
	prompt = "llos$ ";
	done = 0;

	while (!done)
	{
		temp = readline (prompt);
		/* Test for EOF. */
		if (!temp)
			exit (1);
		/* If there is anything on the line, print it and remember it. */
		if (*temp)
		{
			fprintf (stderr, "%s\r\n", temp);
			add_history (temp);
		}
		/* Check for `command' that we handle. */
		if (strcmp (temp, "quit") == 0)
			done = 1;

		if (strcmp (temp, "list") == 0)
		{
			HIST_ENTRY **list;
			register int i;
			list = history_list ();
			if (list)
			{
				for (i = 0; list[i]; i++)
				fprintf (stderr, "%d: %s\r\n", i, list[i]->line);
			}
		}
		free (temp);
	}
	return 0;
}

