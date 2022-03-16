#include <iostream>
#include <sstream>
#include <string>
#include<vector>
#include<fstream>
#include<algorithm>
#include<cmath>
#include<map>
#include<windows.h>
#include"Cell.cpp"
#include"tinyexpr.h"
class Sheet
{
private:
	int id=0;
	int length = 0;
	int gridColor = 15;
	int highlight[4] = { 0, 0, 15, 0 };//przechowanie wartosci do podswietlenia komorki na arkuszu [x, y, kolorTekstu, kolorTla]
	std::vector<int> colWidth;
	std::vector<int> rowHeight;
	std::vector<std::vector<Cell>> sheet;
	std::string alphabet = "ABCDEFGHIJKLMNOPRSTUVWXYZ";//indeksowanie kolumn arkusza literami alfabetu
public:
	//konstruktory
	Sheet() {}
	Sheet(int id) { this->id = id; }
	Sheet(int id, std::vector<std::vector<Cell>> sheet) { this->id = id; this->sheet = sheet; }
	Sheet(int id, int width, int height) //pusty arkusz
	{
		std::vector<std::vector<Cell>> map;
		std::vector<Cell>row;
		for (int j = 0; j < width; j++)
		{
			row.clear();
			for (int i = 0; i < height; i++)
			{

				row.push_back(Cell(id));//dodawanie komorki do rzedu
				id++;
			}
			map.push_back(row);//dodawanie rzedu arkusza
		}
		for (int i = 0; i < height; i++)
		{
			colWidth.push_back(13);
		}
		for (int j = 0; j < width; j++)
		{
			rowHeight.push_back(1);
		}
		this->sheet = map;
	}
	//gettery
	std::vector<int> getColWidths() { return colWidth; }
	std::vector<int> getRowHeights() { return rowHeight; }
	std::vector<std::vector<Cell>> getSheet() { return this->sheet; }
	int getSheetLength() { return this->length; }
	//settery
	void setSheet(std::vector<std::vector<Cell>> sheet) { this->sheet = sheet; }
	void setColWidth(int col, int width) { this->colWidth[col] = width; }
	void setRowHeight(int row, int height) { this->rowHeight[row] = height; }
	void setCellTextColor(int x, int y, int color) { this->sheet[y][x].setTextColor(color); highlight[2] = color; }
	void setCellBackgroundColor(int x, int y, int color) { this->sheet[y][x].setBackgroundColor(color); highlight[3] = color; }
	//podswietlenie komorki 
	void highlightCell(int x, int y)
	{
		//przy przejsciu do nasteonej komorki przywracamy poprzedniej oryginalne kolory
		this->sheet[highlight[0]][highlight[1]].setTextColor(highlight[2]);
		this->sheet[highlight[0]][highlight[1]].setBackgroundColor(highlight[3]);
		//przepisujemy oryginalne kolory do zmiennych
		highlight[0] = y;
		highlight[1] = x;
		highlight[2] = this->sheet[y][x].getTextColor();
		highlight[3] = this->sheet[y][x].getBackgroundColor();
		//odwracamy kolory tla i tekstu
		this->sheet[y][x].setTextColor(std::abs(highlight[2] - 15));
		this->sheet[y][x].setBackgroundColor(std::abs(highlight[3] - 15));

		draw();
	}
	std::string d2s(double d)//pozyczona ze StackOverflow, zmiana double na string(z wylaczeniem przecinka dla liczb calkowitych)
	{
		size_t len = std::snprintf(0, 0, "%.10f", d);
		std::string s(len + 1, 0);
		std::snprintf(&s[0], len + 1, "%.10f", d);
		s.pop_back();
		s.erase(s.find_last_not_of('0') + 1, std::string::npos);
		if (s.back() == '.') {
			s.pop_back();
		}
		return s;
	}
	std::string calculate(std::string input) //obliczanie formul
	{
		double res = 0; //wynik
		try 
		{
			std::string FUN = input.substr(1, input.find('(') - 1); // jaka funkcja
			int colon = input.find(':');
			int startCol = alphabet.find(input.substr(input.find('(') + 1, 1));//kolumna od ktorej zaczynamy
			int startRow = std::stoi(input.substr(input.find('(') + 2, colon - input.find('(') - 2))-1;//wiersz od ktorego zaczynamy
			int endCol = alphabet.find(input.substr(colon + 1, 1));//kolumna koncowa
			int endRow = std::stoi(input.substr(colon + 2, input.find(')') - colon - 2));//wiersz koncowy
			if (endCol > sheet.size() || endRow > sheet[0].size())//sprawdzenie poprawnosci
			{
				return "#BOUNDS";
			}
			else
			{
				if (FUN == "SUM" || FUN == "SUMA")
				{
					for (int j = startCol; j <= endCol; j++)
					{
						for (int i = startRow; i < endRow; i++)
						{
							res += std::stod(this->sheet[i][j].getText());//suma pola komorek
						}
					}
				}
				else if (FUN == "ILOCZYN" || FUN == "PRODUCT")
				{
					res = 1;
					for (int j = startCol; j <= endCol; j++)
					{
						for (int i = startRow; i < endRow; i++)
						{
							res = res * std::stod(this->sheet[i][j].getText());//iloczyn pola komorek
						}
					}
				}
				else if (FUN == "ROZNICA" || FUN == "DIFFERENCE")
				{
					for (int j = startCol; j <= endCol; j++)
					{
						for (int i = startRow; i < endRow; i++)
						{
							res = res - std::stod(this->sheet[i][j].getText());//roznica pola komorek
						}
					}
				}
				return d2s(res);
			}
			
		}
		catch (std::exception) //jezeli nie jest to zadna funkcja, to liczymy wyrazenie(try/catch - pojscie na latwizne)
		{
			int row;
			try
			{
				for (int c = 0; c < alphabet.size(); c++)
				{
					int index = input.find(alphabet[c]);//miejsce wystepowania litery kolumny w stringu input
					while(std::isdigit(input[index + 1]) && c<=colWidth.size())//podmieniamy wszystkie numery komorek na ich wartosci przedtem sprawdzajac czy miescimy sie w zakresie arkusza
					{
						if (std::isdigit(input[index + 2]))
						{
							row = std::stoi(input.substr(index + 1, index + 3));
							if (row > 0 && row<=sheet.size())
							{
								input.replace(index, 3, sheet[row - 1][c].getText()); //podmiana numeru komorki na jej wartosc(zakladam maksymalna ilosc wierszy na 99 z powodu braku czasu :) )
							}
							else
							{
								break;
							}
						}
						else
						{
							row = std::stoi(input.substr(index + 1, index + 2));
							if (row > 0 && row<=sheet.size())
							{
								input.replace(index, 2, sheet[row - 1][c].getText());
							}
							else
							{
								break;
							}
						}
						index = input.find(alphabet[c]); //sprawdzamy czy wyczerpalismy wszystkie komorki z kolumny
					}
				}
				res = te_interp(input.substr(1,input.size()).c_str(), 0);//gotowa biblioteka do interpretacji rownan
			}
			catch (std::exception e) { return "#FORMULA"; }//gdyby cos poszlo nie tak
			return d2s(res);
		}
	}
	void insert(int x, int y, std::string input)//dopisywanie danych do komorki
	{
		if (input[0] == '=') //sprawdzenie czy to co wpisujemy to formula...
		{
			this->sheet[y][x].setFormula(input);
			this->sheet[y][x].setText(calculate(input));
		}
		else if (input == "")
		{
			this->sheet[y][x].setFormula(input);
			this->sheet[y][x].setText(input);
		}
		else//...czy zwykly tekst
		{
			this->sheet[y][x].setText(input);
		}
	}
	
	static void clearConsole() //czyszczenie konsoli (chyba niestety tylko windows)
	{
		HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE); // handler konsoli
		int win_width, win_height;
		CONSOLE_SCREEN_BUFFER_INFO info;
		//zmiana parametrow konsoli
		SetConsoleCursorPosition(hConsole, { 0,0 });
		GetConsoleScreenBufferInfo(hConsole, &info);
		SetConsoleTextAttribute(hConsole, 15);
		//informacje o rozmiarze
		win_width = info.srWindow.Right;
		win_height = info.srWindow.Bottom;
		std::stringstream rubber;//szybciej wyswietlic gotowy string niz coutowac po kolei
		for (int i = 0; i < win_height*win_width; i++)
		{
			rubber << " ";
		}
		std::cout << rubber.str();
	}
	void draw()//rysowanie arkusza z mapy vector<vector<Cell>>
	{
		std::stringstream line, spc;
		int len = 0;
		Cell cell;
		HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
		CONSOLE_CURSOR_INFO cursor;
		GetConsoleCursorInfo(hConsole, &cursor);
		cursor.bVisible = false;
		SetConsoleCursorInfo(hConsole, &cursor);
		SetConsoleTextAttribute(hConsole, 0x0F);
		SetConsoleCursorPosition(hConsole, { 0,0 });
		for (int len : colWidth)//linia o dlugosci arkusza
		{
			for (int i = 0; i < len; i++)
			{
				line << "-";
				this->length++;
			}
		}
		line << "--";//brakuje dwoch(z separatora kolumny z poczatku i z konca arkusza
		for (int i = 0; i < this->sheet.size(); i++)
		{
			std::cout << line.str() << "\n";//rysujemy linie pomiedzy wierszami...
			std::cout << "|";
			for (int h = 0; h < this->rowHeight[i] - 1; h++)
			{
				std::cout << "\n" << "|";//...i miedzy calymi kolumnami
			}
			for (int j = 0; j < this->sheet[0].size(); j++)
			{
				std::string dispText = "";
				cell = this->sheet[i][j];
				spc.str(std::string());
				if (cell.getFormula() != "") { cell.setText(calculate(cell.getFormula())); }//wyswietlamy wartosc formuly jezeli istnieje
				//justowanie do srodka
				if (cell.getText().length() < this->colWidth[j])
				{
					dispText = cell.getText();
				}
				else
				{
					dispText = cell.getText().substr(0, this->colWidth[j] - 1);
				}
				for (int space = 0; space < (this->colWidth[j] - dispText.length()) / 2; space++)
				{
					spc << " ";
				}
				SetConsoleTextAttribute(hConsole, (cell.getBackgroundColor() * 16) + cell.getTextColor()); // kolorowanie zgodnie z wartosciami termiala windowsa
				if (dispText.length() % 2 == 1)
				{
					if (this->colWidth[j] % 2 == 0) { spc << " "; }
					std::cout << spc.str() << dispText << spc.str().substr(0, spc.str().length() - 1);
				}
				else
				{
					std::cout << spc.str() << dispText << spc.str();
				}
				SetConsoleTextAttribute(hConsole, 0x0F);//wracamy do domyslnego koloru zeby ramka byla biala
				std::cout << "|";
			}

			std::cout << " " << i + 1 << "\n";// wyswietlamy numer wiersza na arkuszu
		}
		std::cout << line.str() << "\n";
		//indeks kolumny justowany do srodka
		for (int i = 0; i < colWidth.size(); i++)
		{
			spc.str(std::string());
			for (int s = 0; s < colWidth[i] / 2; s++)
			{
				spc << " ";
			}
			std::cout << spc.str() << alphabet[i] << spc.str();
		}
		std::cout << "\n";
	}
	void saveToFile(std::string path) //zapis
	{
		std::fstream file;
		std::stringstream out;
		file.open(path, std::ios::out); //otwieramy nowy/istniejacy plik 
		out << sheet.size() << "," << sheet[0].size() << "\n";//podajemy wymiary arkusza
		//dlugosci kazej kolumny(prosze zignorowac wysokosc wierszy - braklo czasu na implementacje)
		for (int w : colWidth)
		{
			out << "," << w ;
		}
		out << "\n";
		for (int h : rowHeight)
		{
			out << "," << h;
		}
		out << "\n";
		//dane komorek
		for (int i = 0; i < sheet.size(); i++)
		{
			for (int j = 0; j < sheet[0].size(); j++)
			{
				Cell cell = sheet[i][j];
				out << "i"<< cell.getID() << "[" << cell.getFormula() << "," << "CENTER" << "," << cell.getTextColor() << "," << cell.getBackgroundColor() << "]"<<"\n"<<cell.getText()<<"\n";
			}
		}
		file << out.str();//wszystkie dane zapisujemy z jednego stringstreama
		file.close();//zamykamy plik
	}
	void openFromFile(std::string path)//otwieranie arkusza z pliku
	{
		std::vector<std::vector<Cell>> map; //wyjsciowy arkusz
		std::vector<Cell> row;
		std::vector<int> comma;//wektor indeksow przecinkow(do odczytu szerokosci kolumn)
		std::ifstream file(path);//otwieramy plik ze sciezki
		std::string line;//"bufor"
		int line_count = 0;//iterator
		int commas[3];//indeksy przecinkow z danych komorek
		Cell cell;
		std::getline(file, line);//przewijamy linie z wymiarami arkusza
		std::getline(file, line);
		for (int i = 0; i < line.size(); i++)
		{
			if (line[i] == ',') comma.push_back(i);//indeksy przecinkow(ulatwia wydobycie wartosci dlugosci)
		}
		for (int i = 0; i < comma.size(); i++)
		{
			if (i < comma.size() - 1)
			{
				colWidth.push_back(std::stoi(line.substr(comma[i] + 1, comma[i + 1])));//odczyt dlugosci, ilosci kolumn i przekazadnie go do wektora
			}
			else
			{
				colWidth.push_back(std::stoi(line.substr(comma[i] + 1, line.size())));
			}
		}
		std::getline(file, line);
		comma.clear();
		for (int i = 0; i < line.size(); i++)
		{
			if (line[i] == ',') comma.push_back(i);
		}
		for (int i = 0; i < comma.size(); i++)
		{
			if (i < comma.size() - 1)
			{
				rowHeight.push_back(std::stoi(line.substr(comma[i] + 1, comma[i + 1]))); //odczyt wysokosci kolumn(choc bardziej chodzi o ich ilosc poniewaz wysokosc nie zostala zaimplementowana)
			}
			else
			{
				rowHeight.push_back(std::stoi(line.substr(comma[i] + 1, line.size())));
			}
		}
		while (std::getline(file, line)) //wczytanie danych komorek
		{
			if (line_count % 2 == 0) //parzyste linie to wlasosci komorek...
			{
				id = std::stoi(line.substr(line.find('i') + 1, line.find('[')));//numer
				commas[0] = line.find(',');
				commas[1] = 1 + commas[0] + line.substr(commas[0] + 1, line.size()).find(',');
				commas[2] = 1 + commas[1] + line.substr(commas[1] + 1, line.size()).find(",");
				cell = Cell(id);
				cell.setFormula((line.substr(line.find('[') + 1, commas[0] - line.find('[') - 1)));//formula
				cell.setAlignment((line.substr(commas[0] + 1, commas[1])));//justowanie(niezaiplementowane z braku czasu - domyslnie justowane na srodek)
				cell.setTextColor(std::stoi(line.substr(commas[1] + 1, commas[2])));//kolor tekstu
				cell.setBackgroundColor(std::stoi(line.substr(commas[2] + 1, line.find(']'))));//kolor tla

				line_count++;
			}
			else//... a nieparzyste to ich tekst(by uniknac konfliktu znakow)
			{
				cell.setText(line);
				row.push_back(cell);//komorke z wszystkimi danymi zapisujemy do wiersza
				line_count++;
			}
			if (line_count / 2 >= colWidth.size())//nowy wiersz wystepuje co wczytana ilosc kolumn
			{
				line_count = 0;
				map.push_back(row);//caly wiersz zapisujemy do arkusza
				row.clear();//czyscimy wiersz 
			}
		}
		this->sheet = map;
	}
};
