/* 
 *  Console Color and Menu Class
 *  Created By Jason D. Weeks
 *  757-651-6886
 *  jweeks3631@student.gwinnetttech.edu
 */


#ifndef _UTILITIES_
#define _UTILITIES_

#include <iosfwd>

/*
 *  Uses Windows API to change console colors
 *  By Eklavya Sharma
 *  http://www.cplusplus.com/articles/Eyhv0pDG/
 *
 *  Modified by Jason D. Weeks
 */
struct TextColor
{
	enum code
	{
		black    = 0,   gray   = 8,
		d_blue   = 1,   blue   = 9,
		d_green  = 2,   green  = 10,
		d_cyan   = 3,   cyan   = 11,
		d_red    = 4,   red    = 12,
		d_purple = 5,   purple = 13,
		d_yellow = 6,   yellow = 14,
		d_white  = 7,   white  = 15
	};
};

struct BackColor
{
	enum code
	{
		black    = 0,   gray   = 8,
		d_blue   = 1,   blue   = 9,
		d_green  = 2,   green  = 10,
		d_cyan   = 3,   cyan   = 11,
		d_red    = 4,   red    = 12,
		d_purple = 5,   purple = 13,
		d_yellow = 6,   yellow = 14,
		d_white  = 7,   white  = 15
	};
};

typedef TextColor::code Text;
typedef BackColor::code Back;

class ccl
{
	static Text   default_text_color,
	              current_text_color;
	static Back   default_back_color,
	              current_back_color;

	static void*  std_con_out;

	static void setColor(Text textcolor, Back backcolor);

public:

	static bool colorProtect;

	static void activateConsoleColor();
	static void setToDefaultColors();
	static void setTodefaultTextcolor(); 
	static void setTodefaultBackColor(); 

	/* Overloaded Operators */
	template<class elem,class traits>
	friend std::basic_ostream<elem,traits>& operator <<(std::basic_ostream<elem,traits> &os, Text col);

	template<class elem,class traits>
	friend std::basic_istream<elem,traits>& operator >>(std::basic_istream<elem,traits> &is, Text col);

	template<class elem,class traits>
	friend std::basic_ostream<elem,traits>& operator <<(std::basic_ostream<elem,traits> &os, Back col);

	template<class elem,class traits>
	friend std::basic_istream<elem,traits>& operator >>(std::basic_istream<elem,traits> &is, Back col);

}; // end of ccl -------------------------------------------------------------------------------------

/* Overloaded Operators */
template<class elem,class traits>
std::basic_ostream<elem,traits>& operator <<(std::basic_ostream<elem,traits> &os, Text col)
{
	os.flush();
	ccl::setColor( col, ccl::current_back_color );
	return os;
}

template<class elem,class traits>
std::basic_istream<elem,traits>& operator >>(std::basic_istream<elem,traits> &is, Text col)
{
	std::basic_ostream<elem,traits>* p = is.tie();

	if (p != NULL)
		p->flush();

	ccl::setColor( col, ccl::current_back_color );
	return is;
}

template<class elem,class traits>
std::basic_ostream<elem,traits>& operator <<(std::basic_ostream<elem,traits> &os, Back col)
{
	os.flush();
	ccl::setColor( ccl::current_text_color, col );
	return os;
}

template<class elem,class traits>
std::basic_istream<elem,traits>& operator >>(std::basic_istream<elem,traits> &is, Back col)
{
	std::basic_ostream<elem,traits>* p = is.tie();

	if (p != NULL)
		p->flush();

	ccl::setColor( ccl::current_text_color(), col );
	return is;
}

struct Menu
{
	enum optionMode  
	{
		MIDDLE = 0, 
		FIRST  = 2,  
		LAST   = 4,   
		SINGLE = 8  
	};
	
	static const int OPTION_BOX_PADDING = 12;
	static int box_size;
	static void title(const char* title);
	static void option(int number, const char* option_name, optionMode o = Menu::MIDDLE);
	static void indent();

private:

	static int title_box_length;
	static int indent_1;
	static int indent_2;
};

#endif
