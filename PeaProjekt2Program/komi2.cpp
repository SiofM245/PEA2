

#include "pch.h"
#include "Menu.h"

int main()
{
	srand(time(NULL));
	Menu *menu = new Menu();
	bool end = false;

	do {
		menu->showMenu();
		end = menu->wyborOpcji();
	} while (!end);
}

