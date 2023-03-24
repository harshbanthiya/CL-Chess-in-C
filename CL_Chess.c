#include "CL_Chess.h"

// Abstracted Move Generator 
void    gen_push(int from, int dest, int type, MOVE *pBuf, int *pMCount)
{
    MOVE    move;
    move.from = from;
    move.dest = dest;
    move.type = type;
    // Update the moves buffer
    pBuf[*pMCount] = move;
    *pMCount = *pMCount + 1;
}

void    gen_push_normal(int from, int dest, MOVE *pBuf, int *pMCount)
{
    gen_push(from, dest, MOVE_TYPE_NORMAL, pBuf, pMCount);
}

// Pawn moves generate with promotion 
void    gen_push_pawn(int from, int dest, MOVE *pBuf, int *pMCount)
{
    if (dest > 7 && dest < 56)
        gen_push(from, dest, MOVE_TYPE_NORMAL, pBuf, pMCount);
    else 
    {
        gen_push(from, dest, MOVE_TYPE_PROMOTION_TO_QUEEN, pBuf, pMCount);
        gen_push(from, dest, MOVE_TYPE_PROMOTION_TO_ROOK, pBuf, pMCount);
        gen_push(from, dest, MOVE_TYPE_PROMOTION_TO_BISHOP, pBuf, pMCount);
        gen_push(from, dest, MOVE_TYPE_PROMOTION_TO_KNIGHT, pBuf, pMCount);
    }
}

// Generate all the moves of current_side to move and push them to pBuf and return number of moves 
int generate_all_moves(int current_side, MOVE *pBuf)
{
    int     i, k, y, row, col, movecount;
    movecount = 0;

    for (i = 0; i < 64; i++)
    {
        if (color[i] == current_side)
        {
            switch (board[i])
            {
                case PAWN:
                    col = COL(i); 
                    row = ROW(i);
                    if (current_side == BLACK)
                    {
                        if (color[i + ONE_RANK] == EMPTY) // check if there is an empty square infront of pawn's current rank
                            gen_push_pawn(i, i + ONE_RANK, pBuf, &movecount);
                        if (row == 1 && color[i + ONE_RANK] == EMPTY && color[i + TWO_RANKS] == EMPTY) // check if pawn is at starting rank and two sqaures ahead are empty
                            gen_push_normal(i, i + TWO_RANKS, pBuf, &movecount);
                        if (col && color[i + 7] == WHITE) // This if and the next if check if Whites pieces are either on diagonal left and right and available to capture
                            gen_push_normal(i, i + 7, pBuf, &movecount);
                        if (col < 7 && color[i + 9] == WHITE)
                            gen_push_normal(i, i + 9, pBuf, &movecount);
                    }
                    else // WHITE ; Same logic with different offsets and starting ranks 
                    {
                        if (color[i - ONE_RANK] == EMPTY) // check if there is an empty square infront of pawn's current rank
                            gen_push_pawn(i, i - ONE_RANK, pBuf, &movecount);
                        if (row == 6 && color[i - ONE_RANK] == EMPTY && color[i - TWO_RANKS] == EMPTY) // check if pawn is at starting rank and two sqaures ahead are empty
                            gen_push_normal(i, i - TWO_RANKS, pBuf, &movecount);
                        if (col && color[i - 9] == BLACK) // This if and the next if check if Black pieces are either on diagonal left and right and available to capture
                            gen_push_normal(i, i - 9, pBuf, &movecount);
                        if (col < 7 && color[i - 7] == BLACK)
                            gen_push_normal(i, i - 7, pBuf, &movecount);
                    }
                break;
                case QUEEN: // Do not need additional logic Queen behaves like bishop + rook, so we let it fall through
                case BISHOP:
                    for (y = i - 9; y >= 0 && COL(y) != 7; y -= 9) // Go left up
                    {
                        if (color[y] != current_side)
                            gen_push_normal(i, y, pBuf, &movecount);
                        if (color[y] != EMPTY)
                            break ;
                    }
                    for (y = i - 7; y >= 0 && COL(y) != 0; y -= 7) // Go right up
                    {
                        if (color[y] != current_side)
                            gen_push_normal(i, y, pBuf, &movecount);
                        if (color[y] != EMPTY)
                            break ;
                    }
                    for (y = i + 9; y < 64 && COL(y) != 0; y += 9) // Go left down
                    {
                        if (color[y] != current_side)
                            gen_push_normal(i, y, pBuf, &movecount);
                        if (color[y] != EMPTY)
                            break ;
                    }
                    for (y = i + 7; y < 64 && COL(y) != 7; y += 7) // Go left down
                    {
                        if (color[y] != current_side)
                            gen_push_normal(i, y, pBuf, &movecount);
                        if (color[y] != EMPTY)
                            break ;
                    }
                    if (board[i] == BISHOP) // this is to let it fall through for the queen
                        break ;
                case ROOK:
                    col = COL(i);
                    for (k = i - col, y = i - 1; y >= k; y--) // Go left
                    {
                        if (color[y] != current_side)
                            gen_push_normal(i, y, pBuf, &movecount);
                        if (color[y] != EMPTY)
                            break ;
                    }
                    for (k = i - col + 7, y = i + 1; y <= k; y++) // Go right
                    {
                        if (color[y] != current_side)
                            gen_push_normal(i, y, pBuf, &movecount);
                        if (color[y] != EMPTY)
                            break ;
                    }
                    for (y = i - 8; y >= 0; y -= 8) // Go up
                    {
                        if (color[y] != current_side)
                            gen_push_normal(i, y, pBuf, &movecount);
                        if (color[y] != EMPTY)
                            break ;
                    }
                    for (y = i + 8; y < 64 ; y += 8) // Go down
                    {
                        if (color[y] != current_side)
                            gen_push_normal(i, y, pBuf, &movecount);
                        if (color[y] != EMPTY)
                            break ;
                    }
                break ;
                case KNIGHT:
                    col = COL(i);
                    y = i - 6;
                    if (y >= 0 && col < 6 && color[y] != current_side)
                        gen_push_normal(i, y, pBuf, &movecount);
                    y = i - 10;
                    if (y >= 0 && col > 1 && color[y] != current_side)
                        gen_push_normal(i, y, pBuf, &movecount);
                    y = i - 15;
                    if (y >= 0 && col < 7 && color[y] != current_side)
                        gen_push_normal(i, y, pBuf, &movecount);
                    y = i - 17;
                    if (y >= 0 && col > 0 && color[y] != current_side)
                        gen_push_normal(i, y, pBuf, &movecount);
                    y = i + 6;
                    if (y < 64 && col > 1 && color[y] != current_side)
                        gen_push_normal(i, y, pBuf, &movecount);
                    y = i + 10;
                    if (y < 64 && col < 6 && color[y] != current_side)
                        gen_push_normal(i, y, pBuf, &movecount);
                    y = i + 15;
                    if (y < 64 && col > 0 && color[y] != current_side)
                        gen_push_normal(i, y, pBuf, &movecount);
                    y = i + 17;
                    if (y < 64 && col < 7 && color[y] != current_side)
                        gen_push_normal(i, y, pBuf, &movecount);
                break ;
                case KING:
                    col = COL(i);
                    if (col && color[i - 1] != current_side) // left
                        gen_push_normal(i, i - 1, pBuf, &movecount);
                    if (col < 7 && color[i + 1] != current_side) // right 
                        gen_push_normal(i, i + 1, pBuf, &movecount);
                    if (i > 7 && color[i - 8] != current_side) // up 
                        gen_push_normal(i, i - 8, pBuf, &movecount);
                    if (i < 56 && color[i + 8] != current_side) // down 
                        gen_push_normal(i, i + 8, pBuf, &movecount);
                    if (col && i > 7 && color[i - 9] != current_side) // left up 
                        gen_push_normal(i, i - 9, pBuf, &movecount);
                    if (col < 7 && i > 7 && color[i - 7] != current_side) // right up 
                        gen_push_normal(i, i - 7, pBuf, &movecount);
                    if (col && i < 56 && color[i + 7] != current_side) // left down  
                        gen_push_normal(i, i + 7, pBuf, &movecount);
                    if (col < 7 && i < 56 && color[i + 9] != current_side) // right down 
                        gen_push_normal(i, i + 9, pBuf, &movecount);
                break ;
                default: 
                    puts("Unknown Chess Piece!");
                    assert(0);   
            }
        }
    }
    return movecount;
}


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
    for (;;)
    {
        if (side == computer_side) // Computers Turn
        {
            // Find best move to react to current position
           // MOVE    best_move = computer_think(max_depth); !IMPLEMENT LATER
           // make_move(best_move);
            continue ;
        }
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
        // now maybe the user has entered the move ? - parse values 
        // Extract source square
        from = user_input[0] - 'a'; //  - number between 0 - 7 / col 
        from += 8 * (8 - (user_input[1] - '0')); // - number between 1 - 8 / row | combined to get full square address

        // Extract Dest Square 
        dest = user_input[2] - 'a';
        dest += 8 * (8 - (user_input[3] - '0'));

        // Print Extracted squares 
        printf("\nfrom: %d to: %d \n", from, dest);

        // Lets set ply to zero and generate all the moves 
        ply = 0;
        move_count = generate_all_moves(side, moveBuffer);

        printf("\n move_counts generated : %d\n", move_count);
    }

}