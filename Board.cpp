#include "Board.h"

// letter values are   A B C D E  F G H I J K L M N O P Q  R S T U V W X Y Z
int LetterScore[26] = {1,3,3,2,1 ,4,2,4,1,8,5,1,3,1,1,3,10,1,1,1,1,4,4,8,4,10};

Board::Board()
{
	int i, j;
	text = "";
    for (i=0; i<15; i++)
	{
        for (j=0; j<15; j++)
        {
            board[i][j].type = TILE_NORMAL;
            board[i][j].letter = ' ';
			board[i][j].scored = false;
            if ((i==0 || i==7 || i==14) && (j==0 || j==7 || j==14))     //Triple Word initialization, except for the center tile.
                board[i][j].type = (i==7 && j==7) ? TILE_DOUBLEWORD : TILE_TRIPLEWORD;
            else if ((i==5 || i==9) && ((j-1)%4==0))                    //
                board[i][j].type = TILE_TRIPLELETTER;
            else if ((j==5 || j==9) && ((i-1)%4==0))
                board[i][j].type = TILE_TRIPLELETTER;
            else if ((i==6 || i==8) && (j==2 || j==6 || j==8 || j==12))
                board[i][j].type = TILE_DOUBLELETTER;
            else if ((j==6 || j==8) && (i==2 || i==6 || i==8 || i==12))
                board[i][j].type = TILE_DOUBLELETTER;
            else if ((i==3 || i==11) && (j==0 || j==7 || j==14))
                board[i][j].type = TILE_DOUBLELETTER;
            else if ((j==3 || j==11) && (i==0 || i==7 || i==14))
                board[i][j].type = TILE_DOUBLELETTER;
            else if (i==j || i==(14-j))
                board[i][j].type = TILE_DOUBLEWORD;
        }
	}
	//Board[7][7].scored = true;

	LetterPile[0] = 9;
	LetterPile[1] = 2;
	LetterPile[2] = 2;
	LetterPile[3] = 4;
	LetterPile[4] = 12;
	LetterPile[5] = 2;
	LetterPile[6] = 3;
	LetterPile[7] = 2;
	LetterPile[8] = 9;
	LetterPile[9] = 1;
	LetterPile[10] = 1;
	LetterPile[11] = 4;
	LetterPile[12] = 2;
	LetterPile[13] = 6;
	LetterPile[14] = 8;
	LetterPile[15] = 2;
	LetterPile[16] = 1;
	LetterPile[17] = 6;
	LetterPile[18] = 4;
	LetterPile[19] = 6;
	LetterPile[20] = 4;
	LetterPile[21] = 2;
	LetterPile[22] = 2;
	LetterPile[23] = 1;
	LetterPile[24] = 2;
	LetterPile[25] = 1;

	countmoves = 0;
	tilesplaced = 0;

	turn = 0;
	selection = ' ';
	Shuffle[0] = 1;
	Shuffle[1] = 1;
	Pass[0] = 1;
	Pass[1] = 1;
	Score[0] = 0;
	Score[1] = 0;

	//randomize the hands
	for(int i = 0;i<7;i++)
	{
		for(int j = 0;j<2;j++)
		{
			Hand[j][i] = 65+takeFromPile();
		}
	}

	SubmitButton = Button("Submit",ButtonXOffset,SubmitYOffset);
	PassTurnButton = Button("Pass Turn",ButtonXOffset,PassYOffset);
	ShuffleHandButton = Button("Shuffle Hand",ButtonXOffset,ShuffleYOffset);
	ReturnButton = Button("Main Menu",ButtonXOffset,ReturnYOffset);

	isMultiplayer = false;
	amIHost = false;
	myPlayer = 0;

	initializeRectShapes();
}

Board::~Board()
{
}

void Board::Submit()
{
	int turnscore = getScore();
		if(turnscore>=0)
		{
			//submit code here
			for(int i = 0;i<7;i++)
			{
				if(Hand[turn][i]==' ')
					Hand[turn][i] = 65+takeFromPile();
			}
			Score[turn] += turnscore;
			text = "Player "+std::to_string(turn+1)+" scored "+std::to_string(turnscore)+" points!";
			if(turn==0) turn=1;
			else turn=0;
			countmoves++;
			if(tilesplaced>=7)
			{
				Score[turn]+=50;
			}
			tilesplaced=0;
			for(int i = 0;i<15;i++)
			{
				for(int j = 0;j<15;j++)
				{
					if(board[i][j].letter!=' ') 
						board[i][j].scored = true; //score all words
				}
			}
		}
		else if(turnscore==-1)
		{
			text = "Invalid placement of letters!";
		}
		else if(turnscore==-2)
		{
			text = "That word doesn't exist!";
		}
		else if(turnscore==-3)
		{
			text = "Did you even add a word?";
		}
		else if(turnscore==-4)
		{
			text = "Start from the centre square!";
		}
		if(turnscore>=0)
			Sounds[SOUND_SUBMIT].play();
		else
			Sounds[SOUND_BUTTON].play();
}

void Board::ShuffleHand()
{
	if(Shuffle[turn]>0)
	{
		text = "Player "+std::to_string(turn+1)+" shuffled their hand!";
		Shuffle[turn]--;
		for(int i = 0;i<7;i++)
		{
			if(Hand[turn][i]!=' ')
				Hand[turn][i] = 65+takeFromPile();
		}
		Sounds[SOUND_BUTTON].play();
	}
}

void Board::PassTurn()
{
	if(Pass[turn]>0)
	{
		text = "Player "+std::to_string(turn+1)+" has passed their turn!";
		Pass[turn]--;
		if(turn==0) turn=1;
		else turn=0;
		Sounds[SOUND_BUTTON].play();
	}
}

void Board::PlaceTile(int x,int y,char c,bool isMyPlayer)
{
	//swap selection and letter in hand
	if(c!=' ' && board[y][x].letter==' ') //update tilesplaced
	{
		tilesplaced++;
		Sounds[SOUND_PLACE].play();
	}
	else if(c==' ' && board[y][x].letter!=' ')
	{
		tilesplaced--;
		Sounds[SOUND_PICKUP].play();
	}
	if(isMyPlayer)
	{
		selection = board[y][x].letter;
	}
	board[y][x].letter = c;
}

//gets the X-coordinate of the tile the mouse is positioned over, returns -1 if mouse isn't over any tile
int getSquareX(sf::Event& event)
{
	int endofboard = TileXOffset + 15*TileSize;
	if(event.mouseButton.x < TileXOffset || event.mouseButton.x >= endofboard)
		return -1;
	return (event.mouseButton.x-TileXOffset)/TileSize;
}

//gets the Y-coordinate of the tile the mouse is positioned over, returns -1 if mouse isn't over any tile
int getSquareY(sf::Event& event)
{
	int endofboard = TileYOffset + 15*TileSize;
	if(event.mouseButton.y < TileYOffset || event.mouseButton.y >= endofboard)
		return -1;
	return (event.mouseButton.y-TileYOffset)/TileSize;
}

//gets the position of the letter in the hand if the mouse is positioned over the hand, returns -1 if not
int getHandPosition(sf::Event& event)
{
	if(event.mouseButton.y < HandYOffset || event.mouseButton.y > HandYOffset + LetterSize ||
		event.mouseButton.x < HandXOffset || event.mouseButton.x >= HandXOffset+7*LetterSize+6*HandSeperation)
		return -1;
	return (event.mouseButton.x-HandXOffset)/(LetterSize+HandSeperation);
}

int Board::handleEvent(sf::Event& event)
{
	if(event.type == sf::Event::MouseButtonReleased)
	{
		if(event.mouseButton.button == sf::Mouse::Left)
		{
			int handpos = getHandPosition(event);
			if(handpos!=-1) //is mouse positioned over hand?
			{
				if(!isMultiplayer || myPlayer==turn)
				{
					//swap selection and letter in hand
					char tmp = selection;
					selection = Hand[turn][handpos];
					Hand[turn][handpos] = tmp;
					if(selection!=' ')
					{
						Sounds[SOUND_PICKUP].play();
					}
					else if(Hand[turn][handpos]!=' ')
					{
						Sounds[SOUND_PLACE].play();
					}
					return GOTO_NONE;
				}
			}

			int x = getSquareX(event);
			int y = getSquareY(event);
			if(x!=-1 && y!=-1) //is mouse over the board?
			{
				if(board[y][x].scored==0 || board[y][x].letter==' ')
				{
					if(isMultiplayer)
					{
						sf::Packet packet;
						sf::Int32 t=MESSAGE_PLACETILE;
						sf::Int32 x2 = x;
						sf::Int32 y2 = y;
						sf::Int32 c = selection;
						packet << t << x2 << y2 << c;
						Socket.send(packet);
						packet >> t;
						packet >> x2;
						packet >> y2;
						packet >> c;
					}
					PlaceTile(x,y,selection,true);
			
					return GOTO_NONE;
				}
			}

			if(SubmitButton.checkCollision(event.mouseButton.x,event.mouseButton.y))
			{
				if(!isMultiplayer)
					Submit();
				else if(turn==myPlayer)
				{
					sf::Packet packet;
					sf::Int32 t=MESSAGE_SUBMIT;
					packet << t;
					Socket.send(packet);
					Submit();
				}
			}
			if(ShuffleHandButton.checkCollision(event.mouseButton.x,event.mouseButton.y))
			{
				if(!isMultiplayer)
					ShuffleHand();
				else if(turn==myPlayer)
				{
					sf::Packet packet;
					sf::Int32 t=MESSAGE_SHUFFLE;
					packet << t;
					Socket.send(packet);
					ShuffleHand();
				}
			}
			if(PassTurnButton.checkCollision(event.mouseButton.x,event.mouseButton.y))
			{
				if(!isMultiplayer)
					PassTurn();
				else if(turn==myPlayer)
				{
					sf::Packet packet;
					sf::Int32 t=MESSAGE_PASSTURN;
					packet << t;
					Socket.send(packet);
					PassTurn();
				}
			}
			if(ReturnButton.checkCollision(event.mouseButton.x,event.mouseButton.y))
			{
				Sounds[SOUND_BUTTON].play();
				return GOTO_MENU;
			}
		}
	}
	return GOTO_NONE;
}

void Board::draw(sf::RenderWindow& window,int x,int y)
{
	drawTitle(window);

	//draw the board
	for(int i = 0;i<15;i++)
	{
		for(int j = 0;j<15;j++)
		{
			window.draw(rectshapes[i][j]);

			if(board[i][j].type == TILE_DOUBLEWORD)
			{
				sf::Text t("DOUBLE\nWORD",Font,8);
				t.setPosition(TileXOffset+TileSize*j+TextXOffset,TileYOffset+TileSize*i+TextYOffset);
				t.setColor(sf::Color::Black);
				window.draw(t);
			}
			else if(board[i][j].type == TILE_TRIPLEWORD)
			{
				sf::Text t("TRIPLE\nWORD",Font,8);
				t.setPosition(TileXOffset+TileSize*j+TextXOffset,TileYOffset+TileSize*i+TextYOffset);
				t.setColor(sf::Color::Black);
				window.draw(t);
			}
			else if(board[i][j].type == TILE_DOUBLELETTER)
			{
				sf::Text t("DOUBLE\nLETTER",Font,8);
				t.setPosition(TileXOffset+TileSize*j+TextXOffset,TileYOffset+TileSize*i+TextYOffset);
				t.setColor(sf::Color::Black);
				window.draw(t);
			}
			else if(board[i][j].type == TILE_TRIPLELETTER)
			{
				sf::Text t("TRIPLE\nLETTER",Font,8);
				t.setPosition(TileXOffset+TileSize*j+TextXOffset,TileYOffset+TileSize*i+TextYOffset);
				t.setColor(sf::Color::Black);
				window.draw(t);
			}
		}
	}

	//draw all the letters on the board
	for(int i = 0;i<15;i++)
	{
		for(int j = 0;j<15;j++)
		{
			if(board[i][j].letter!=' ')
			{
				drawLetter(TileXOffset+LetterXOffset+TileSize*j,TileYOffset+LetterYOffset+TileSize*i,board[i][j].letter,!board[i][j].scored,window);
			}
		}
	}

	//draw the hand
	for(int i = 0;i<7;i++)
	{
		sf::RectangleShape rect(sf::Vector2f(LetterSize,LetterSize));
		rect.setOutlineColor(sf::Color::Black);
		rect.setOutlineThickness(1);
		rect.setFillColor(Colors[COLOR_DISBUTTON]);
		rect.setPosition(HandXOffset+(LetterSize+HandSeperation)*i,HandYOffset);
		window.draw(rect);

		if(isMultiplayer)
		{
			if(Hand[myPlayer][i]!=' ') 
			{
				drawLetter(HandXOffset+(LetterSize+HandSeperation)*i,HandYOffset,Hand[myPlayer][i],false,window); //hand letter
			}
		}
		else
		{
			if(Hand[turn][i]!=' ') 
			{
				drawLetter(HandXOffset+(LetterSize+HandSeperation)*i,HandYOffset,Hand[turn][i],false,window); //hand letter
			}
		}
	}

	//Buttons

	if(Pass[turn]>0)
		PassTurnButton.isDisabled = false;
	else
		PassTurnButton.isDisabled = true;
	if(Shuffle[turn]>0)
		ShuffleHandButton.isDisabled = false;
	else
		ShuffleHandButton.isDisabled = true;

	SubmitButton.draw(window,x,y);
	PassTurnButton.draw(window,x,y);
	ShuffleHandButton.draw(window,x,y);
	ReturnButton.draw(window,x,y);

	//Text
	sf::Text txt(text,Font,16);
	txt.setColor(sf::Color::Black);

	txt.setPosition(750,75);
	if(turn==0)
		txt.setString("Turn: Player 1");
	else 
		txt.setString("Turn: Player 2");
	window.draw(txt);

	std::string s;
	s += "Player 1 Score: ";
	s += std::to_string(Score[0]);
	txt.setString(s);
	txt.setPosition(750,100);
	window.draw(txt);

	s = "Player 2 Score: ";
	s += std::to_string(Score[1]);
	txt.setString(s);
	txt.setPosition(750,125);
	window.draw(txt);

	txt.setPosition(750,175);
	txt.setString(text);
	window.draw(txt);

	//draw the selected letter
	if(selection!=' ')
	{
		drawLetter(x,y,selection,true,window);
	}
}

void Board::initializeRectShapes()
{
	for(int i = 0;i<15;i++)
	{
		for(int j = 0;j<15;j++)
		{
			rectshapes[i][j] = sf::RectangleShape(sf::Vector2f(TileSize,TileSize));
			rectshapes[i][j].setPosition(TileXOffset+TileSize*j,TileYOffset+TileSize*i);
			rectshapes[i][j].setOutlineThickness(3);
			rectshapes[i][j].setOutlineColor(sf::Color::Black);
			if(board[i][j].type==TILE_DOUBLEWORD)
			{
				rectshapes[i][j].setFillColor(Colors[COLOR_DOUBLEWORD]);
			}
			else if(board[i][j].type==TILE_TRIPLEWORD)
			{
				rectshapes[i][j].setFillColor(Colors[COLOR_TRIPLEWORD]);
			}
			else if(board[i][j].type==TILE_DOUBLELETTER)
			{
				rectshapes[i][j].setFillColor(Colors[COLOR_DOUBLELETTER]);
			}
			else if(board[i][j].type==TILE_TRIPLELETTER)
			{
				rectshapes[i][j].setFillColor(Colors[COLOR_TRIPLELETTER]);
			}
			else
			{
				rectshapes[i][j].setFillColor(Colors[COLOR_NORMAL]);
			}
		}
	}
}

int Board::takeFromPile()
{
	int i,sum=0;
	for(i = 0;i<26;i++)
	{
		sum += LetterPile[i];
	}
	if(sum==0) return -1;
	int r = rand()%sum;
	int ptr = 0;
	for(int i = 0;i<26;i++)
	{
		ptr += LetterPile[i];
		if(ptr>r)
			return i;
	}
	return -2;
}

void Board::receivePacket(sf::Packet& p)
{
	sf::Int32 type;
	p >> type;
	if(type==MESSAGE_PLACETILE)
	{
		sf::Int32 x,y,c;
		p >> x;
		p >> y;
		p >> c;
		PlaceTile(x,y,char(c),false);
	}
	if(type==MESSAGE_SUBMIT)
	{
		Submit();
	}
	if(type==MESSAGE_PASSTURN)
	{
		PassTurn();
	}
	if(type==MESSAGE_SHUFFLE)
	{
		ShuffleHand();
	}
}

int getWordMul(TileType t)
{
	if(t==TILE_DOUBLEWORD) return 2;
	if(t==TILE_TRIPLEWORD) return 3;
	return 1;
}

int getLetterMul(TileType t)
{
	if(t==TILE_DOUBLELETTER) return 2;
	if(t==TILE_TRIPLELETTER) return 3;
	return 1;
}

/*This is the part of the code where all of the words are processed at once

The steps for processing all the words are as follows.

1) The word must be longer than 1 letter(Except at the start)
2) There should be atleast one 0 and one 1 in the given string. Two flag variables will indicate this.

At this point, if both conditions are satisfied, then the word is stored into a 2D array to compute.
This process is repeated vertically and horizontally

We then examine the 2D array.
3) At least one of the substrings with Zeroes has to be equal to the tiles placed.(Zeroes are measured here and equated to the tiles placed.) If true
4) Finally, all substrings are evaluated for validity.

If the given evaluation works, then and only then is the score added to the overall score and the board is updated.

Another possible approach was to use matrix subtraction, but that was a bit heavy on the memory than we wanted, and so we decided against it.
*/

Tile Board::tileat(int x,int y)
{
	if(x>=0 && x<15 && y>=0 && y<15) return board[x][y];
	Tile t;
	t.letter = ' ';
	t.scored = false;
	t.type = TILE_NORMAL;
	return t;
}

int Board::getScore()
{
	for(int i = 0;i<15;i++) //initialize
	{
		for(int j = 0;j<15;j++)
		{
			tempstr[i][j].letter = ' ';
			tempstr[i][j].type = TILE_NORMAL;
			tempstr[i][j].scored = false;
		}
		tempstring[i].letter = ' ';
		tempstring[i].type = TILE_NORMAL;
		tempstring[i].scored = false;
	}
	counter = 0;

    int x,y,u,v,m,n,no,flag=0,flagx=0,temp=0,tempscore=0,noo=0;

	if(board[7][7].letter==' ') 
		return -4; //starting square not occupied

    if(countmoves==0)  // Starting condition
	{
		for(x=0;x<15;x++)  
		{
			for(y=0;y<15;y++)
			{
				if(board[x][y].letter!=' ')    //First letter encountered of the game.  .
				{
					if(y<14 && board[x][y+1].letter!=' ')
					   y+=traverseWord(y,x,'S');
					else if(x<14 && board[x+1][y].letter==' ')
						if(x>0 && board[x-1][y].letter==' ')
							return -1; //single letter found
				}
			}
		}
		for(y=0;y<15;y++)
		{
			for(x=0;x<15;x++)
			{
				if(board[x][y].letter!=' ')    //First letter encountered of the game.  .
				{
					if(x<14 && board[x+1][y].letter!=' ')
					   x+=traverseWord(y,x,'S');
					else if(y<14 && board[x][y+1].letter==' ')
						if(y>0 && board[x][y-1].letter==' ')
							return -1; //single letter found
				}
			}
		}
	}
	else
	{
		for(x=0;x<15;x++)  // Processing all words Horizontally
		{
			for(y=0;y<14;y++)
			{          
				if(board[x][y].letter!= ' ')
                {
					if(board[x][y+1].letter!=' ')
					{
						temp=traverseWord(y,x,'H');
						if(temp!=-1)
							y+=temp;
                    }
					else if(x<14 && board[x+1][y].letter==' ')
					{
						if(x>0 && board[x-1][y].letter==' ')
							return -1; //single letter found
						else if(x==0)
							return -1;
					}
					else if(x==14)
					{
						if(board[x-1][y].letter==' ')
							return -1;
					}
                }
			}
		}
    
		for(u=0;u<15;u++)  //Processing all words vertically
		{
			for(v=0;v<14;v++)
			{
				if(board[v][u].letter!= ' ')
				{
					if(board[v+1][u].letter!=' ')
					{
						temp=traverseWord(u,v,'V');
						if(temp!=-1)
							v+=temp;
					}
					else if(u<14 && board[v][u+1].letter==' ')
					{
						if(y>0 && board[v][u-1].letter==' ')
							return -1; //single letter found
						else if(y==0)
							return -1;
					}
					else if(u==14)
					{
						if(board[v][u-1].letter==' ')
							return -1;
					}
				}
			}
		}
	}

	if(counter==0)
		return -3; //no words added

    for(m=0;m<counter;m++)  // Checking for the zeroes being equal to Tiles placed
    {
		noo=0;
        for(n=0;n<15;n++)
        {
			if(tempstr[m][n].letter==' ') break;
            if(tempstr[m][n].scored==false)
                noo++;
        }
		if(noo==tilesplaced)
        {
            flag=1;
            break;
        }
    }
    if(flag)    // Final evaluation of strings.
    {
		flagx=1;
		for(m=0;m<counter;m++)
		{
			int has1 = 0;
			int has0 = 0;
			for(n=0;n<15;n++)
			{
				tempstring[n]=tempstr[m][n];
				/*if(tempstr[m][n].scored==true)
					has1 = 1;
				if(tempstr[m][n].scored==false)
					has0 = 1;*/
				if(tempstr[m][n].letter==' ') break;
			}
			//tempstring[n+1].letter=' ';
			//if((has0 && has1) || countmoves==0)
			//{
				int wordscore = scoreWord(tempstring);
				if(wordscore==-1)
				{
					flagx=0;
					break;
				}
				else
				{
					tempscore += wordscore;
				}
			//}
        }
    }
	else 
		return -1; //invalid placement
    if(flagx)
	{
		if(tempscore!=0)
			return tempscore;
		return -2; //invalid word added
	}
    else
		return -2; //invalid word added
}

int Board::traverseWord(int c, int r, char dir)  // Traversing word
{
    int p=r,q=c,n=0,n1=0,n2=0,flag1=0,flag2=0;
    if(dir=='S')   // Word is either entered horizontally or vertically
    {
        if(r<14 && board[r+1][c].letter!=' ')
        {
            n1=0;
            while(board[p][q].letter!=' ')
			{
				tempstr[counter][n1]=board[p][q];
				n1++;
				n++;
				p++;
				if(p>=15) break;
			}
			counter++;
		}
		else if(c<14 && board[r][c+1].letter!=' ')
		{
			n1=0;
			while(board[p][q].letter!=' ')
			{
				tempstr[counter][n1]=board[p][q];
				n1++;
				n++;
				q++;
				if(q>=15) break;
			}
			counter++;
		}
		else 
			return 0;
    }
    else if(dir=='H')   //Horizontal
    {
        n=0,n1=0;
		while(board[p][q].letter!=' ')
		{
			if(board[p][q].scored==false)
				flag1=1;
            if(board[p][q].scored==true)
				flag2=1;
            q++;
            n++;
			if(q>=15) break;
        }
        if(flag1 && flag2)
        {
			while(board[r][c].letter!=' ')
			{
				tempstr[counter][n1]=board[r][c];
				n1++;
				c++;
				if(c>=15) break;
			}
			counter++;
		}
		else if(flag1 && !flag2)
		{
			int checkflag = 0;
			p=r,q=c;
			while(board[p][q].letter!=' ')
			{
				if(p<14 && board[p+1][q].scored==true)
				{
					checkflag = 1;
					break;
				}
				else if(p>0 && board[p-1][q].scored==true)
				{
					checkflag = 1;
					break;
				}
				q++;
				if(q>=15) break;
			}
			if(checkflag==1)
			{
				while(board[r][c].letter!=' ')
				{
					tempstr[counter][n2]=board[r][c];
					n2++;
					c++;
					if(c>=15) break;
				}
				counter++;
			}
		}
    }
    else if(dir=='V')   //Vertical
    {
		n2 = 0;
        while(board[p][q].letter!=' ')
        {
			if(board[p][q].scored==false)
                flag1=1;
			if(board[p][q].scored==true)
                flag2=1;
            p++;
            n++;
			if(p>=15) break;
        }
        if(flag1 && flag2)
        {
			while(board[r][c].letter!=' ')
			{
				tempstr[counter][n2]=board[r][c];
				n2++;
				r++;
				if(r>=15) break;
			}
			counter++;
		}
		else if(flag1 && !flag2)
		{
			int checkflag = 0;
			p=r,q=c;
			while(board[p][q].letter!=' ')
			{
				if(q<14 && board[p][q+1].scored==true)
				{
					checkflag = 1;
					break;
				}
				else if(q>0 && board[p][q-1].scored==true)
				{
					checkflag = 1;
					break;
				}
				p++;
				if(p>=15) break;
			}
			if(checkflag==1)
			{
				while(board[r][c].letter!=' ')
				{
					tempstr[counter][n2]=board[r][c];
					n2++;
					r++;
					if(r>=15) break;
				}
				counter++;
			}
		}
    }
	else 
		return 0;
	if(n==0) return 0;
	return n-1;
}

int Board::scoreWord(Tile word[15])
{
    char tempword[17];
    int tempscore, wordmult=1, index,i=0;
    tempscore = 0;
	for(i = 0;i<15;i++)
	{
		if(word[i].letter==' ') break;
		tempword[i]=word[i].letter;
	}

    tempword[i]=0;

	struct word wordstruct;
	for(i = 0;i<17;i++)
	{
		wordstruct.tempw[i] = tempword[i];
	}

    if(!searchhash(wordstruct))
    {
        return -1;
    }
    else
    {
		for(int i = 0;i<15;i++)
		{
			if(word[i].letter==' ') break;
			index = word[i].letter - 'A';  // find zero-based index for alpha tile score.
		
			      // new tiles get letter and/or word multiplier, if any
			if(word[i].scored==false)
			{
				tempscore += LetterScore[index] * getLetterMul(word[i].type);
				wordmult *= getWordMul(word[i].type);  //Saving the new multiplier into a temporary variable.
			}
			else 
				tempscore += LetterScore[index];
		}
    }    
	// calculate final word score
    return (tempscore * wordmult);
}
