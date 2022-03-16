#include <iostream>
#include <sstream>
#include <string>
#include<vector>
#include<windows.h>
#include"Navi.cpp"
int main()
{
	//inicjalizacja przed switchem
	std::string in;
	std::stringstream path;
	std::ifstream test;
	int width, height;
	bool on = true;
	//rozpoczecie programu od powitalnego arkusza
	Sheet* sheet = new Sheet(1);
	sheet->openFromFile("welcome_sheet.pienko");
	Navi navi = Navi(sheet);
	sheet->clearConsole();
	sheet->draw();
	while (on)
	{
		//petla zmiany/tworzenia arkusza
		switch (navi.naviSheet())
		{
		case 0://otwieranie istniejacego arkusza
			path.str(std::string()); //reset stringstream
			std::cout << "Enter file name: ";
			std::getline(std::cin, in);
			path << in << ".pienko";//ladujemy plik
			sheet = new Sheet(0);
			//sprawdzamy czy plik istnieje przed utworzeniem z niego arkusza
			test.open(path.str());
			if (test.good())
			{
				sheet->openFromFile(path.str());
				navi = Navi(sheet);
			}
			else { std::cout << "Could not open file."; }
			test.close();
			break;
		case 1://nowy arkusz
			std::cout << "Width: ";
			std::cin >> width;
			std::cout << "Height: ";
			std::cin >> height;
			if (std::cin.good())//sprawdzenie poprawnosci wejscia
			{
				sheet = new Sheet(0, width, height);
				navi = Navi(sheet);
			}
			else 
			{
				std::cin.clear();
				navi = Navi(sheet);
			}
			break;
		case -1://wyjscie
			on = false;
			break;
		default:
			break;
		}
		sheet->clearConsole();
		sheet->draw();
	}
}

