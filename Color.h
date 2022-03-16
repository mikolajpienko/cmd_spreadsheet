class Color
{
private:
	char val = 0;
	Color* color;
public:

	Color(char val)
	{
		this->val = val;
	}
	Color(Color* color)
	{
		this->color = color;
	}
	static Color black() { return Color('0'); }
	static Color dark_red() { return Color('4'); }
	static Color dark_green() { return Color('2'); }
	static Color dark_yellow() { return Color('6'); }
	static Color dark_blue() { return Color('1'); }
	static Color dark_magenta() { return Color('5'); }
	static Color dark_cyan() { return Color('3'); }
	static Color light_gray() { return Color('7'); }
	static Color dark_gary() { return Color('8'); }
	static Color bright_red() { return Color('C'); }
	static Color bright_green() { return Color('A'); }
	static Color bright_yellow() { return Color('E'); }
	static Color bright_blue() { return Color('9'); }
	static Color bright_magenta() { return Color('D'); }
	static Color bright_cyan() { return Color('B'); }
	static Color white() { return Color('F'); }


	char setColor(Color color)
	{
		return color.val;
	}
	char setColor(char val)
	{
		return val;
	}
};