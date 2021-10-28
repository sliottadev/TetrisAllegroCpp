#include "LogicalGame.h"
#include "constants.h"
#include <stdlib.h>
#include <iostream>

LogicalGame::LogicalGame(int xpos, int ypos)
{
    this->pxXPos = xpos;
    this->pxYPos = ypos;
    for (int col = 0; col < GAME_MATRIX_COL; col++) {
        for (int row = 0; row < GAME_MATRIX_ROW; row++) {
            this->logicMatriz[col][row] = GAME_MATRIX_INITIAL_VALUE;
        }
    }
}

void LogicalGame::initLinesComplete()
{
    for (int i = 0; i < 17; i++) {
        this->linesComplete->leftTransition = -1;
    }
}

void LogicalGame::mergePiece(Piece *p)
{
    for (int pcol = 0; pcol < GAME_LENGTH_LOGICAL_PIECE; pcol++) {
        for (int prow = 0; prow < GAME_LENGTH_LOGICAL_PIECE; prow++) {
            if (p->getMatrizValue(pcol,prow)==1)
                this->logicMatriz[p->getXPos() + pcol][p->getYpos() + prow-1] = p->getColor();
        }
    }
}

bool LogicalGame::checkColision(Piece *p, int movX, int movY)
{
    bool collision = false;

    int pcol = 0;
    int prow = 0;
    //Primero verifico que la posicion siguiente no sobrepase los limites
    //si movX=0 entonces solo esta moviendo en vertical y tengo que verificar que no toque piso
    if (movX == 0) {
        int row = (GAME_LENGTH_LOGICAL_PIECE - 1) - ((GAME_MATRIX_ROW) - (p->getYpos()) );
        std::cout << "row calculado: " << row;
        if (row >= 0) {
            for (int i = 0; i < GAME_LENGTH_LOGICAL_PIECE; i++) {
                if (p->getMatrizValue(i,3- row) != 0)
                    collision = true;
            }
        }
    }
    

    //si movY=0 hay dos caminos, 
    //si es mayor a cero tengo que verificar que no sobrepase la pared derecha
    //si es menor a cero tengo que verificar que no sobrepase la pared izquierda

    //calculo posicion futura de la pieza segun movX y movY (RA)
    //aislo la region que ocuparia la pieza en la matriz logica (RB)
    //comparo las regiones 
    //si en alguna celda existen valores de ambos lados, entonces existe colision
    int icol = p->getXPos() + movX;
    int irow = p->getYpos() + movY-1;
    for (int col = icol; (col < icol + GAME_LENGTH_LOGICAL_PIECE) && !collision; col++) {
        for (int row = irow; (row < irow + GAME_LENGTH_LOGICAL_PIECE) && !collision; row++) {
            if (this->logicMatriz[col][row] != -1) { //si hay un bloque en la matriz logica verifica la pieza
                if (p->getMatrizValue(pcol, prow) == 1) { //si hay un bloque en la pieza, hay colision
                    collision = true;
                }
            }
            prow++;
        }
        pcol++;
        prow = 0;
    }
    
    return collision;
}

int LogicalGame::getBlockValue(int row, int col)
{
    return this->logicMatriz[col][row];
}

void LogicalGame::verifyLines()
{
    for (int row = GAME_MATRIX_ROW - 1; row >= 0; row--) {
        bool isLine = true;
        for (int col = 0; col < GAME_MATRIX_COL; col++) {
            if (this->logicMatriz[col][row] == -1) {
                isLine = false;
                break;
            }
        }
        if (isLine) {
            this->linesComplete[row].leftTransition = COUNT_TRANSITIONS_LINE_COMPLETE;
        }
    }
}

int LogicalGame::isLineComplete(int row)
{
    return (this->linesComplete[row].leftTransition >= 0);
}

int LogicalGame::lineCompleteDownTransitions(int row)
{
    return --this->linesComplete[row].leftTransition;
}

void LogicalGame::popLine(int row)
{
    for (int irow = row; irow > 0; irow--) {
        for (int col = 0; col < GAME_MATRIX_COL; col++)
            this->logicMatriz[col][irow] = this->logicMatriz[col][irow-1];
    }
    this->linesComplete[row].leftTransition = -1;
}
