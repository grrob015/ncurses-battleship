#include <ncurses.h>
#include <time.h>
#include <stdlib.h>
#define MAX_X 80
#define MAX_Y 24

//goals -> practice writing functions that we can use
//use ncurses to make a more visual game

void print_board(int board[10][10],int refy, int refx)
{
	//print top portion
	move(refy, refx); printw("    0 1 2 3 4 5 6 7 8 9");
	move(refy+1, refx); printw("  X--------------------X");

	//loop section
	int x, y;

	for(y = 0; y <= 9; y++)
	{
		//once for each row
		//print left bookend
		move(refy+2+y, refx);
		printw("%c |",'A'+y);

		//read and print the data from the array
		for(x = 0; x <= 9; x++)
		{
			switch(board[x][y])
			{
				case 0:
					printw(" .");
					break;
				case 1:
					printw("##");
					break;
				case 2:
					printw("[]");
					break;
				case 3:
					printw("><");
					break;
				case 9:
					printw(" .");
					break;
				default:
					//error code, hopefully won't mess up spacing but will alert me
					printw("EE");
					break;
			}
		}

		//once per row
		//print right bookend
		printw("|");

	}

	//print bottom portion
	move(refy+12, refx);
	printw("  X--------------------X");

}	

void place_ship(int board[10][10], int codes[20], int length, int refy, int refx)
{
	//one function for all ship placement.
	//base limits of where you can move your ship based on length (length = ship length)
	
	//collision detection from adding up existing space value with theoretically ship placement
	//do that in real time (after each keypress) and print the error "EE" when detected
	//r to rotate, space to place (rotate bottom/left square depending on horizontal or vertical placement) (arrow keys to move obviously)
	//BOTTOMEST/LEFTMOST SQUARE IS "REFERENCE SQUARE"


	clear();
	
	int i, collision;
	int vertical = 0;
	int x, y;
	x = 0;
	y = 9;

	//updates to array need to happen before it is printed or else the game will be a frame (one input) behind
	//take position of x, y and modify array to reflect the theoretical and collisions (temporary +2 to values affected by coords and rotation)
	
	//assume position is safe.
	for(;;)
	{
		if(vertical)
		{
			for(i = length - 1; i >= 0; i--) board[x][y - i] += 2;	
		} else {
			for(i = 0; i <= length - 1; i++) board[x + i][y] += 2;
		}

		//print board, instructions, and move cursor to "reference square"
		print_board(board, refy, refx);
		move(2,25); printw("Place your ships!");
		move(3,25); printw("You'll be placing them from largest to smallest. (Lengths of 5, 4, 3, 3, and 2.)");
		move(5,25); printw("Arrow keys to move, R to rotate your ship, and space to place it.");
		move(6,25); printw("EE will show overlaps of ships. Move the ship to a valid position, then place.");

		move(refy + 2 + y, refx + 4 + (2 * x));
		refresh();

		//interactive bit
		//when modify position is selected (either arrows or rotating), -2 to current values
		//if placing, test for collision, deny if true, update array and end function if false (no collisions)

		//safely reuse i
		i = getch();

		if(i == KEY_DOWN)
		{
		
			if(vertical)
			{
				for(i = length - 1; i >= 0; i--) board[x][y - i] -= 2;
				if (y < 9) y++;
			} else {
				for(i = 0; i <= length - 1; i++) board[x + i][y] -= 2;
				if (y < 9) y++;
			}
			i = KEY_DOWN;
		}

		if(i == KEY_UP)
		{
			if(vertical)
			{
				for(i = length - 1; i >= 0; i--) board[x][y - i] -= 2;
				if(y > (length - 1)) y--;
			} else {
				for(i = 0; i <= length - 1; i++) board[x + i][y] -= 2;
				if(y > 0) y--;
			}
			i = KEY_UP;
		}
	
		if(i == KEY_LEFT)
		{
			if(vertical)
			{
				for(i = length - 1; i >= 0; i--) board[x][y - i] -= 2;
				if(x > 0) x--;
			} else {
				for(i = 0; i <= length - 1; i++) board[x + i][y] -= 2;
				if(x > 0) x--;
			}
			i = KEY_LEFT;
		}
	
		if(i == KEY_RIGHT)
		{
			if(vertical)
			{
				for(i = length - 1; i >= 0; i--) board[x][y - i] -= 2;
				if(x < 9) x++;
			} else {
				for(i = 0; i <= length - 1; i++) board[x + i][y] -= 2;
				if(x < (10 - length)) x++;
			}
			i = KEY_RIGHT;
	
		}
	
		if(i == 'r')
		{
			if(vertical) {for(i = length - 1; i >= 0; i--) board[x][y - i] -= 2;} else {for(i = 0; i <= length - 1; i++) board[x + i][y] -= 2;}
			
			
			if(vertical && x > (10 - length))
			{
				x = 10 - length;
			} else if (!(vertical) && y < length - 1)
			{
				y = length - 1;
			}
			i = 'r';
			

			//change value of vertical variable at very end
			if(vertical) {vertical = 0;} else {vertical = 1;}
		}
	
		if(i == ' ')
		{
			collision = 0;
		
			//different tests for vert and horz ships
			if(vertical)
			{
				for(i = length - 1; i >= 0; i--)
				{
					if(board[x][y - i] > 3) collision = 1;
				}
			} else {
				for(i = 0; i <= length - 1; i++)
				{
					if(board[x + i][y] > 3) collision = 1;
				}
			}

			//either place or reset values		
			if(collision == 0)
			{
				//UPDATE "SHIP IDENTITY ARRAY"
				i = 0;
				//looking for first available ship encoding space (length will never be 0)
				while(codes[i]) i += 4;
				codes[i] = length;
				codes[i + 1] = x;
				codes[i + 2] = y;
				codes[i + 3] = vertical;
				return;
			} else {
				if(vertical) for(i = length - 1; i>= 0; i--) board[x][y - i] -= 2;
				if(!(vertical)) for(i = 0; i <= length - 1; i++) board[x + i][y] -= 2;
				i = ' ';
			}
		}
		
		if(i != KEY_UP && i != KEY_DOWN && i != KEY_LEFT && i != KEY_RIGHT && i != 'r' && i != ' ')
		{
			if(vertical) for(i = length - 1; i >= 0; i--) board[x][y - i] -= 2;
			if(!(vertical)) for(i = 0; i <= length - 1; i++) board[x + i][y] -=2;
		}
	}
}

void random_ship(int board[10][10], int codes[20], int length)
{
	//no visual component needed.
	//generate random ship code, check for collisions, update.
	int x, y, vertical, i, collision;

	for(;;)
	{
		//commence randomness
		x = rand() % 10;
		y = rand() % 10;
		vertical = rand() % 2;

		//sanitize random output so it always fits within the board
		if(vertical)
		{
			if(y < (length - 1)) y = length - 1;
		} else {
			if(x > (10 - length)) x = 10 - length;
		}

		//check for collision if no collision, update codes and return. if collision, reroll
		collision = 0;
		if(vertical)
		{
			for(i = length - 1; i >= 0; i--) if(board[x][y - i]) collision = 1;
		} else {
			for(i = 0; i <= length - 1; i++) if(board[x + i][y]) collision = 1;
		}

		if(vertical && !(collision))
		{
			for(i = length - 1; i >= 0; i--) board[x][y - i] = 9;
			i = 0;
			while(codes[i]) i += 4;
			codes[i] = length;
			codes[i + 1] = x;
			codes[i + 2] = y;
			codes[i + 3] = vertical;
			return;
		} else if (!(vertical) && !(collision)){
			for(i = 0; i <= length - 1; i++) board[x + i][y] = 9;
			i = 0;
			while(codes[i]) i += 4;
			codes[i] = length;
			codes[i + 1] = x;
			codes[i + 2] = y;
			codes[i + 3] = vertical;
			return;
		}
	}
}

int ship_number(int board[10][10], int codes[20])
{
	//figure out how many ships are left based on codes and game data matrix
	//codes array will be full every time (same process 5 times)
	//x, y, for array coords, i for iterating through them, safe as our test variable, intact for keeping track of ships.
	int x, y, i, safe, intact_ships;

	intact_ships = 0;

	for(int j = 0; j <= 16; j+=4)
	{
		//set safe value to 0 for each loop iteration
		safe = 0;

		x = codes[j+1];
		y = codes[j+2];
		
		//if vertical
		if(codes[j+3])
		{
			for(i = codes[j] - 1; i >= 0; i--) if(board[x][y - i] == 2 || board[x][y - i] == 9) safe = 1;
		} else {
			for(i = 0; i <= codes[j] - 1; i++) if(board[x + i][y] == 2 || board[x + i][y] == 9) safe = 1;
		}
		if(safe) intact_ships++;
	}
	return intact_ships;
}

void print_game(int top_board[10][10], int bottom_board[10][10], int top_ship_codes[20], int bottom_ship_codes[20])
{
	print_board(top_board, 0, 0);
	move(14, 0); printw("-- v YOU v ----- ^ AI ^ --");
	print_board(bottom_board, 16, 0);
						
	move(2, 25); printw("AI ships left: %d", ship_number(top_board, top_ship_codes));
	move(18, 25); printw("Player ships left: %d", ship_number(bottom_board, bottom_ship_codes));
	move(20,25); printw("' .' = empty space/hidden ship space");
       	move(21,25); printw("'##' = missed shot");
	move(22,25); printw("'><' = hit shot");
	move(23,25); printw("'[]' = undamaged ship space (player only)");
	return;
}


void shot_select(int top_board[10][10], int bottom_board[10][10], int top_ship_codes[20], int bottom_ship_codes[20], int shot_codes[10])
{

	//need x and y to keep track of cursor position and to keep it on the board.
	//need letter to have a value to test for keyboard presses
	int x, y, i, fail;
	x = 0;
	y = 9;
	int letter;
	
	//reset shots for each call of the function.
	for(i = 0; i <= 9; i++) shot_codes[i] = 0;

	for(;;)
	{
		clear();
	
		//print game
		print_game(top_board, bottom_board, top_ship_codes, bottom_ship_codes);

		//print shot list
		move(4, 25);
		printw("Selected spaces for firing:");
		for(i = 0; i <= ship_number(bottom_board, bottom_ship_codes) - 1; i++)
		{
			move(5+i, 25);

			if(shot_codes[i*2])
			{
				//Yes, I am aware this could just be a couple lines and have the y value manipulate letter with an ascii table
				//But I don't want to mess with that just yet
				//Also, this works just fine.
				switch(shot_codes[(i*2)+1] - 1)
				{
					case 0: letter = 'A'; break;
					case 1: letter = 'B'; break;
					case 2: letter = 'C'; break;
					case 3: letter = 'D'; break;
					case 4: letter = 'E'; break;
					case 5: letter = 'F'; break;
					case 6: letter = 'G'; break;
					case 7: letter = 'H'; break;
					case 8: letter = 'I'; break;
					case 9: letter = 'J'; break;
					default: letter = '?'; break;
				}
				printw("%d, %c", shot_codes[i*2] - 1, letter);
			} else {
				printw("--, --");	
			}
		}
		move(11,25); printw("Arrow keys to move, space to add current position to shot list.");
		move(12,25); printw("Press R to reset all shots, or U to undo last shot at any time.");

		//move cursor to bottom left of screen
		move(2 + y, 4 + (2 * x));

		refresh();

		//have cursor movement if statements
		letter = getch();

		if(letter == KEY_UP)
		{
			y--;
			if(y<0) y = 0;
		}
		if(letter == KEY_DOWN)
		{
			y++;
			if(y>9) y = 9;
		}
		if(letter == KEY_LEFT)
		{
			x--;
			if(x<0) x = 0;
		}
		if(letter == KEY_RIGHT)
		{
			x++;
			if(x>9) x = 9;
		}	

		//when all shots placed, ask if player wants to fire.
		//how to test for all shots placed?
		
		//player can fire before a full shot code array by pressing hidden option f.
		if(letter == 'f')
		{
			for(;;)
			{

				clear();
	
				//print game
				print_game(top_board, bottom_board, top_ship_codes, bottom_ship_codes);

				//print shot list
				move(4, 25);
				printw("Selected spaces for firing:");
				for(i = 0; i <= ship_number(bottom_board, bottom_ship_codes) - 1; i++)
				{
					move(5+i, 25);

					if(shot_codes[i*2])
					{
						//Yes, I am aware this could just be a couple lines and have the y value manipulate letter with an ascii table
						//But I don't want to mess with that just yet
						//Also, this works just fine.
						switch(shot_codes[(i*2)+1] - 1)
						{
							case 0: letter = 'A'; break;
							case 1: letter = 'B'; break;
							case 2: letter = 'C'; break;
							case 3: letter = 'D'; break;
							case 4: letter = 'E'; break;
							case 5: letter = 'F'; break;
							case 6: letter = 'G'; break;
							case 7: letter = 'H'; break;
							case 8: letter = 'I'; break;
							case 9: letter = 'J'; break;
							default: letter = '?'; break;
						}
						printw("%d, %c", shot_codes[i*2] - 1, letter);
					} else {
						printw("--, --");	
					}
				}

				move(11, 25); printw("Are you sure you want to fire without using all your shots?");
				move(12, 25); printw("Press [Y]es or [N]o");
				refresh();

				//interactive bit
				letter = getch();
				if(letter == 'y') return;
				if(letter == 'n') break;
			}
		}

		//assume shot_codes is not full, "PROTECT THIS BY TESTING FOR A FULL SHOT LIST BEFORE THIS"
		//or by testing afterwards, it's safe either way, and by putting it afterwards, we remove a weird hanging getch() that messes up flow.
		//inside of the space one to place the shot, test for value of AI board that cursor is on
		if(letter == ' ')
		{
			//if space == 9 or 0 AND shot is not already on the shot list, place shot, otherwise do nothing
			if(top_board[x][y] == 0 || top_board[x][y] == 9)
			{
				fail = 0;
				//testing for duplicate shot
				for(i = 0; i <= ship_number(bottom_board, bottom_ship_codes) - 1; i++)
				{
					if((shot_codes[i*2] == x+1) && (shot_codes[(i*2)+1] == y+1)) fail = 1;
				}

				//place shot
				if(!(fail))
				{	
					i = 0;
					while(shot_codes[i]) i += 2;
					shot_codes[i] = x + 1;
					shot_codes[i+1] = y + 1;
				}
			}
		
		}
		//reset shots prematurely
		if(letter == 'r')
		{
			for(i = 0; i <= 9; i++) shot_codes[i] = 0;
		}
		//undo shot
		if(letter == 'u')
		{
			//if no shots, do nothing
			if(shot_codes[0] == 0) continue;
			
			//find most recent shot
			//start at i = 8 then iterate by -2 until nonzero value is found
			//set those values to 0
			i = ship_number(bottom_board, bottom_ship_codes) * 2 - 1;
			while(shot_codes[i] == 0) i -= 2;
			shot_codes[i] = 0;
			shot_codes[i-1] = 0;
		}	

		if(shot_codes[(ship_number(bottom_board, bottom_ship_codes)*2) - 1])
		{	
			//print hidden menu of options
			for(;;)
			{
				clear();
	
				//print game
				print_game(top_board, bottom_board, top_ship_codes, bottom_ship_codes);

				//print shot list
				move(4, 25); printw("Selected spaces for firing:");
				for(i = 0; i <= ship_number(bottom_board, bottom_ship_codes) - 1; i++)
				{
					move(5+i, 25);

					if(shot_codes[i*2])
					{
						//Yes, I am aware this could just be a couple lines and have the y value manipulate letter with an ascii table
						//But I don't want to mess with that just yet
						//Also, this works just fine.
						switch(shot_codes[(i*2)+1] - 1)
						{
							case 0: letter = 'A'; break;
							case 1: letter = 'B'; break;
							case 2: letter = 'C'; break;
							case 3: letter = 'D'; break;
							case 4: letter = 'E'; break;
							case 5: letter = 'F'; break;
							case 6: letter = 'G'; break;
							case 7: letter = 'H'; break;
							case 8: letter = 'I'; break;
							case 9: letter = 'J'; break;
							default: letter = '?'; break;
						}
						printw("%d, %c", shot_codes[i*2] - 1, letter);
					} else {
						printw("--, --");	
					}
				}

				move(11, 25); printw("All shots used. Press [F]ire, [R]eset all shots, or [U]ndo last shot.");
				refresh();

				//interactive bit
				letter = getch();

				if(letter == 'f')
				{
					return;
				}
				if(letter == 'r')
				{
					for(i = 0; i <= 9; i++) shot_codes[i] = 0;
					break;
				}
				if(letter == 'u')
				{
					i = ship_number(bottom_board, bottom_ship_codes) * 2 - 1;
					while(shot_codes[i] == 0) i -= 2;
					shot_codes[i] = 0;
					shot_codes[i-1] = 0;
					break;
				}
			}		
		}
	}
}

void fire_shots(int shot_codes[10], int target_board[10][10])
{
	//update target board based on shot_codes coordinates
	int i=0;
	while(shot_codes[i])
	{
		switch(target_board[shot_codes[i]-1][shot_codes[i+1]-1])
		{
			//miss
			case 0: target_board[shot_codes[i]-1][shot_codes[i+1]-1] = 1; break;

			//invisible ship hit and visible shit hit
			case 9: target_board[shot_codes[i]-1][shot_codes[i+1]-1] = 3; break;
			case 2: target_board[shot_codes[i]-1][shot_codes[i+1]-1] = 3; break;

			//default case error
			default: target_board[shot_codes[i]-1][shot_codes[i+1]-1] = 1; break;
		}
		if(i == 8) return;
		i+=2;
	} 
}

int is_shot_already_encoded(int test_x, int test_y, int shot_codes[10])
{
	int i;
	for(i = 0; i <= 4; i++)
	{
		if(shot_codes[i])
		{
			if(((shot_codes[i] - 1) == test_x) && ((shot_codes[i+1] - 1) == test_y)) return 1;
		}
	}

	return 0;
}

void generate_ai_shots(int difficulty_level, int shot_codes[10], int bottom_board[10][10], int top_board[10][10], int top_ship_codes[20], int targets[], int hits[])
{
	if(difficulty_level == 2)
	{
		//everything below this point is just for the easy difficulty
		//randomly generate ai shots, based on difficulty.
		int i, j, index, rand_output;	
		//reset shots for each function call
		for(i = 0; i <= 9; i++) shot_codes[i] = 0;
	
		//available will track how many available spaces there are for the computer to shoot at.
		int available = 100;
	
		//see how many spots we can shoot at
		for(j = 0; j <= 9; j++) for(i = 0; i <= 9; i++) if(bottom_board[i][j] == 1 || bottom_board[i][j] == 3) available--;
	
		//prevent float error core dump
		if(available == 0) return;
	
		//big shot encoding loop (1 for each ship left)
		for(index = 0; index < ship_number(top_board, top_ship_codes); index++)
		{
			
			//generate random empty spot value
			rand_output = rand() % available;
	
			for(i = 0; i <= 9; i++) for (j = 0; j <= 9; j++)
			{
				//count along the array, subtracting from random value with each empty space or shot already generated
				//not an empty space, do nothing
				if(bottom_board[i][j] == 1 || bottom_board[i][j] == 3) continue;
	
				//figure out a way to test if the space the loop is on is already encoded...
				if(is_shot_already_encoded(i, j, shot_codes)) continue;
	
				//when the random value hits 0, encode the shot.	
				if(!rand_output)
				{
					//shot encoding
					shot_codes[index*2] = i + 1;
					shot_codes[(index*2)+1] = j + 1;
	
					//end both loops
					i = 10;
				}	
				
				rand_output--;
			}
			
			//after shot is encoded, one less place is available.
			available--;
			//prevent float core dump
			if(available == 0) return;
		
		}
	
		return;
	} else {
		// ------- HARD AI GENERATION ALGORITHM(S) ------- //
			
		int i, j, available, index, rand_output;

		//reset shot codes for each function call
		for(i = 0; i <= 9; i++) shot_codes[i] = 0;

		//targets are hits that are REQUIRED to belong to different ships.
		//hits that are 4 spaces apart or greater in a straight line (x....x) 
		//or, hits that are in any way diagonal from each other (on a different y && x from each other)
	
		//pattern detection and a decision making matrix that mirrors the most psychologically probable and important targets to focus on.
		//??RULES?? GUIDELINES??
		//MOST PLAYERS WILL NOT HAVE THEIR SHIPS TOUCH EACH OTHER
		//MOST PLAYERS WILL NOT UNDERSTAND HOW THIS AI WORKS (OR TAKE THE TIME TO READ IT), SO "DEVELOPER ONLY" OPTIMIZATIONS LIKE RANDOMLY OFFSETTING THE CHECKERBOARD ARE NOT IMPORTANT
		//HUMAN-LIKE DECISIONMAKING BASED ON PATTERNS THE AI OBSERVES

		//if no targets, fire im a random checkboard pattern based off the smallest ship sunk.
		//don't worry about smallest and largest ships alove, checkerboard is fine for our purposes.
	
		//collect all hits into one array
		index = 0;
		for(i = 0; i <= 9; i++) for(j = 0; j <= 9; j++)
		{
			if(bottom_board[i][j] == 3)
			{
				//index iterates AFTER statement is run
				hits[index++] = i + 1;
				hits[index++] = j + 1;
			}		
		}
			
		//analyze hits and group them into targets (only happens when hits exist.)
		//use abs() to determine distance.
		//reset targets and index so bleeding logic errors don't happen
		for(index = 0, i = 0; i <= 19; i++) targets[i] = 0;
		int index2;

		//index = hit index. index2 = target index. (for now)
		while(hits[index])
		{
			//first hit will always encode new target
			if(!index)
			{
				targets[0] = hits[0]; targets[1] = hits[1];
				index+=2;
				continue;
			}	
			
			//for each target that already exists. (for loop)
			for(index2 = 0; targets[index2]; index2 += 3)
			{
				//other targets need to be proved, going along the shot list
				//if a shot can be connected to an existing target, update direction (along same line and within 3 distance WITHOUT misses in between)
				//if a shot COULD be connected to another shot that already has a directional pattern, then ??? probably new target with 0 pattern
				//if a shot can be proven to not be a part of an existing target ship, encode new target
				
				//if different x AND y FOR ALL TARGETS, insta new target encode/outer loop iteration.
				//if a match is scored, move on and analyze the match more
				if(targets[index2] == hits[index] || targets[index2 + 1] == hits[index + 1])
				{

					//if we do have a match, more logic ensues.

					//compare directions from target index2 and hit index to determine pattern
					//compare y values (x values are same). if any tests fail, continue to next target. if a test on all shots fails, we will encode.
					if(targets[index2] == hits[index])
					{
						//make sure distance is less than 4.
						if(abs(targets[index2 + 1] - hits[index + 1]) > 3) continue;

						//make sure no missed shots in between two hits.
						//i used as index, j as flag to fail test.
						if(targets[index2 + 1] > hits[index + 1])
						{
							for(i = hits[index + 1], j = 0; i < targets[index2 + 1] - 1; i++) if(bottom_board[hits[index]-1][i] == 1) j = 1;
						} else if(hits[index + 1] > targets[index2 + 1])
						{
							for(i = targets[index2 + 1], j = 0; i < hits[index + 1] - 1; i++) if(bottom_board[hits[index]-1][i] == 1) j = 1;
						}
						if(j) continue;
						
						//only change pattern if the pattern is 0 on the target hit.
						if(!targets[index2 + 2]) {targets[index2 + 2] = 1;} else {continue;}
					}

					//compare x values (y values are same). if any tests fail, continue to next target. if a test on all shots fails, we will encode.
					if(targets[index2 + 1] == hits[index + 1])
					{
						//make sure distance is less than 4.
						if(abs(targets[index2] - hits[index]) > 3) continue;

						//make sure no missed shots in between two hits.
						//i used as index, j as flag to fail test
						if(targets[index2] > hits[index])
						{
							for(j = 0, i = hits[index]; i < targets[index2] - 1; i++) if(bottom_board[i][hits[index]-1] == 1) j = 1;
						} else if(hits[index] > targets[index])
						{
							for(j = 0, i = targets[index2]; i < hits[index] - 1; i++) if(bottom_board[i][hits[index]-1] == 1) j = 1;
						}
						if(j) continue;

						//only change pattern if the pattern is 0 on the target hit.
						if(!targets[index2 + 2]) {targets[index2 + 2] = 2;} else {continue;}
					}
					
				}
			}
			
			//if we get here, encode new target. no matches were found.
			for(j = 0; targets[j]; j += 3); //get to next available space 
			targets[j] = hits[index]; targets[j + 1] = hits[index + 1];

			//shot has been analyzed enough, time to look at next shot.
			index += 2; 
			continue;
		}
		
		//"NEXT STEP"
		//once target array is initialized, check for completed targets seperately.
		//store the length of "completed" targets into some kind of system so we don't just have to play it safe and assume max length of 5 every time.
		//if the length of a completed target is 1, change the pattern to the opposite and then check again
		//pretty sure its impossible to have a hit surrounded by misses so that SHOULDN'T cause a softlock
	
		//for loop that step through each target, stops when empty target or all targets looked at.
		for(index = 0; targets[index] && index < 19; index += 3)
		{
			//index2 used for length of ship now. we don't check the encoded shot because we know it's a hit.
			index2 = 1;
			//different algorithms activate for horizontal and vertical patterns
			//vertical.
			if(targets[index+2] == 1)
			{
				//start at one off the encoded square and move in one direction.
				for(i = targets[index] - 1, j = targets[index+1];;j++)
				{
					//If Hit, iterate coordinates, add +1 to length again and keep checking
					if(bottom_board[i][j] == 3) {index2++; continue;}

					//If Empty, continue; out target loop.
					if(bottom_board[i][j] == 0) {break;}

					//If Miss, one side successful, start from top going other direction.
					if(bottom_board[i][j] == 2)
					{
						for(i = targets[index] - 1, j = targets[index+1] - 2;; j--)
						{
							if(bottom_board[i][j] == 3) {index2++; continue;}
							if(bottom_board[i][j] == 0) {break;}
							//if both sides have a miss, target is complete.
							//if lengh = 1, switch patterns and try again.
							if(bottom_board[i][j] == 2) 
							{
								if(index2 > 1) {targets[index + 2] = 3; break;}
								if(index2 == 1) {targets[index+2] = 2; index -=3; break;}
							}
						}		
						break;
					}
				}
				//goes to next target in loop. no data needs changed.
				if(bottom_board[i][j] == 0 || index2 == 1) continue;
			} 
			//horiztonal.
			else if (targets[index+2] == 2)
			{
				//start at one off the encoded square and move in one direction.
				for(i = targets[index], j = targets[index];; i++)
				{
					//If Hit, iterate coordinates, add +1 to length again and keep checking
					if(bottom_board[i][j] == 3) {index2++; continue;}

					//If Miss, one side successful, start from top going other direction.
					if(bottom_board[i][j] == 0) {break;}

					//If Miss, one side successful, start from top going other direction.
					if(bottom_board[i][j] == 2)
					{
						for(i = targets[index] - 2, j = targets[index+1] - 1;; i--)
						{
							if(bottom_board[i][j] == 3) {index2++; continue;}
							if(bottom_board[i][j] == 0) {break;}
							//if length = 1, switch patterns and try again.
							if(bottom_board[i][j] == 2) 
							{
								if(index2 > 1) {targets[index + 2] = 3; break;}
								if(index2 == 1) {targets[index + 2] = 2; index -=3; break;}
							}

						}
						break;
					}
				}
				//goes to next target in loop. no data needs changed.
				if(bottom_board[i][j] == 0 || index2 == 1) {continue;}
			}

		}


		//"LAST STEP"
		//this entire step needs to be determined by shots left. if at any point we run out of shots, this part should terminate without any trouble.
		//3 patterns ignored, they are complete. shots distributed to 1 and 2 patterns
		//then 0 patterns. randomly generate shots around it based on shots left.
		
		//index2 used to keep track of shots left.
		for(index2 = ship_number(top_board, top_ship_codes); index2 > 0;)
		{
			//index used to keep track of target.
			//find first non-complete, patterned (1 or 2) target.
			for(index = 0; index < 19; index += 3)
			{
				//if already complete, skip to next target.
				if(targets[index+2] == 3) continue;

				//if incomplete pattern, nuke it. (continue pattern until miss).
				if(targets[index+2] == 1 || targets[index+2] == 2)
				{
					//vertical contingency
					if(targets[index+2] == 1)
					{
						//encode shots upward until miss
						for(i = targets[index+1] - 1; bottom_board[targets[index]-1][i] != 1 && i >= 0; i--)
						{
							if(bottom_board[targets[index]-1][i] == 0 || bottom_board[targets[index]-1][i] == 2)
							{ 
								if(is_shot_already_encoded(targets[index]-1, i, shot_codes))
								{
									continue;
								} else {
									for(j = 0; shot_codes[j]; j++); 
									shot_codes[j++] = targets[index];
							        	shot_codes[j] = i+1;
									index2--;
									if(!index2) return;
								}	
							} else if (bottom_board[targets[index]-1][i] == 3) continue;

						}	
						//encode shots downward until miss
						for(i = targets[index+1] - 1; bottom_board[targets[index]-1][i] != 1 && i <= 9; i++)
						{
							if(bottom_board[targets[index]-1][i] == 0 || bottom_board[targets[index]-1][i] == 2)
							{
								if(is_shot_already_encoded(targets[index]-1, i, shot_codes))
								{
									continue;
								} else {
									for(j = 0; shot_codes[j]; j++);
									shot_codes[j++] = targets[index];
									shot_codes[j] = i+1;
									index2--;
									if(!index2) return;
								}
							} else if (bottom_board[targets[index]-1][i] == 3) continue;
						}
					}
					//horizontal contingency
					if(targets[index+2] == 2)
					{
						//encode shots left until miss
						for(i = targets[index] - 1; bottom_board[i][targets[index+1]-1] != 1 && i >= 0; i--)
						{
							if(bottom_board[i][targets[index+1]-1] == 0 || bottom_board[i][targets[index+1]-1] == 2)
							{
								if(is_shot_already_encoded(i, targets[index+1]-1, shot_codes))
								{
									continue;
								} else {
									for(j = 0; shot_codes[j]; j++);
									shot_codes[j++] = i + 1;
									shot_codes[j] = targets[index+1];
									index2--;
									if(!index2) return;
								}
							} else if (bottom_board[i][targets[index]-1] == 3) continue;
						}
						//encode shots right until miss
						for(i = targets[index] - 1; bottom_board[i][targets[index+1]-1] != 1 && i <= 9; i++)
						{	
							if(bottom_board[i][targets[index+1]-1] == 0 || bottom_board[i][targets[index+1]-1] == 2)
							{
								if(is_shot_already_encoded(i, targets[index+1]-1, shot_codes))
								{
									continue;
								} else {
									for(j = 0; shot_codes[j]; j++);
									shot_codes[j++] = i + 1;
									shot_codes[j] = targets[index+1];
									index2--;
									if(!index2) return;
								}
							} else if (bottom_board[i][targets[index]-1] == 3) continue;
						}
					}
				}
				//if we have shots left over, repeat this block safely without softlocking.
			}
			
			//if no more 1s or 2s exist, find first 0 pattern
			//shoot around it to determine pattern randomly.
			//index used to keep track of target
			for(index = 0; index < 19; index += 3)
			{
				//if the target is non-patterned, do our test
				if(!targets[index+2])
				{
					//shoot in cardinal directions around target starting with shot directly above initial hit.
					//test to see if shot is encoded and if we have shots left with each encoding section.
					//y+1
					if(bottom_board[targets[index]-1][targets[index+1]] == 0 || bottom_board[targets[index]-1][targets[index+1]] == 2)
					{
						if(is_shot_already_encoded(targets[index]-1, targets[index+1], shot_codes))
						{
							; //do nothing
						} else {
							for(j = 0; shot_codes[j]; j++);
							shot_codes[j++] = targets[index];
							shot_codes[j] = targets[index + 1] + 1;
							index2--;
							if(!index2) return;
						}
					}

					//x+1
					if(bottom_board[targets[index]][targets[index+1]-1] == 0 || bottom_board[targets[index]][targets[index+1]-1] == 2)
					{
						if(is_shot_already_encoded(targets[index], targets[index+1]-1, shot_codes))
						{
							; //do nothing
						} else {
							for(j = 0; shot_codes[j]; j++);
							shot_codes[j++] = targets[index] + 1;
							shot_codes[j] = targets[index + 1];
							index2--;
							if(!index2) return;
						}
					}

					//y-1
					if(bottom_board[targets[index]-1][targets[index+1] - 2] == 0 || bottom_board[targets[index]-1][targets[index+1] - 2] == 2)
					{
						if(is_shot_already_encoded(targets[index]-1, targets[index+1] - 2, shot_codes))
						{
							; //do nothing
						} else {
							for(j = 0; shot_codes[j]; j++);
							shot_codes[j++] = targets[index];
							shot_codes[j] = targets[index + 1] - 1;
							index2--;
							if(!index2) return;
						}
					}
					
					//x-1
					if(bottom_board[targets[index]-2][targets[index+1]-1] == 0 || bottom_board[targets[index]-2][targets[index+1]-1] == 2)
					{
						if(is_shot_already_encoded(targets[index]-2, targets[index+1]-1, shot_codes))
						{
							; //do nothing
						} else {
							for(j = 0; shot_codes[j]; j++);
							shot_codes[j++] = targets[index]-1;
							shot_codes[j] = targets[index + 1];
							index2--;
							if(!index2) return;
						}
					}

				//if target is patterned or complete, skip target.
				} else {continue;}
			}
			//if both of these tests complete with shots left, checkboard.	
		}
		



		//randomly generate checkerboard shots if no targets
		//OR ALL TARGETS IN ARRAY ARE COMPLETE
		//This used to be conditional, but if we get here, we need to execute it and I'm not un-indenting all of this :P
		if(1)
		{
			//see how many squares are available to shoot at
			available = 50;
			//go down each row
			for(i = 0; i <= 9; i++)
			{
				//define behavior for odd and even rows
				if(i % 2) {j = 1;} else {j = 0;}

				//AI only has the the information a human would, I don't let it cheat :)))
				for(; j <= 9; j+=2) if(bottom_board[i][j] == 1 || bottom_board[i][j] == 3) available--;
			}			
			
			if(!available) return;

			//generate random shots, for however many left. convert to index2 based, counting downwards.
			for(; index2 > 0; index2--)
			{
				rand_output = rand() % available;
				
				for(i = 0; i <= 9; i++)
				{
					if(i % 2) {j = 1;} else {j = 0;}
					for(; j <= 9; j+=2)
					{
						if(bottom_board[i][j] == 1 || bottom_board[i][j] == 3) continue;

						if(is_shot_already_encoded(i, j, shot_codes)) continue;

						if(!rand_output)
						{
							//shot encoding
							//index used to find open space
							for(index = 0; shot_codes[index]; index++);
							shot_codes[index++] = i + 1;
							shot_codes[index] = j + 1;
							index2--;
							if(!index2) return;

							//end both loops
							i = 10;
						}

						rand_output--;	
					}
				}
				//after shot is encoded, 1 less place is available
				available--;
				//prevent float core dump
				if(!available) return;
			}
			
			return;
		}	
	
		// ------- HARD AI GENERATION ALGORITHM(S) END ELSE ------- //
	}
}

int main(void)
{
	//initialize screen
	initscr();
	//seed random numbers
	srand(time(NULL));
	//turns off echo from keyboard input
	noecho();
	//turns on the keypad and arrow keys
	keypad(stdscr, true);

	//initialize variables for game boards, cursor positions, etc.
	int x, y, letter;
	x = 0;
	y = 2;
	//default starting level, 2 is easy, 4 is hard.
	int ai_level = 2;

	int player_ship_codes[20] = {0};
	int ai_ship_codes[20] = {0};
	int player_board[10][10] = {0};
	int ai_board[10][10] = {0};
	int shot_codes[10] = {0};
	int hits[35] = {0}; //one extra space to terminate loop. technically never needs to be reset because you'll never have LESS hits than you begin a turn with.
	int ai_targets[20] = {0};
	int game_end = 0;

	// --- MENU LOOP --- //
	while (!game_end)
	{
		//print menu
		//arrow keys to move, enter to select.
		clear();
		move(0,0); printw("Battleship by Gabe (2024)");

		move(2,4); printw("New Game");
		move(4,4); printw("AI Level");
		move(6,4); printw("Rules");
		move(8,4); printw("Quit");
		move(10,0); printw("Arrow keys to move cursor, space to select option");
	
		move(y,0); printw("->");
		refresh();

		//the big line that determines behavior
		letter = getch();

		//"switch" statement for key behavior
		if(letter == 'q') game_end = 1;
		
		if(letter == KEY_DOWN)
		{
			y+=2;
			if(y > 8) y = 8;
		}
		
		if(letter == KEY_UP)
		{
			y-=2;
			if(y < 2) y = 2;
		}
		
		if(letter == ' ')
		{
			switch(y)
			{
				//new game
				case 2:
					// --------- BEGIN MAIN GAME SWITCH SECTION --------- //
					clear();
					//select position of player ships
					//"TO DO:" update place ship function to have a description
					place_ship(player_board, player_ship_codes, 5, 0, 0);
					place_ship(player_board, player_ship_codes, 4, 0, 0);
					place_ship(player_board, player_ship_codes, 3, 0, 0);
					place_ship(player_board, player_ship_codes, 3, 0, 0);
					place_ship(player_board, player_ship_codes, 2, 0, 0);

					//generate random ship placement for ai (biggest to smallest for favorable gen time)
					random_ship(ai_board, ai_ship_codes, 5);
					random_ship(ai_board, ai_ship_codes, 4);
					random_ship(ai_board, ai_ship_codes, 3);
					random_ship(ai_board, ai_ship_codes, 3);
					random_ship(ai_board, ai_ship_codes, 2);
					
					//tests we don't need anymore
					//print_board(player_board,0,0);
					//print_board(ai_board, 0, 30);
					//refresh();
					//getch();

					//repeating game loop
					for(;;)
					{
						//ask for human inputs to start player turn
						clear();
						print_game(ai_board, player_board, ai_ship_codes, player_ship_codes);
						move(12,28); printw("************************************");
						move(13,28); printw("*                                  *");
						move(14,28); printw("* Press any key to start your turn *");
						move(15,28); printw("*                                  *");
						move(16,28); printw("************************************");
						refresh();
						getch();	

						//human selects shots
						shot_select(ai_board, player_board, ai_ship_codes, player_ship_codes, shot_codes);		
						
						//shots update on target board (ai)
						fire_shots(shot_codes, ai_board);	
						
						//test for 0 ai ships, if so, end game
						if(ship_number(ai_board, ai_ship_codes) == 0)
						{
							//print player wins message
							clear();
							print_game(ai_board, player_board, ai_ship_codes, player_ship_codes);
							//message
							move(12,28); printw("************");
							move(13,28); printw("*          *");
							move(14,28); printw("* YOU WIN! *");
							move(15,28); printw("*          *");
							move(16,28); printw("************");
							refresh();
							
							//test for human input, break.
							getch();
							break;
						}
						//ask for human input to generate and fire AI shots
						clear();
						print_game(ai_board, player_board, ai_ship_codes, player_ship_codes);
						move(12,28); printw("**********************************");
						move(13,28); printw("*                                *");
						move(14,28); printw("* Press any key to start AI turn *");
						move(15,28); printw("*                                *");
						move(16,28); printw("**********************************");
						refresh();
						getch();

						//ai chooses shots
						generate_ai_shots(ai_level, shot_codes, player_board, ai_board, ai_ship_codes, ai_targets, hits);

						//shots update on target board (player)
						fire_shots(shot_codes, player_board);

						//test to see if player has 0 ships, if so, end game
						if(ship_number(player_board, player_ship_codes) == 0)
						{
							//print ai wins message
							clear();
							print_game(ai_board, player_board, ai_ship_codes, player_ship_codes);
							//message
							move(12,28); printw("************");
							move(13,28); printw("*          *");
							move(14,28); printw("* AI WINS! *");
							move(15,28); printw("*          *");
							move(16,28); printw("************");
							refresh();

							//test for human input
							getch();
							break;
						}
					}
					
					//reset needed game variables
					for(int i = 0; i <= 9; i++) for(int j = 0; j <= 9; j++) {ai_board[i][j] = 0; player_board[i][j] = 0;}	
					for(int i = 0; i <= 19; i++) {ai_ship_codes[i] = 0; player_ship_codes[i] = 0;}

					// --------- END MAIN GAME SWITCH SECTION --------- //
					break;
				//ai select
				case 4:
					//make sure to make it so the cursor prints where the ai's level is at.
					//cheat way: make the y value of the cursor correspond to ai level.
					//2 = easy, 4 = hard.
					for(;;)
					{
						clear();
						move(0,0);
						printw("Select AI difficulty settings.");
						move(2,4);
						printw("Easy");
						move(4,4);
						printw("Hard");
						move(ai_level,0);
						printw("->");
						refresh();

						//interactive bit
						letter = getch();

						if(letter == KEY_DOWN)
						{
							ai_level += 2;
							if(ai_level > 4) ai_level = 4;
						}
						if(letter == KEY_UP)
						{
							ai_level -=2;
							if(ai_level < 2) ai_level = 2;
						}
						if(letter == ' ') break;
						
					}

					break;
				//rules
				case 6:
					for(;;)
					{
						clear();
						move(0,0);
						printw("Welcome to Battleship! This is my rendition of the classic game in the SALVO mode,");
						move(1,0);
						printw("where you can fire as many shots as you have ships. Use arrow keys and space to selects");
						move(2,0);
						printw("shots, then check for hits. Sink all of your opponent's ships by scoring a hit on each");
						move(3,0);
						printw("section of the ship to win. If all of your ships get sank, you lose!");
						
						//did it like this so the cursor was in the same spot as the menu
						move(5,4);
						printw("Back to Menu");
						move(5,0);
						printw("->");

						refresh();
						
						letter = getch();

						if(letter == ' ') break;
					}
					break;
				//quit
				case 8:
					game_end = 1;
					break;
				deafult:
					break;
		
			//end switch statement with y parameter
			}

		//end if statement for spacebar on main menu
		}
	
	//end while loop
	}

	endwin();

	return 0;
//end void function
}	
