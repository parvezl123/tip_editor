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

void Interface::press_enter()
{
	cout << "press enter to continue\n";
	inputs.ignore(numeric_limits<streamsize>::max(), '\n');
}

char Interface::getOneChar() //if reads one char succesfully changes c to that char else changes c to null
{

	string input;
	getline(inputs, input);
	if (inputs.fail())
	{
		inputs.clear();
		inputs.ignore(numeric_limits<streamsize>::max(), '\n');
		return 0;
	}
	if (input.length() != 1)
	{
		return 0;
	}
	return input[0];

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

int Interface::putOneNatural(size_t& c)
{
	string input;
	getline(inputs, input);
	if (inputs.fail())
	{
		inputs.clear();
		inputs.ignore(numeric_limits<streamsize>::max(), '\n');
		return -1;
	}

	if (!isNatural(input))
		return -1;

	c = atoi(input.c_str());

	return 0;
	
}


int Interface::exec() //start menu
{	

	while (true)
	{
		if (exit)
		{	
	//		clear_screen();
			return 1;
		}
		clear_screen();
		 cout << "Welcome in arrival tip editor, what do you want to do?\n"
		<< "1. Create new set of tips\n"
		<< "2. Load existing set from file\n"
		<< "q. Exit application\n\n";
		switch (getOneChar())
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
	index = index ? index-1 : cont.size(); // if index is == 0 i add at the end of vector otherwise add on choosen element

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

		switch (getOneChar())
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



		if (putOneNatural(n)) 
			{ cout << "wrong input\n"; press_enter(); continue; }
		else if (n == 0) return;
		else if (n == std::numeric_limits<std::size_t>::max()) 
		{
			cout << "that number is very big, please enter smaller one\n";
			press_enter();
		}
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
	direction dir;
	bool choosen = 0;
	while (!choosen)
	{
		clear_screen();
		cout << "Choose direction to turn:\n"
		<< "r. right\n"
		<< "l. left\n"
		<< "b. turn back\nq.exit\n\n";



		switch (getOneChar())
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
		cout << "On which ramp would you like to exit?\n"
		<<	"press 0 to exit\n\n";

		if (putOneNatural(n)) 
			{ cout << "wrong input\n"; press_enter(); continue; }
		else if (n == 0) return;
		else if (n == std::numeric_limits<std::size_t>::max())
		{
			cout << "that number is very big, please enter smaller one\n";
			press_enter();
		}
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
	direction dir;
	bool choosen = 0;
	while (!choosen)
	{
		clear_screen();
		cout << "Choose direction to turn:\n"
			<< "r. right\n"
			<< "l. left\n"
			<< "q. exit\n\n";



		switch (getOneChar())
		{
		case 'r': {dir = direction::right; choosen = 1; break; }
		case 'l': {dir = direction::left; choosen = 1; break; }
		case 'q': { return; break; }
		default: { cout << "wrong input\n"; press_enter(); break; }

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
		if (putOneNatural(index) || index > cont.size()+1) { cout << "wrong input\n"; press_enter(); continue; }
		else if (index == 0) return;
		else
			break;
	}
	addMenu(index);
	return;
}

void Interface::scanLeftTurns()
{
	ScanLeftTurnsVisitor visitor;


	cont.visitAll(visitor);


	if (visitor.getFlag() == true)
		cout << visitor.getAmount() << " left turns found!!!\n\n";
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

		if (putOneNatural(index) || index > cont.size()) { cout << "wrong input\n"; press_enter(); continue; }
		else if (index == 0) return;
		else
			break;
	}
	cont.remove(index-1);
	return;
}


void Interface::show()
{
	PrintVisitor vis(cout);
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
	getline(inputs, fileName);

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

		getline(inputs, fileName);


		if (!loadTips(cont, fileName.c_str()) || fileName == "") return;

		cout << "error while reading(file may not exist or it's type is wrong)\n";
		press_enter();

	}


}

void Interface::mainMenu()
{
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
			<< "w. Write set to file\n\n"
			<< "q. Close set\n"
			<< "e. Exit intefrace\n\n";

		switch (getOneChar())
		{
		case 'a': {addMenu(); break; }
		case 'r': {remove(); break; }
		case 'i': {input(); break; }
		case 's': {clear_screen();  show(); press_enter(); break; }
		case 'l': {scanLeftTurns(); break; }
		case 'w': {save(); break; }
		case 'e': {exit = true; return; }
		case 'q': {cont.clear(); return; break; }
		default: { cout << "wrong input\n"; press_enter(); break; }
		}

	}
}


