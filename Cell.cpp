
#include<windows.h>
#include <string>
#include<iostream>

//klasa pojedynczej komorki arkusza
class Cell
{
private:
	std::string align = "CENTER";//justowanie(nie zaimplementowane)
	int text_color = 15;//kolor tekstu
	int background_color = 0;//kolor tla
	std::string text = "";//zawartosc tekstowa
	std::string formula = "";//zawartosc formuly
	int id=0, width=0, height=0, col=0, row=0; //parametry
public:
	//konstruktory
	Cell() 
	{
		this->width = 13;
		this->height = 1;
	}
	~Cell() {}
	Cell(int id)
	{
		this->id = id;
		this->width = 13;
		this->height = 1;
		this->col = 0;
		this->row = 0;
	}
	Cell(int id, std::string text)
	{
		this->id = id;
		this->width = 13;
		this->height = 1;
		this->text = text;
	}
	int getTextColor() { return text_color; }
	//gettery
	int getBackgroundColor() { return background_color; }
	int getID() { return id; }
	int getWidth() { return width; }
	int getHeight() { return height; }
	int getColumn() { return col; }
	int getRow() { return height; }
	std::string getText() { return text; }
	std::string getFormula() { return formula; }
	std::string getAlignment() { return align; }
	//settery
	void setTextColor(int color) { this->text_color = color; }
	void setBackgroundColor(int color) { this->background_color = color; }
	void setText(std::string text) { this->text = text; }
	void setFormula(std::string form) { this->formula = form; }
	void setID(int id) { this->id = id; }
	void setWidth(int width) { this->width = width; }
	void setHeight(int height) { this->height = height; }
	void setAlignment(std::string align) { this->align = align; }
};