#include "Piece.h"
#include "pieces.h"

Piece::Piece(int piece)
{
    setNewPiece(piece);
}

int Piece::getColor()
{
    return this->color;
}

int Piece::getXPos()
{
    return this->XPos;
}

int Piece::getYpos()
{
    return this->YPos;
}

void Piece::setColor(int color)
{
    this->color = color;
}

void Piece::setXPos(int xPos)
{
    this->XPos = xPos;
}

void Piece::setYPos(int yPos)
{
    this->YPos = yPos;
}

void Piece::setMatriz(int x, int y, int value)
{
    this->matriz[x][y] = value;
}


int Piece::getMatrizValue(int row, int col)
{
    return this->matriz[col][row];
}

void Piece::copyMatriz(int matriz[4][4])
{
    for (int x = 0; x < 4; x++) {
        for (int y = 0; y < 4; y++) {
            this->matriz[x][y] = matriz[x][y];
        }
    }
}

void Piece::copyMatriz(int source[4][4], int dest[4][4])
{
    for (int x = 0; x < 4; x++) {
        for (int y = 0; y < 4; y++) {
            dest[x][y] = source[x][y];
        }
    }
}

void Piece::setNewPiece(int piece)
{
    this->color = piece - 1;
    this->YPos = 0;
    this->setXPos(3);
    switch (piece)
    {
    case 1:
        this->copyMatriz(PIECE_1);
        break;
    case 2:
        this->copyMatriz(PIECE_2);
        break;
    case 3:
        this->copyMatriz(PIECE_3);
        break;
    case 4:
        this->copyMatriz(PIECE_4);
        break;
    case 5:
        this->copyMatriz(PIECE_5);
        break;
    case 6:
        this->copyMatriz(PIECE_6);
        break;
    case 7:
        this->copyMatriz(PIECE_7);
        break;
    default:
        this->copyMatriz(PIECE_1);
    }
}

void Piece::moveVertical()
{
    this->YPos++;
}

void Piece::moveLeft()
{
    this->XPos--;
}

void Piece::moveRight()
{
    this->XPos++;
}

void Piece::rotateRight()
{
    int original[4][4];
    this->copyMatriz(this->matriz, original);
    for (int row = 0; row < GAME_LENGTH_LOGICAL_PIECE; row++) {
        for (int col = 0; col < GAME_LENGTH_LOGICAL_PIECE; col++) {
            this->matriz[col][row] = original[(GAME_LENGTH_LOGICAL_PIECE-1)-row][col];
        }
    }
}

void Piece::rotateLeft()
{
    int original[4][4];
    this->copyMatriz(this->matriz, original);
    for (int row = 0; row < GAME_LENGTH_LOGICAL_PIECE; row++) {
        for (int col = 0; col < GAME_LENGTH_LOGICAL_PIECE; col++) {
            this->matriz[col][row] = original[row][(GAME_LENGTH_LOGICAL_PIECE-1) - col];
        }
    }
}
