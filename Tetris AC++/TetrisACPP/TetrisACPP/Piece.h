#pragma once
#include "constants.h"
class Piece
{
private: 
	int color;
	int XPos, YPos;
	int matriz[4][4];
public:
	Piece(int);
	int getColor();
	int getXPos();
	int getYpos();
	void setColor(int);
	void setXPos(int);
	void setYPos(int);

	void setMatriz(int, int, int);
	int getMatrizValue(int, int);
	void copyMatriz(int[4][4]);
	void copyMatriz(int[4][4], int[4][4]);
	void setNewPiece(int);

	void moveVertical();
	void moveLeft();
	void moveRight();

	void rotateRight();
	void rotateLeft();
};

