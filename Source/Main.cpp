#include <stdlib.h>
#include "Application.h"
#include "Globals.h"

#include "SDL.h"

enum main_states
{
	MAIN_CREATION,
	MAIN_START,
	MAIN_UPDATE,
	MAIN_FINISH,
	MAIN_EXIT
};

int main(int argc, char ** argv)
{
	ConsoleBuffer* consoleBuff = new ConsoleBuffer;
	consoleBuff->initBuff1 = LOG("Starting Engine...");

	int main_return = EXIT_FAILURE;
	main_states state = MAIN_CREATION;
	Application* App = NULL;

	while (state != MAIN_EXIT)
	{
		switch (state)
		{
		case MAIN_CREATION:

			consoleBuff->initBuff2 = LOG("-------------- Application Creation --------------");
			App = new Application(consoleBuff);
			state = MAIN_START;
			break;

		case MAIN_START:

			if (App != nullptr)
			{
				if (App->gui != nullptr)
				{
					App->gui->LogConsole(LOG("-------------- Application Init --------------"));
				}
			}
			if (App->Init() == false)
			{
				if (App->gui != nullptr)
				{
					App->gui->LogConsole(LOG("Application Init exits with ERROR"));
				}
				state = MAIN_EXIT;
			}
			else
			{
				state = MAIN_UPDATE;
				if (App->gui != nullptr)
				{
					App->gui->LogConsole(LOG("-------------- Application Update --------------"));
				}
			}

			break;

		case MAIN_UPDATE:
		{
			int update_return = App->Update();

			if (update_return == UPDATE_ERROR)
			{
				App->gui->LogConsole(LOG("Application Update exits with ERROR"));
				state = MAIN_EXIT;
			}

			if (update_return == UPDATE_STOP)
				state = MAIN_FINISH;
		}
			break;

		case MAIN_FINISH:

			App->gui->LogConsole(LOG("-------------- Application CleanUp --------------"));
			if (App->CleanUp() == false)
			{
				App->gui->LogConsole(LOG("Application CleanUp exits with ERROR"));
			}
			else
				main_return = EXIT_SUCCESS;

			state = MAIN_EXIT;

			break;

		}
	}

	delete App;
	LOG("Exiting engine...\n");
	return main_return;
}