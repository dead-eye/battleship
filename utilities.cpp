/* 
 *  Console Color and Menu Class
 *  Created By Jason D. Weeks
 *  757-651-6886
 *  jweeks3631@student.gwinnetttech.edu
 */

#include "utilities.h"
#include <Windows.h>
#include <iostream>
#include <iomanip>

using namespace std;

/*
 ---------------------------------------------------------------------------------------------
 *  Console Color                                                                            *---------------------------------------------
 ---------------------------------------------------------------------------------------------
*/ 

/* Intitialize Static Variables */
Text  ccl::default_text_color = Text(0),
      ccl::current_text_color = Text(0);
Back  ccl::default_back_color = Back(0),
      ccl::current_back_color = Back(0);

void* ccl::std_con_out        = 0;
bool  ccl::colorProtect       = true;

void ccl::activateConsoleColor() 
{
	  // get address for handle
	std_con_out = GetStdHandle( STD_OUTPUT_HANDLE );

	  // create an object to hold console screen info
	CONSOLE_SCREEN_BUFFER_INFO csbi;

	  // load object with console screen info
	GetConsoleScreenBufferInfo( std_con_out, &csbi );

	  /* get current text and background color and assigned those values
	     to the default variables */
	default_text_color = current_text_color = Text(csbi.wAttributes & 15);
	default_back_color = current_back_color = Back((csbi.wAttributes & 0xf0)>>4);
}

void ccl::setColor(Text textcolor, Back backcolor)
{
	  /* if color protect is true and both text and back colors
	     are the same then abort the function */
	if (colorProtect && textcolor == (Text)backcolor)
		return;

	  // update current color variables
	current_text_color = textcolor;
	current_back_color = backcolor;

	  // store new colors to a new 2 byte variable
	unsigned short wAttributes = ((unsigned int)backcolor << 4) | (unsigned int)textcolor;

	  // set console color based on the variable 'wAttributes'
	SetConsoleTextAttribute( std_con_out, wAttributes );	  
}

void ccl::setTodefaultTextcolor() 
{ 
	setColor( default_text_color, current_back_color );
}

void ccl::setTodefaultBackColor() 
{ 
	setColor( current_text_color, default_back_color );
}

void ccl::setToDefaultColors()
{
	setColor( default_text_color, default_back_color );
}	

/*
 ---------------------------------------------------------------------------------------------
 *  END OF Console Color --------------------------------------------------------------------*---------------------------------------------
 ---------------------------------------------------------------------------------------------
*/
/*
 ---------------------------------------------------------------------------------------------
 *  Menu Class                                                                               *---------------------------------------------
 ---------------------------------------------------------------------------------------------
*/ 

/* Initialize Static Variables */
int Menu::box_size = 0;
int Menu::title_box_length = 0;
int Menu::indent_1 = 0;
int Menu::indent_2 = 0;
	
void Menu::title(const char* title)
{	
	  // reset variable
	indent_1 = 0;

	  // make sure box size is not negative
	if (box_size < 0)
		box_size = 0;
	
	  // get string length of title
	title_box_length = strlen( title ) + 6;
	  
	  /* Create extra margin size if needed */
	int size = box_size + OPTION_BOX_PADDING;
	if (size < title_box_length)
		size = title_box_length;
	int outside_margin = (size - title_box_length)/2;



//	/* 
//	 *                       * MENU TITLE LAYOUT *
//	 * 
//	 *  top box border ---->  //----------------//
//	 *  left box border --->  /*  'menu title'  */  <- right border                          
//	 *  bottom box border ->  //----------------//
//	 *
//	 */

	cout << Text::d_yellow;
	cout << '\n';

	 // top box border
	cout << '\t';
	cout << setw( outside_margin + 2 ) << "//";
	for (int i = 0; i < title_box_length - 4; i++)
		cout << "-";
	cout << "//\n";

	  // left box border
	cout << '\t';
	cout << setw( outside_margin + 2) << "/*" << " ";

	  // menu title
	cout << Text::d_white << title;
	
	  // right box border
	cout << Text::d_yellow << " */\n";

	  // bottom box border
	cout << '\t';
	cout << setw( outside_margin + 2) << "//";
	for (int i = 0; i < title_box_length - 4; i++)
		cout << "-";
	cout << "//\n";

	  // set set colors to their defaults 
	ccl::setToDefaultColors();
}
	
void Menu::option(int number, const char* option_name, optionMode o)
{
	  // make sure box size is not negative
	if (box_size < 0)
		box_size = 0;
	
	const char* OPTION_BOX_HEADER = "[Enter a number]";
	int         box_header_length = strlen( OPTION_BOX_HEADER );
	int         size              = box_size + OPTION_BOX_PADDING;

	  /* create indent sizes if neeeded */
	if (size < title_box_length)
		indent_1 = (title_box_length - (size < box_header_length? box_header_length : size))/2;
	if (size < box_header_length)
		indent_2 = (box_header_length - size)/2;

	/* 
	 *                             * OPTION BOX LAYOUT *
	 * 
	 *  box header ---------------->  [Enter a number]
	 *  top box border ---->  --------------------------------
	 *  left box border --->  |        'empty space'         |  <- right box boder
	 *  left box border --->  |   'option number and name'   |  <- right box boder
	 *  left box border --->  |        'empty space'         |  <- right box boder
	 *  bottom box border ->  --------------------------------
	 *
	 */

	cout << Text::d_yellow;
	 
	if (o == FIRST || o == SINGLE) 
	{
		  // box header
		cout << Text::d_white << "\n\t";
		for (int i = 0; i < indent_1; i++)
			cout << ' ';
		cout << setw( box_header_length + (size - box_header_length)/2 ) << OPTION_BOX_HEADER;
		cout << Text::d_yellow << '\n';

		 // top box border
		indent();		
		for (int i = 0; i < size; i++)
			cout << '-';
		cout << '\n';

		  // left box border
		indent();		
		cout << '|';

		  // empty option space
		for (int i = 0; i < size - 2; i++)
			cout << ' ';

		  // right box border
		cout << "|\n";
	}

	  // left box border
	indent();
	cout << "| ";

	  // option number and name
	cout << Text::d_white << setw( 3 ) << number;
	cout << left;
	cout << ". " << Text::d_yellow << setw( size - 8 ) << option_name;

	  // right box border
	cout << "|\n";
	cout << right;

	  // left box border
	indent();
	cout << '|';

	  // empty space
	for (int i = 0; i < size - 2; i++)
		cout << " ";

	  // right box border
	cout << "|\n";

	if (o == LAST || o == SINGLE)  
	{
		  // bottom box border
		indent();
		for (int i = 0; i < size; i++)
			cout << "-";
		cout << "\n\n";

		  // reset variable
		title_box_length = 0;
	}
	  // set colors to their defaults
	ccl::setToDefaultColors();
}

void Menu::indent()
{
	cout << '\t';
	for (int i = 0; i < indent_1; i++)
		cout << ' ';
	for (int i = 0; i < indent_2; i++)
		cout << ' ';
}
/*
 ---------------------------------------------------------------------------------------------
 *  END OF Menu Class -----------------------------------------------------------------------*---------------------------------------------
 ---------------------------------------------------------------------------------------------
*/ 
