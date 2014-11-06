#pragma once

#include "Button.h"
#include "Hashing.h"

enum TileType {TILE_NORMAL,TILE_DOUBLEWORD,TILE_TRIPLEWORD,TILE_DOUBLELETTER,TILE_TRIPLELETTER};
typedef struct {
    char letter;      //current letter at board position
    TileType type;    //type of the tile eg: doubleword, tripleword
	bool scored;      //bool to check if the score of this letter has been taken care of
} Tile;

class Board
{
public:
	Board();
	~Board();

	//networking values
	int isMultiplayer;
	int amIHost;
	int myPlayer;

	//logic values
	Tile tempstr[15][15];

	int countmoves;
	int tilesplaced;

	Tile tempstring[15];
	int counter;

	//globals
	Tile board[15][15];
	sf::RectangleShape rectshapes[15][15];
	int turn;
	char Hand[2][7];
	int Pass[2];
	int Shuffle[2];
	int Score[2];
	int LetterPile[26];

	//GUI values
	char selection;
	std::string text;

	//Buttons
	Button SubmitButton;
	Button PassTurnButton;
	Button ShuffleHandButton;
	Button ReturnButton;

	void Submit();
	void PassTurn();
	void ShuffleHand();
	void PlaceTile(int x,int y,char c,bool isMyPlayer);

	int takeFromPile();
	void initializeRectShapes();

	void receivePacket(sf::Packet& p);
	int handleEvent(sf::Event& event);
	void draw(sf::RenderWindow& window,int x,int y);

	Tile tileat(int x,int y);

	int getScore();
	int traverseWord(int c, int r, char dir);
	int scoreWord(Tile word[15]);
};

