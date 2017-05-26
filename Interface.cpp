#include "Interface.h"
#include <cstdlib>
#include <thread>
#include <chrono>
#include <string>
#include <limits>
#include "SaveVisitor.h"
#include "ScanLeftTurnVisitor.h"
#include "PrintVisitor.h"
using namespace std;

void clear_screen()
{
#ifdef _WIN32
	 system("cls");
#else
	// Assume POSIX
	 system("clear");
#endif
}

void press_enter()
{
	cout << "press enter to continue\n";
	cin.ignore(numeric_limits<streamsize>::max(), '\n');
}

int getOneChar(char& c) //if reads one char succesfully changes c to that char else changes c to null
{

	string input;
	getline(cin, input);
	if (cin.fail())
	{
		cin.clear();
		cin.ignore(numeric_limits<streamsize>::max(), '\n');
		return -1;
	}
	if (input.length() != 1)
	{
		c = 0;
		return -1;
	}
	c = input[0];
	return 0;
}

bool isNatural(const string& input)
{
	bool endOfDigits = false;
	for (size_t i = 0; i < input.size(); i++)
	{
		if (!endOfDigits)
		{
			if (!isdigit(input[i]))
			{
				if (!isspace(input[i]))
					return false;
				else
					endOfDigits = true;
			}
		}
		if (endOfDigits)
		{
			if (!isspace(input[i]))
				return false;
		}
	}
	return true;
}

int getOneNatural(size_t& c)
{
	string input;
	getline(cin, input);
	if (cin.fail())
	{
		cin.clear();
		cin.ignore(numeric_limits<streamsize>::max(), '\n');
		return -1;
	}

	if (!isNatural(input))
		return -1;

	c = atoi(input.c_str());

	return 0;
	
}


int Interface::exec() //start menu
{	

	char choice;
	while (true)
	{
		clear_screen();
		 cout << "Welcome in arrival tip editor, what do you want to do?\n"
		<< "1. Create new set of tips\n"
		<< "2. Load existing set from file\n"
		<< "q. Exit application\n\n";
		getOneChar(choice);
		switch (choice)
		{
		case '1': {addMenu(); if(cont.size() != 0) mainMenu(); break; }
		case '2': {load(); if (cont.size() != 0) mainMenu(); break; }
		case 'q': {return 0; break; }
		default: { cout << "wrong input\n"; press_enter(); break; }
		}

	}


}

void Interface::addMenu(size_t index)
{
	index = index ? index : cont.size(); // if index is == 0 i add at the end of vector otherwise add on choosen element
	char choice;
	while (true)
	{
		clear_screen();
		cout << "Adding at: " << index+1  << ". index" << "                              number of tips : " << cont.size() << endl << endl
		<< "Choose kind of tip:\n"
		<< "1. Move forward\n"
		<< "2. Turn\n"
		<< "3. Exit ramp\n"
		<< "4. Destination side\n"
		<< "q. quit\n\n";

		getOneChar(choice);
		switch (choice)
		{
		case '1': {addForward(index); return; break; }
		case '2': {addTurn(index); return; break; }
		case '3': {addExitRamp(index); return; break; }
		case '4': {addDestination(index); return; break; }
		case 'q': {return; break; }
		default: {cout << "wrong input\n"; press_enter(); break; }
		}

	}


}

void Interface::addForward(size_t index)
{
	size_t n = 0;
	while (true)
	{
		clear_screen();
		cout << "How many metres would you like to drive forward?\n"
		<<	"press 0 to cancel\n\n";



		if (getOneNatural(n)) 
			{ cout << "wrong input\n"; press_enter(); continue; }
		else if (n == 0) return;
		else
		{
			if (index == cont.size())
				cont.add(unique_ptr<Tip>(new Forward(n)));
			else
				cont.insert(index, unique_ptr<Tip>(new Forward(n)));
			return;
		}

	}
}

void Interface::addTurn(size_t index)
{
	char choice;
	direction dir;
	bool choosen = 0;
	while (!choosen)
	{
		clear_screen();
		cout << "Choose direction to turn:\n"
		<< "r. right\n"
		<< "l. left\n"
		<< "b. turn back\nq.exit\n\n";

		getOneChar(choice);
		if (!cin) { cout << "wrong input"; press_enter(); continue; }

		switch (choice)
		{
		case 'r': {dir = direction::right; choosen = 1; break; }
		case 'l': {dir = direction::left; choosen = 1; break; }
		case 'b': { dir = direction::back; choosen = 1; break; }
		case 'q': { return; break; }
		default : { cout << "wrong input"; press_enter();  }
		}

	}
	if (index == cont.size())
		cont.add(unique_ptr<Tip>(new Turn(dir)));
	else
		cont.insert(index, unique_ptr<Tip>(new Turn(dir)));
	return;
}

void Interface::addExitRamp(size_t index)
{
	size_t n;
	while (true)
	{
		clear_screen();
		cout << "On which ramp would you like to exit\n?"
		<<	"press 0 to exit\n\n";

		if (getOneNatural(n)) 
			{ cout << "wrong input"; press_enter(); continue; }
		else if (n == 0) return;
		else
		{
			if (index == cont.size())
				cont.add(unique_ptr<Tip>(new ExitRamp(n)));
			else
				cont.insert(index, unique_ptr<Tip>(new ExitRamp(n)));
			return;
		}

	}
}

void Interface::addDestination(size_t index)
{
	char choice;
	direction dir;
	bool choosen = 0;
	while (!choosen)
	{
		clear_screen();
		cout << "Choose direction to turn:\n"
			<< "r. right\n"
			<< "l. left\n"
			<< "q. exit\n\n";


		getOneChar(choice);
		switch (choice)
		{
		case 'r': {dir = direction::right; choosen = 1; break; }
		case 'l': {dir = direction::left; choosen = 1; break; }
		case 'q': { return; break; }
		default: { cout << "wrong input"; press_enter(); break; }

		}

	}
	if (index == cont.size())
		cont.add(unique_ptr<Tip>(new Destination(dir)));
	else
		cont.insert(index, unique_ptr<Tip>(new Destination(dir)));
	return;
}

void Interface::input()
{
	size_t index;

	while (1)
	{
		clear_screen();
		cout << "Choose index to input tip:\n0 to exit\n\n";
		show();
		if (getOneNatural(index) || index > cont.size()+1) { cout << "wrong input\n"; press_enter(); continue; }
		else if (index == 0) return;
		else
			break;
	}
	addMenu(index-1);
	return;
}

void Interface::scanLeftTurns()
{
	ScanLeftTurnsVisitor visitor;
	cont.visitAll(visitor);
	if (visitor.getFlag() == true)
		cout << "Left turns were found!!!\n\n";
	else
		cout << "No left turns were found ;)\n";
	press_enter();
}

void Interface::remove()
{
	size_t index;

	while (1)
	{
		clear_screen();
		cout << "Choose index to remove:\npress 0 to exit\n\n";
		show();

		if (getOneNatural(index) || index > cont.size()) { cout << "wrong input\n"; press_enter(); continue; }
		else if (index == 0) return;
		else
			break;
	}
	cont.remove(index-1);
	return;
}


void Interface::show()
{
	PrintVisitor vis(std::cout);
	cout << "Your set of tips:\n";
	for (size_t i = 0; i < cont.size(); i++)
	{
		cout << i+1 << ". ";
		cont[i].Accept(vis);
		cout << endl;
	}
	cout << endl;
}

void Interface::save()
{
	string fileName;

	clear_screen();
	cout << "Enter file name:\n\n";
	getline(cin, fileName);

	SaveVisitor vis(fileName.c_str());

	cont.visitAll(vis);

}

void Interface::load()
{
	while (true)
	{
		string fileName;

		clear_screen();

		cout << "Enter file name:\npress enter to exit\n\n";

		getline(cin, fileName);


		if (!loadTips(cont, fileName.c_str()) || fileName == "") return;

		cout << "error while reading(file may not exist or it's type is wrong)\n";
		press_enter();

	}
	///////tu


}

void Interface::mainMenu()
{
	char choice;
	while (true)
	{
		clear_screen();
		cout << "MAIN MENU\n"
			<< "choose option\n"
			<< "a. Add new tip\n"
			<< "r. Remove tip\n"
			<< "i. Insert tip on choosen index\n"
			<< "s. Show all tips\n"
			<< "l. Seek left turns\n\n"
			<< "w. Write set to file\n"
			<< "q. Close set\n\n";
		getOneChar(choice);
		switch (choice)
		{
		case 'a': {addMenu(); break; }
		case 'r': {remove(); break; }
		case 'i': {input(); break; }
		case 's': {clear_screen();  show(); press_enter(); break; }
		case 'l': { scanLeftTurns(); break; }
		case 'w': { save(); break; }
		case 'q': { cont.clear(); return; break; }
		default: { cout << "wrong input\n"; press_enter(); break; }
		}

	}
}
