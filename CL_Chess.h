#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>

// Some Definitions 

#define PAWN        0
#define KNIGHT      1
#define BISHOP      2
#define ROOK        3
#define QUEEN       4
#define KING        5
#define EMPTY       6

#define WHITE       0
#define BLACK       1

// Defining Values of Pieces 

#define VALUE_PAWN      100
#define VALUE_KNIGHT    300
#define VALUE_BISHOP    300
#define VALUE_ROOK      500
#define VALUE_QUEEN     900
#define VALUE_KING      10000

#define MATE            10000 // Equivalent to king, losing king == mate

// Board Representation 

int         initial_board[64] = 
{
    ROOK, KNIGHT, BISHOP, QUEEN, KING, BISHOP, KNIGHT, ROOK,
    PAWN, PAWN, PAWN, PAWN, PAWN, PAWN, PAWN, PAWN,
    EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY,
    EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY,
    EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY,
    EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY,
    PAWN, PAWN, PAWN, PAWN, PAWN, PAWN, PAWN, PAWN,
    ROOK, KNIGHT, BISHOP, QUEEN, KING, BISHOP, KNIGHT, ROOK,
};

int     board[64];
int     color[64];
int     side; // Side to move [Black or White]
int     computer_side = -1;

int     initial_color[64] =
{
    BLACK, BLACK, BLACK, BLACK, BLACK, BLACK, BLACK, BLACK,
    BLACK, BLACK, BLACK, BLACK, BLACK, BLACK, BLACK, BLACK,
    EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY,
    EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY,
    EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY,
    EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY,
    WHITE, WHITE, WHITE, WHITE, WHITE, WHITE, WHITE, WHITE,
    WHITE, WHITE, WHITE, WHITE, WHITE, WHITE, WHITE, WHITE,
};

// STRUCTS

typedef struct s_move
{
    int     from;
    int     dest;
    int     type;
}       MOVE;

