#include <stdio.h>
#include <stdlib.h>
#include <windows.h>
#include <conio.h>


//Set up a condition of movement
enum State {UP, DOWN, LEFT, RIGHT} tt;

//Source (Clear screen): http://diendan.congdongcviet.com/archive/index.php/t-4577.html
void clrscr()
{
	CONSOLE_SCREEN_BUFFER_INFO	csbiInfo;
	HANDLE	hConsoleOut;
	COORD	Home = {0,0};
	DWORD	dummy;

	hConsoleOut = GetStdHandle(STD_OUTPUT_HANDLE);
	GetConsoleScreenBufferInfo(hConsoleOut,&csbiInfo);

	FillConsoleOutputCharacter(hConsoleOut,' ',csbiInfo.dwSize.X * csbiInfo.dwSize.Y,Home,&dummy);
	csbiInfo.dwCursorPosition.X = 0;
	csbiInfo.dwCursorPosition.Y = 0;
	SetConsoleCursorPosition(hConsoleOut,csbiInfo.dwCursorPosition);
}

void gotoxy();
void table();
void movement(int *ptrx, int *ptry);
void control();
void body(int *ptrx, int *ptry, int currentscore);
void touchwall(int *ptrx, int *ptry, int *count);
void touchbody(int *ptrx, int *ptry, int *count, int currentscore);
void food(int *x, int *y, int *ptrx, int *ptry, int currentscore, int currentfood);
void screenplay(int currentscore);
int menu();
void name();

int main() 
{
	//This is a checkpoint, if the game is over, and the user want to return the menu, it will go here.
	AGAIN: 
	//Paint the background
	system("color 0a");
	
	//First, run the menu, if it returns value 1, it means that the user choose "Start game"
	if (menu()==1){
	clrscr();
	system("color 5b");
	//Process of inputting user's name
	name();
	system("color 1a");
	
	//Declare the score + the variable to monitor the state game
	int score=0;			int countover = 0;
	//Declare the safe food
	int countfood=0;
	//Declare the whole parts of snake, snake[0] will be the head
	int snakex[22]={0};   	int snakey[22]={0};
	//Making the table
	table();
	//Declare the position of food, "a" as position in X-axis, "b" in Y-axis
	int a=0, b=0;
	//Declare the head
	snakex[0]=40; 			snakey[0]=17;
	gotoxy(snakex[0],snakey[0]);
	
	//Initialize the movement of the snake, it will first go leftwards
	tt = LEFT;
	
	//Display the first food
	food(&a, &b, &snakex[0], &snakey[0],score,countfood);
	
		
	
	while(1){
		//Delay the program in 50 milliseconds
		Sleep(50);
		
		//Display the name and score
		screenplay(score);
		
		//Monitor the keyboards inputted by the user		
		control();
		
		//If the snake's head touches the fruit, the score will increase by 1, and the other food will appear
		if (snakex[0]==a && snakey[0]==b)
			{
				score++;
				food(&a, &b, &snakex[0], &snakey[0],score,countfood);
			}
		
		//Manage the movement of the snake's head
		movement(&snakex[0], &snakey[0]);
		
		//Handle the display of the body
		body(&snakex[0], &snakey[0],score);
		
		//Checking if the snake's head touches wall, touches its body or not
		touchwall(&snakex[0], &snakey[0], &countover);
		touchbody(&snakex[0], &snakey[0], &countover, score);
		
		
		//Countover: the variable monitoring the value to tell the program that "Game is over!"
		//countover = 0 --> Game is in normal state
		//countver != 0 --> Game is over
		if (countover!=0){
			gotoxy(25,6);
			printf("* Press ENTER to return menu!");
			gotoxy(25,8);
			printf("* Press ESC to exit the game!");
			break;
		}
		
		//Checking if the user win the game or not	
		if (score == 21){
			gotoxy(35,2);
			printf("You win!!!!!");
			gotoxy(25,6);
			printf("* Press ENTER to return menu!");
			gotoxy(25,8);
			printf("* Press ESC to exit the game!");
			break;
		}
	}
	
	//Waiting the user input the keys to return the menu or exit the game
	//This block will run if the game is in "Game over state" or "Winning state"
	while (1){
	//Catch the keyboard event from user
	if (kbhit())
        {
            char key = _getch();
            if (key == 13)
            	goto AGAIN;
            else if (key == 27)
            	goto END;
        }
    }
}
	//This is an label. If the user want to exit the game, then the program will run here
	END: 
	return 0;
}


//Source (Sample of "gotoxy" function): https://www.youtube.com/watch?v=wEmzyepukxg
void gotoxy (int x, int y)
{
    COORD c;
    c.X=x;
    c.Y=y;
    SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE),c);
}

//Create the wall on the screen
void table()
{
	//Set up borders
	gotoxy(0,10);
	printf("%c", 218);
	gotoxy(79,10);
	printf("%c", 191);
	gotoxy(0,24);
	printf("%c", 192);
	gotoxy(79,24);
	printf("%c", 217);
	
	//Set up horizontal lines
	int i=0;
	for (i=1; i<=78; i++){
		gotoxy(i,10);
		printf("%c", 196);
		gotoxy(i,24);
		printf("%c", 196);
	}
	
	//Set up vertical lines
	for (i=11; i<=23; i++){
		gotoxy(0,i);
		printf("%c", 179);
		gotoxy(79,i);
		printf("%c", 179);
	}
	printf("\n\n");
}

void movement(int *ptrx, int *ptry)
{
	/*
	  tt: Condition of the snake
	  		ex: tt = UP -> The snake is going upwards
	*/

  	if (tt == UP)
    {
		//The snake is going up, it means that the coordinate "y" will decrease
		(*ptry)--;
		//After decreasing the coorinate "y", move the cursor (teleport the head of snake) to new position
        gotoxy(*ptrx,*ptry);
        //Output the head of snake
      	printf("*");
    }
    else if (tt == DOWN)
    {
        (*ptry)++;
        gotoxy(*ptrx,*ptry);
      	printf("*");
    }
    else if (tt == LEFT)
    {
        (*ptrx)--;
        gotoxy(*ptrx,*ptry);
      	printf("*");
    }
    else if (tt == RIGHT)
    {
        (*ptrx)++;
        gotoxy(*ptrx,*ptry);
      	printf("*");
    }

}

//Manage the movement of the snake's head
void control()
{
	if (kbhit())
        {
    /*
	The format of the catching keyborad event of the system is:
		if (kbhit())
		{
			char key = _getch();
			
			...
		}
	*/
//tt: the enum-variable, it just receive only 4 values (UP, DOWN, LEFT, RIGHT).
//The enum variable make easier for everyone to read.
//If tt = LEFT, it means that the current state of the snake's head is LEFT
//, and the snake is moving leftwards.
			
			char key = _getch();

            if ((key == 'a' || key == 75) && tt!=RIGHT)
            	tt = LEFT;
			else if ((key == 's' || key == 80) && tt!=UP)
            	tt = DOWN;
			else if ((key == 'd' || key == 77) && tt!=LEFT)
            	tt = RIGHT;
			else if ((key == 'w' || key == 72) && tt!=DOWN)
            	tt = UP;
        }
}

//Handle displaying the parts of the body
void body(int *ptrx, int *ptry, int currentscore)
{
	int i=0;

	gotoxy(ptrx[currentscore + 1] , ptry[currentscore + 1]);
	printf(" ");
	for (i=currentscore+1; i>=1; i--){
		ptrx[i] = ptrx[i-1];
		ptry[i] = ptry[i-1];
	}
	for (i=currentscore+1; i>=0; i--){
		gotoxy(ptrx[i] , ptry[i]);
		printf("*");
	}
	
}

void touchwall(int *ptrx, int *ptry, int *count)
{
	//The wall is in the positions:
	// x = 0
	// x = 79
	// y = 10
	// y = 24
	// *ptrx: The current position of the snake's head in X-axis (Snake's head)
	// *ptry: The current position of the snake's head in Y-axis (Snake's head)
	if (*ptrx == 0 || *ptrx == 79 || *ptry == 10 || *ptry == 24){
		gotoxy (35,2);
		printf("Game over!");
		
		//The "countover" will not be zero, so it tell the program that it is in "Game over" state
		(*count)++;
	}
		
}

void touchbody(int *ptrx, int *ptry, int *count, int currentscore)
{
	int i=0;
	
	for(i=2; i<=currentscore; i++){
		if (ptrx[0] == ptrx[i] && ptry[0] == ptry[i]){
			gotoxy (35,2);
			printf("Game over!");
			
			//The "countover" will not be zero, so it tell the program that it is in "Game over" state
			(*count)++;
		}
	}
	
	
}


void food(int *x, int *y, int *ptrx, int *ptry, int currentscore, int currentfood)
{
	srand(time(NULL));
	
	//If the food touches the snake when first appearing, the loop will run again
	while (1)
	{
		int i=0;
		
		//currentfood: a variable to check if the food "touches" the snake when just appearing
		//If currentfood = 0, the food will appear properly
		currentfood = 0;
		
		//*x, *y: the position of the current cursor
		*x = 1 + rand() % 78;
		*y = 11 + rand() % 13;
		
		//Checking if the food "touch" the body of the snake
		//First, we scan over the array containing postion of all parts of snake
		//Then, if the position of the food = position of a part of snake -> currentfood is not be zero
		for(i=0; i<=currentscore; i++){
		if (*x == ptrx[i] && (*y == ptry[i]) )
			currentfood++;
		}
		
		//If the food is "safe" (not touch the body of the snake), create the food on the screen, and exit the while loop
		if (currentfood==0)
		{
			gotoxy (*x,*y);
			printf("O");
			break;
		}			
			
	}	
	
	
}

//Display the current score
void screenplay(int currentscore)
{
	gotoxy(50,4);
	printf("Score: %d", currentscore);
}


int menu()
{
	int y=13;
	
	//Clear everything on the console screen
	clrscr();
	
	//Print the title of the game
	gotoxy(21,5);
	printf("*****  *   *      *      *    *  *****");
	gotoxy(21,6);
	printf("*      **  *     * *     *   *   *");
	gotoxy(21,7);
	printf("*****  * * *    *****    ***     ***");
	gotoxy(21,8);
	printf("    *  *  **   *     *   *   *   *");
	gotoxy(21,9);
	printf("*****  *   *  *       *  *    *  *****");
	
	gotoxy(35,13);
	printf("Start game");
	gotoxy(35,15);
	printf("Exit");
	
	
	//Set up an triangle choice on the screen
	gotoxy(33,13);
	putchar(16);
	
	
	gotoxy(18,20);
	printf("* Note: Press WASD or arrow keys to control!");
	gotoxy(26,22);
	printf("Press ENTER to choose!");
	
	//Waiting the user input the keyboards
	while(1){

	if (kbhit())
        {
            char key = _getch();

            //The user press "s" or arrow down key	
			if (key == 's' || key == 80){
				
				//Delete the old triangle
				gotoxy(33,13);
				printf(" ");
				
				//Create new triangle
				gotoxy(33,15);
				putchar(16);
				
				//Record the new position (just y coordinate) to variable y
				y = 15;
			}
            	

            //The user press "w" or arrow up key	
			if (key == 'w' || key == 72){
				//Delete the old triangle
				gotoxy(33,15);
				printf(" ");
				
				//Create new triangle
				gotoxy(33,13);
				putchar(16);
				
				//Record the new position (just y coordinate) to variable y
				y = 13;
			}
			
			//If the triangle points to the line "Start game", then play game
			if (key == 13 && y==13)
				return 1;
				
			//If the triangle points to the line "Exit", then exit the game
			if (key == 13 && y==15)
				return 2;	   
	}
}
}

//Handle the process of inputting user's name
void name()
{
	//Clear the screen
	clrscr();
	
	//Create the array containing the name
	char n[30];
	
	
	gotoxy(4, 18);
	printf("*Instruction: After typing your name, press ENTER to play.");
	gotoxy(30, 22);
	printf("Good luck to you ^^");
	
	gotoxy(25,12);
	printf("Enter your name: ");
	
	//The same as "scanf", but it can allow the user input the name with "space"
	fgets(n, sizeof(n), stdin);
	
	//Clear screen again and go to the in-game screen
	clrscr();
	gotoxy(15,4);
	printf("Your name: %s", n);
}
	

