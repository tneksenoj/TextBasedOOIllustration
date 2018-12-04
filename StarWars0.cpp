////////////////////////////////////////////////////////////////
// Author:  Kent Jones
// Purpose: Illustrate inheritance, polymorphism and OO concepts
//          in the context of a simple text based game.
// Date:        Changes:
// 12/14/2018  Pushed version to github
////////////////////////////////////////////////////////////////
#include <iostream>
#include <string>
#include <vector>
#include <cstdlib>
#include <ctime>
#include <windows.h> 
#include <Wincon.h>
using namespace std;

// Stuff for output to console screen
HANDLE hStdout, hStdin; 
CONSOLE_SCREEN_BUFFER_INFO csbiInfo; 

void ShowConsoleCursor(bool showFlag)
{
	HANDLE out = GetStdHandle(STD_OUTPUT_HANDLE);
    CONSOLE_CURSOR_INFO     cursorInfo;
    GetConsoleCursorInfo(out, &cursorInfo);
    cursorInfo.bVisible = showFlag; // set the cursor visibility
    SetConsoleCursorInfo(out, &cursorInfo);
}

// Max dimensions of text based space game
const int MAX_X = 40;
const int MAX_Y = 20;

// A Generic Game Object
class GameObject 
{
protected:
    char shape;
	COORD pos;
	int vx, vy;
public:
	// Constructor for the base class
	GameObject( char Shape ) : shape(Shape)
	{   
		// random position in console
		pos.X = rand() % MAX_X;
        pos.Y = rand() % MAX_Y;

		// Velocity in x and y
		vx = rand() % 3 + -1;
		vy = rand() % 3 + -1;
    }

    // Draw the object
    virtual void draw( ) 
	{	SetConsoleCursorPosition(hStdout,pos);
		cout << shape;
	}
	
	// Move the object (for now this just adds 1 to x and y)
    virtual void move( )
    {	
		pos.X += vx; 
		pos.Y += vy;
		// Keep the object inside the bounds 
		if (pos.X >= MAX_X) { vx *= -1; pos.X = MAX_X - 1; }
		if (pos.Y >= MAX_Y) { vy *= -1;	pos.Y = MAX_Y - 1; }
		if ( pos.X <= 0 )   { vx *= -1;	pos.X = 1; 		   }
		if (pos.Y <= 0 )    { vy *= -1;	pos.Y = 1; 		   }
		//cout << pos.X << " " << pos.Y << " " << vx << " " << vy << " ";
	}
};

class SpaceShip : public GameObject
{

public:
    SpaceShip( char Shape ) : GameObject(Shape) 
    {}
	
	void move()
	{	GameObject::move();
	}
};

class Bullet : public GameObject
{
public:
	Bullet() : GameObject('.')
	{}

	void move()
	{	GameObject::move();
	}
};

int main()
{
	srand(unsigned(time(NULL)));
	vector<GameObject *> gobjects;


    // Get handles to STDIN and STDOUT. 
    hStdin = GetStdHandle(STD_INPUT_HANDLE); 
    hStdout = GetStdHandle(STD_OUTPUT_HANDLE); 
    if (hStdin == INVALID_HANDLE_VALUE || 
        hStdout == INVALID_HANDLE_VALUE) 
    {
        MessageBox(NULL, TEXT("GetStdHandle"), TEXT("Console Error"), 
            MB_OK);
        return 1;
    }

	char chmenu, chobj;
	do
	{	cout << "What type of game object do you want?" << endl;
		cout << "0: SpaceShip" << endl;
		cout << "1: Bullet" << endl;
		cout << "2: Finished" << endl;

		cin >> chmenu;
		switch ( chmenu )
		{
		case '0':  cout << "What character do you want for a SpaceShip? ";
                   cin >> chobj;
                   gobjects.push_back( new SpaceShip(chobj) ); 
                   break;
		case '1':  gobjects.push_back( new Bullet() );    
                   break;
		case '2': break;
		}
	} while( chmenu== '0' || chmenu == '1' );

    int steps;
    cout << "How many steps do you want the game to loop for? " << endl;
    cin >> steps;

    for ( int c=0; c<steps; c++ )
    {
	    for ( int i=0; i<gobjects.size(); i++ )
	    {	gobjects[i]->draw();
            gobjects[i]->move();
	    }
		Sleep(250);
		cout << "Step " << c << endl;
		system("cls");
		ShowConsoleCursor(false);
    }
	ShowConsoleCursor(true);
	return 0;
}


