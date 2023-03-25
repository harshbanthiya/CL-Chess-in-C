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

// Check and return 1 if the current side is in check
int is_in_check(int current_side)
{
    int         k, h, y, row, col, xside;

    xside = (WHITE + BLACK) - current_side; // Opposite of current side whose check we need to work on

    // First lets find  the King 
    for (k = 0; k < 64; k++)
    {
        if (board[k] == KING && color[k] == current_side)
            break ;
    }
    
    row = ROW(k), col = COL(k);

    // Check for attacks from knight 
    if (col > 0 && row > 1 && color[k - 17] == xside && board[k - 17] == KNIGHT)
        return 1;
    if (col < 7 && row > 1 && color[k - 15] == xside && board[k - 15] == KNIGHT)
        return 1;
    if (col > 1 && row > 0 && color[k - 10] == xside && board[k - 10] == KNIGHT)
        return 1;
    if (col < 6 && row > 0 && color[k - 6] == xside && board[k - 6] == KNIGHT)
        return 1;
    if (col > 1 && row < 7 && color[k + 6] == xside && board[k + 6] == KNIGHT)
        return 1;
    if (col < 6 && row < 7 && color[k + 10] == xside && board[k + 10] == KNIGHT)
        return 1;
    if (col > 0 && row < 6 && color[k + 15] == xside && board[k + 15] == KNIGHT)
        return 1;
    if (col < 7 && row < 6 && color[k + 17] == xside && board[k + 17] == KNIGHT)
        return 1;

    // Check horizontal and vertical lines for attacking of Queen, Rook and King
    y = k + 8;
    if (y < 64) {
        if (color[y] == xside && (board[y] == KING || board[y] == QUEEN || board[y] == ROOK))
            return 1;
        if (board[y] == EMPTY)
            for (y += 8; y < 64; y += 8) {
                if (color[y] == xside && (board[y] == QUEEN || board[y] == ROOK))
                    return 1;
                if (board[y] != EMPTY)
                    break;

            }
    }
    // go left 
    y = k - 1;
    h = k - col;
    if (y >= h) {
        if (color[y] == xside && (board[y] == KING || board[y] == QUEEN || board[y] == ROOK))
            return 1;
        if (board[y] == EMPTY)
            for (y--; y >= h; y--) {
                if (color[y] == xside && (board[y] == QUEEN || board[y] == ROOK))
                    return 1;
                if (board[y] != EMPTY)
                    break;
            }
    }
    // go right
    y = k + 1;
    h = k - col + 7;
    if (y <= h) {
        if (color[y] == xside && (board[y] == KING || board[y] == QUEEN || board[y] == ROOK))
            return 1;
        if (board[y] == EMPTY)
            for (y++; y <= h; y++) {
                if (color[y] == xside && (board[y] == QUEEN || board[y] == ROOK))
                    return 1;
                if (board[y] != EMPTY)
                    break;
            }
    }
    // go up 
    y = k - 8;
    if (y >= 0) {
        if (color[y] == xside && (board[y] == KING || board[y] == QUEEN || board[y] == ROOK))
            return 1;
        if (board[y] == EMPTY)
            for (y -= 8; y >= 0; y -= 8) {
                if (color[y] == xside && (board[y] == QUEEN || board[y] == ROOK))
                    return 1;
                if (board[y] != EMPTY)
                    break;
            }
    }
    /* Check diagonal lines for attacking of Queen, Bishop, King, Pawn */
    // go right down
    y = k + 9;
    if (y < 64 && COL(y) != 0) {
        if (color[y] == xside) {
            if (board[y] == KING || board[y] == QUEEN || board[y] == BISHOP)
                return 1;
            if (current_side == BLACK && board[y] == PAWN)
                return 1;
        }
        if (board[y] == EMPTY)
            for (y += 9; y < 64 && COL(y) != 0; y += 9) {
                if (color[y] == xside && (board[y] == QUEEN || board[y] == BISHOP))
                    return 1;
                if (board[y] != EMPTY)
                    break;
            }
    }
    // go left down
    y = k + 7;
    if (y < 64 && COL(y) != 7) {
        if (color[y] == xside) {
            if (board[y] == KING || board[y] == QUEEN || board[y] == BISHOP)
                return 1;
            if (current_side == BLACK && board[y] == PAWN)
                return 1;
        }
        if (board[y] == EMPTY)
            for (y += 7; y < 64 && COL(y) != 7; y += 7) {
                if (color[y] == xside && (board[y] == QUEEN || board[y] == BISHOP))
                    return 1;
                if (board[y] != EMPTY)
                    break;

            }
    }
    // go left up 
    y = k - 9;
    if (y >= 0 && COL(y) != 7) {
        if (color[y] == xside) {
            if (board[y] == KING || board[y] == QUEEN || board[y] == BISHOP)
                return 1;
            if (current_side == WHITE && board[y] == PAWN)
                return 1;
        }
        if (board[y] == EMPTY)
            for (y -= 9; y >= 0 && COL(y) != 7; y -= 9) {
                if (color[y] == xside && (board[y] == QUEEN || board[y] == BISHOP))
                    return 1;
                if (board[y] != EMPTY)
                    break;

            }
    }
    // go right up
    y = k - 7;
    if (y >= 0 && COL(y) != 0) {
        if (color[y] == xside) {
            if (board[y] == KING || board[y] == QUEEN || board[y] == BISHOP)
                return 1;
            if (current_side == WHITE && board[y] == PAWN)
                return 1;
        }
        if (board[y] == EMPTY)
            for (y -= 7; y >= 0 && COL(y) != 0; y -= 7) {
                if (color[y] == xside && (board[y] == QUEEN || board[y] == BISHOP))
                    return 1;
                if (board[y] != EMPTY)
                    break;
            }
    }
    return 0;
}

// Main - Brain function. I guess
int eval()
{
    int     value_piece[6] = {VALUE_PAWN, VALUE_KNIGHT, VALUE_BISHOP, VALUE_ROOK, VALUE_QUEEN, VALUE_KING};
    int     i, score = 0;
    
    for (i = 0; i < 64; i++) // the whole board
    {
        if (color[i] == WHITE)
            score += value_piece[board[i]];
        else if (color[i] == BLACK)
            score -= value_piece[board[i]];
    }
    if (side == WHITE)
        return score;
    return -score;
}

int make_move(MOVE m)
{
    int         r;
    hist[hdp].m = m;
    hist[hdp].cap = board[m.dest];
    board[m.dest] = board[m.from];
    board[m.from] = EMPTY;
    color[m.dest] = color[m.from];
    color[m.from] = EMPTY;
    if (m.type >= MOVE_TYPE_PROMOTION_TO_QUEEN) // Promotion
    {
        switch(m.type)
        {
            case MOVE_TYPE_PROMOTION_TO_QUEEN:
                board[m.dest] = QUEEN;
                break ;

            case MOVE_TYPE_PROMOTION_TO_ROOK:
                board[m.dest] = ROOK;
                break ;

            case MOVE_TYPE_PROMOTION_TO_BISHOP:
                board[m.dest] = BISHOP;
                break ;

            case MOVE_TYPE_PROMOTION_TO_KNIGHT:
                board[m.dest] = KNIGHT;
                break ;
            
            default:
                puts("Impossible to get here.!");
                assert(0);
        }
    }
    ply++;
    hdp++;
    r = !is_in_check(side);
    side = (WHITE + BLACK) - side; // After making the move, we need to give the chance to the opponent
    return r; 
}

void take_back(void) // Undo the move made by make_move 
{
    side = (WHITE + BLACK) - side;
    hdp--;
    ply--;
    board[hist[hdp].m.from] = board[hist[hdp].m.dest];
    board[hist[hdp].m.dest] = hist[hdp].cap;
    color[hist[hdp].m.from] = side;
    if (hist[hdp].cap != EMPTY)
        color[hist[hdp].m.dest] = (WHITE + BLACK) - side;
    else 
        color[hist[hdp].m.dest] = EMPTY;
    if (hist[hdp].m.type >= MOVE_TYPE_PROMOTION_TO_QUEEN)
        board[hist[hdp].m.from] = PAWN;
}

// This is basic minmax search algorithm with alpha beta pruning 
/*
    It works by exploring the possible moves of a game, and assigning a score to each possible outcome. 
    For each level of the search tree, the algorithm alternates between maximizing and minimizing the score, 
    as if the two players are taking turns.
    Alpha-beta pruning is a technique used to optimize the minimax algorithm, by reducing the number of nodes that need to be explored. 
    It works by maintaining two values: alpha, which represents the maximum score that the maximizer player can achieve so far, and beta, 
    which represents the minimum score that the minimizer player can achieve so far. As the algorithm explores the nodes of the game tree, 
    it updates the values of alpha and beta accordingly. If the algorithm finds a node that has a score worse than the current alpha or beta value, 
    it stops exploring the subtree rooted at that node, because it knows that this subtree will not affect the final result. 
*/

int search(int alpha, int beta, int depth, MOVE *pBestMove)
{
    int     i, value, havemove, move_count;
    MOVE    moveBuffer[200], tmpMove;

    nodes++;    // Visit node and count it
    havemove = 0;
    pBestMove->type = MOVE_TYPE_NONE;
    move_count = generate_all_moves(side, moveBuffer); // Generate all moves from current position

    // Loop through the moves 
    for (i = 0; i < move_count; ++i)
    {
        if (!make_move(moveBuffer[i]))
        {
            take_back();
            continue ;
        }
        havemove = 1;
        if (depth - 1 > 0) // If depth is still present continue to search deeper
            value = -search(-beta, -alpha, depth - 1, &tmpMove); // recursively calling with different alpha and beta 
        else 
            value = eval();
        take_back();
        if (value > alpha)
        {
            // this move is so good, it causes a cutoff
            if (value >= beta)
                return (beta);
            alpha = value;
            *pBestMove = moveBuffer[i]; // so far current move is the best move for the current position

        }
    }
    if (!havemove) // If no legal moves left that is either checkmate or stalemate
    {
        if (is_in_check(side))
            return -MATE + ply; // add ply to find longest path to lose or shortest path to win
        else 
            return 0;
    }
    return alpha;
}


// Just calls the search function and prints the results
MOVE computer_think(int max_depth)
{
    MOVE    m;
    int     score;

    // Reset some values before searching
    ply = 0;
    nodes = 0;
    // search using algo
    score = search(-MATE, MATE, max_depth, &m);
    printf("Search result: move = %c%d%c%d; nodes = %d, score = %d\n",
            'a' + COL(m.from),
            8 - ROW(m.from),
            'a' + COL(m.dest),
            8 - ROW(m.dest),
            nodes,
            score
    );
    return m;
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

    for (i = 0; i < 64; i++)
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
    printf("\n type D: to print board on screen\n type move (e.g. b1c3): to make move \n type move with promotion piece (eg. a7a8q): will promote to queen (other choices: q,r,k,b) \n type exit: to quit\n");

    side = WHITE;
    computer_side = BLACK;
    max_depth = 5;
    hdp = 0;
    init_board();
    for (;;)
    {
        if (side == computer_side) // Computers Turn
        {
            // Find best move to react to current position
           MOVE    best_move = computer_think(max_depth);
           make_move(best_move);
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

        // // Print Extracted squares 
        // printf("\nfrom: %d to: %d \n", from, dest);

        // Lets set ply to zero and generate all the moves 
        ply = 0;
        move_count = generate_all_moves(side, moveBuffer);
        // printf("\n move_counts generated : %d\n", move_count);

        // Loop through the moves to see if it is legal 
        for (i = 0; i < move_count; i++)
        {
            if (moveBuffer[i].from == from && moveBuffer[i].dest == dest)
            {
                if (board[from] == PAWN && (dest < 8 || dest > 55)) // check for promotions 
                {
                    switch (user_input[4])
                    {
                        case 'q':
                            moveBuffer[i].type = MOVE_TYPE_PROMOTION_TO_QUEEN;
                            break;
                        case 'r':
                            moveBuffer[i].type = MOVE_TYPE_PROMOTION_TO_ROOK;
                            break;
                        case 'b':
                            moveBuffer[i].type = MOVE_TYPE_PROMOTION_TO_BISHOP;
                            break ;
                        case 'n':
                            moveBuffer[i].type = MOVE_TYPE_PROMOTION_TO_KNIGHT;
                            break; 
                        default :
                            puts ("Default Promotion to Queen! \n");
                            moveBuffer[i].type = MOVE_TYPE_PROMOTION_TO_QUEEN;
                    }
                }
                if (!make_move(moveBuffer[i]))
                {
                    take_back();
                    printf("Illegal move. Not allowed! \n");
                }
                break ;
            }
        }
    }
    return (0);
}