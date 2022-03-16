#include"Sheet.cpp"
#include<conio.h>
//klasa do nawigacji po arkuszu
class Navi
{
private:
	int bColor = 0;
	int width=0, height=0, exit = 0;
	bool inColorPalette = false;
	bool text = false;
	bool loop = true;
	Sheet* sheet;
	//handler konsoli
	HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
	CONSOLE_CURSOR_INFO cursor;
public:
	int x;
	int y;
	enum class DIR { LEFT, RIGHT, UP, DOWN };//kierunki zaznaczenia komorki(chcialem sie pobawic enumem bo nigdy go nie rozumialem :) )
	Navi(Sheet* sheet)//konstruktor
	{
		this->sheet = sheet;
		x = 0;
		y = 0;
	}
	//gettery
	int getX(void) { return x; }
	int getY(void) { return y; }
	//setter
	void move(DIR _where) // przesuwamy sie o 1 komorke w kierunku:
	{
		switch (_where)
		{
			case DIR::LEFT:
				if (x != 0)//nie przekraczamy granic
				{
					x--;
				}
				break;
			case DIR::RIGHT:
				if (x != sheet->getColWidths().size()-1)//nie przekraczamy granic
				{
					x++;
				}
				break;
			case DIR::UP:
				if (y != 0)
				{
					y--;
				}
				break;
			case DIR::DOWN:
				if (y != sheet->getRowHeights().size()-1)
				{
					y++;
				}
				break;
			default:
				break;

		}
		try
		{
			sheet->highlightCell(x, y);//podswietlamy aktualnie wybrana komorke...
		}
		catch (std::exception) {}//...chyba ze nie istnieje 
		if (sheet->getSheet()[y][x].getFormula().size() == 0)
		{
			showFormulaBar(sheet->getSheet()[y][x].getText());//pasek funkcyjny u dolu arkusza wyswietla zawartosc komorki...
		}
		else
		{
			showFormulaBar(sheet->getSheet()[y][x].getFormula());//..lub jej formule
		}
	}
	void showFormulaBar(std::string input)
	{
		SetConsoleTextAttribute(hConsole, 0xF0);//zmieniamy tlo na biale 
		CONSOLE_SCREEN_BUFFER_INFO csbi;
		GetConsoleScreenBufferInfo(hConsole, &csbi);
		for (int i = 0; i < csbi.srWindow.Right - csbi.srWindow.Left - input.size(); i++)//rysujemy bialy pasek o dlugosci okna
		{
			std::cout << " ";
		}
		std::cout << "\r" << input << "\r";//cofamy sie do poczatku linii i wypisujemy input
	}
	void showColorPalette()//paleta dostepnych kolorow wyswietlana pod paskiem funkcyjnym
	{
		SetConsoleTextAttribute(hConsole, 0x0F);
		CONSOLE_SCREEN_BUFFER_INFO csbi;
		GetConsoleScreenBufferInfo(hConsole, &csbi);
		std::cout<<std::endl;
		for (int i = 0; i < 16; i++)
		{
			SetConsoleTextAttribute(hConsole, i*16 + 15);
			std::cout << "  ";
		}
		inColorPalette = true;//zmiana zachowania klawiszy strzalek
	}
	int naviSheet()
	{
		while (loop)//petla sterowania klawiszami
		{
			if (_kbhit())//jezeli wcisnieto przycisk
			{
				int key = _getch();//jaki to przycisk
				std::string input = "";
				std::stringstream path;
				switch (key)
				{
				case 72://strzalka w gore
					if(!inColorPalette)move(Navi::DIR::UP);
					break;
				case 77://strzalka w prawo
					if (!inColorPalette) { move(Navi::DIR::RIGHT); } //zmiana komorki 
					else //zmiana koloru
					{ 
						if (bColor <= 14)
						{
							bColor++;
							if (text) sheet->setCellTextColor(x, y, bColor);
							else sheet->setCellBackgroundColor(x, y, bColor);
							sheet->draw();
						}
					}
					break;
				case 80://strzalka w dol
					if(!inColorPalette)move(Navi::DIR::DOWN);
					break;
				case 75://strzalka w lewo
					if (!inColorPalette) { move(Navi::DIR::LEFT); }
					else
					{
						if (bColor >= 1)
						{
							bColor--;
							if(text) sheet->setCellTextColor(x, y, bColor);
							else sheet->setCellBackgroundColor(x, y, bColor);
							sheet->draw();
						}
					}
					break;
				case 13://enter
					if (!inColorPalette)
					{
						GetConsoleCursorInfo(hConsole, &cursor);
						cursor.bVisible = true;
						SetConsoleCursorInfo(hConsole, &cursor);
						showFormulaBar("");
						std::getline(std::cin, input);
						sheet->insert(x, y, input);
						sheet->draw();
					}
					else
					{
						inColorPalette = false;
						sheet->clearConsole();
						sheet->draw();
					}
					break;
				case 99://klawisz 'C'
					if (inColorPalette)
					{
						text = !text;//zmiana koloou tekstu/tla
					}
					else
					{
						showColorPalette();
					}
					break;
				case 27://esc
					inColorPalette = false;
					sheet->clearConsole();
					sheet->draw();
					break;
				case 15://Ctrl+O
					loop = false;
					exit = 0;//kod wyjsciowy
					break;
				case 17://ctrl+Q
					loop = false;
					exit = -1;
					break;
				case 19://ctrl+s
					GetConsoleCursorInfo(hConsole, &cursor);
					cursor.bVisible = true;
					SetConsoleCursorInfo(hConsole, &cursor);
					showFormulaBar("Enter file name: \n");
					std::getline(std::cin, input);
					path.str(std::string());
					path << input << ".pienko";
					sheet->saveToFile(path.str());
					break;
				case 14://ctrl+n
					loop = false;
					exit = 1;
				case 115://ctrl+strzalka w lewo
					width = sheet->getColWidths()[x];
					if (width >4) { sheet->setColWidth(x, width - 2); }
					else if(width==4){ sheet->setColWidth(x, width - 1); }//minimalna dlugosc komorki to 3(granica poprawnego odczytu dlugosci)
					sheet->clearConsole();
					sheet->draw();
					break;
				case 116://ctrl+strzalka w prawo
					width = sheet->getColWidths()[x];
					sheet->setColWidth(x, width + 2);
					sheet->clearConsole();
					sheet->draw();
					break;
				default:
					break;
				}

				GetConsoleCursorInfo(hConsole, &cursor);
				cursor.bVisible = false;
				SetConsoleCursorInfo(hConsole, &cursor);
				//std::cout << std::endl << key;
			}

		}
		return exit;
	}
};