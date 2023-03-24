#include "CL_Chess.h"

void init_board(void)
{
    memcpy(board, initial_board, sizeof(board));
    memcpy(color, initial_color, sizeof(color));
}

void print_board(void)
{
    char    pieceNames[] = "PNBRQKpnbrqk";
    int     i = 0;

    while (i < 64)
    {
        if ((i & 7) == 0) // if i is a multiple of 8 we have print a separating line
        {
            printf("   +---+---+---+---+---+---+---+---+\n");
            if (i <= 56) // Prints row labels from 1 to 8 
            {
                printf(" %d |", 8 - (((unsigned) i) >> 3));
            }
        }
        if (board[i] == EMPTY)
            printf("   |");
        else 
            printf(" %c |", pieceNames[board[i] + (color[i] == WHITE ? 0 : 6)]);
        if ((i & 7) == 7)
            printf("\n");
        i++;
    }
   printf("   +---+---+---+---+---+---+---+---+\n     a   b   c   d   e   f   g   h\n");
}

int main()
{
    char    user_input[256];
    int     from, dest, i;
    MOVE    moveBuffer[200];
    int     move_count;

    printf("CL-Chess by HB\n");
    printf("\n type D: to print board on screen\n to move type move string (e.g. b1c3, a7a8q)\n to quit type: exit\n");

    side = WHITE;
    computer_side = BLACK;
    init_board();
    while (1)
    {
        // Get user input 
        printf(">> ");
        if (scanf("%s", user_input) == EOF)
            return 1;
        if (!strcmp(user_input, "D"))
        {
            print_board();
            continue ;
        }
        if (!strcmp(user_input, "exit"))
        {
            printf("Goodbye, thankyou for playing\n");
            return 0;
        }
    }

}