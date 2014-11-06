#ifndef CONSTANTS_H
#define CONSTANTS_H

//change these values at your peril!

//window dimensions
const int WindowWidth  = 1200;
const int WindowHeight = 720;

///BOARD CONSTANTS
//Tiles
const int TileSize = 40; //Length of a tile
const int TileXOffset = 100; //Board X offset
const int TileYOffset = 40;  //Board Y offset

//Letters
const int LetterSize = 30; //Length of a letter tile
const int LetterXOffset = 4;  //Letter offset relative to the tile
const int LetterYOffset = 4;

//Letter and Tile text
const int TextXOffset = 5; //Text offset relative to the letter
const int TextYOffset = 5;

//Notation
const int NotationOffset = 13;

//Letter Score
const int LetterScoreXOffset = 15; //Score offset relative to the letter
const int LetterScoreYOffset = 15;

//Hand
const int HandSeperation = 3; //Seperation between 2 letters in the hand
const int HandXOffset = TileXOffset+(TileSize*15)/2-((LetterSize+HandSeperation)*7)/2; //Hand position
const int HandYOffset = TileYOffset+(TileSize*15)+10;

//Buttons
const int ButtonXOffset = 750;

const int SubmitYOffset = 300;
const int PassYOffset = 350;
const int ShuffleYOffset = 400;
const int SaveGameYOffset = 450;
const int ReturnYOffset = 500;

///MENU CONSTANTS
//Title Position
const int TitleXOffset = TileXOffset+(TileSize*15)/2-LetterSize*4;
const int TitleYOffset = 100;

//Buttons
const int MenuItemXOffset = TileXOffset+(TileSize*15)/2-LetterSize*4 + 50;

const int MenuItemHeight = 25;
const int MenuItemWidth = (LetterSize*8) - 100;

const int NewGameYOffset = 150;
const int ContinueGameYOffset = 200;
const int MultiplayerYOffset = 250;
const int LoadGameYOffset = 300;
const int ExitYOffset = 300;


#endif