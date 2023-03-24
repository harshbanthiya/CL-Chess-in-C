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

#define COL(pos)        ((pos)&7)
#define ROW(pos)        (((unsigned)pos)>>3)

// Some Board Constants for ease of reading 

#define ONE_RANK (8)
#define TWO_RANKS (16)
#define UP (-8)
#define DOWN (8)
#define LEFT (-1)
#define RIGHT (1)
#define RIGHT_UP (RIGHT+UP)
#define LEFT_UP (LEFT+UP)
#define RIGHT_DOWN (RIGHT+DOWN)
#define LEFT_DOWN (LEFT+DOWN)

// For Generating Moves 
#define MOVE_TYPE_NONE                      0
#define MOVE_TYPE_NORMAL                    1
#define MOVE_TYPE_PROMOTION_TO_QUEEN        4
#define MOVE_TYPE_PROMOTION_TO_ROOK         5
#define MOVE_TYPE_PROMOTION_TO_BISHOP       6
#define MOVE_TYPE_PROMOTION_TO_KNIGHT       7

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

int     board[64];
int     color[64];
int     side; // Side to move [Black or White]
int     computer_side = -1;
int     max_depth = 5;



// STRUCTS

typedef struct s_move
{
    int     from;
    int     dest;
    int     type;
}       MOVE;

typedef struct s_history // move history
{
    MOVE    m;
    int     cap;
}   HISTORY;

/* For Searching */
int     nodes; // Count of all visited nodes for seaching 
int     ply;  
/*
    a "ply" refers to one move made by one player, and a "two-ply search" would involve considering two moves ahead, 
    i.e., both the player's move and the opponent's response. In the context of computer science, 
    "ply" may also refer to the depth of a search algorithm or decision tree, where each ply represents a level of analysis or computation.
*/

/* For Storing */

HISTORY hist[6000];  // Game length < 6000
int     hdp;         // Current move order 

