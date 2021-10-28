#pragma once
#include "Piece.h"

struct LinesComplete{
	int line;
	int leftTransition;
};
class LogicalGame
{
private:
	int logicMatriz[10][17];
	int pxXPos;
	int pxYPos;
	LinesComplete linesComplete[17];
public:
	LogicalGame(int xpos, int ypos);
	void initLinesComplete();
	void mergePiece(Piece *p);
	bool checkColision(Piece *p, int movX, int movY);
	int getBlockValue(int row, int col);
	void verifyLines();
	int isLineComplete(int); //return -1 NO, >-1 left animation
	int lineCompleteDownTransitions(int);
	void popLine(int);
};

