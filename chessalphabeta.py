import chess

def evaluate(board: chess.Board) -> int:
    """Evaluate the board position."""
    if board.is_checkmate():
        return -9999 if board.turn else 9999
    if board.is_stalemate() or board.is_insufficient_material():
        return 0

    piece_scores = {
        chess.PAWN: 1,
        chess.KNIGHT: 3,
        chess.BISHOP: 3,
        chess.ROOK: 5,
        chess.QUEEN: 9,
        chess.KING: 0
    }

    white_score = sum(len(board.pieces(pt, chess.WHITE)) * val for pt, val in piece_scores.items())
    black_score = sum(len(board.pieces(pt, chess.BLACK)) * val for pt, val in piece_scores.items())
    return white_score - black_score


def alpha_beta(board: chess.Board, depth: int, alpha: int, beta: int, is_maximizing: bool) -> int:
    """Minimax algorithm with Alpha-Beta pruning."""
    if depth == 0 or board.is_game_over():
        return evaluate(board)

    if is_maximizing:
        best_value = -float('inf')
        for move in board.legal_moves:
            board.push(move)
            value = alpha_beta(board, depth - 1, alpha, beta, False)
            board.pop()
            best_value = max(best_value, value)
            alpha = max(alpha, value)
            if beta <= alpha:
                break  
        return best_value
    else:
        best_value = float('inf')
        for move in board.legal_moves:
            board.push(move)
            value = alpha_beta(board, depth - 1, alpha, beta, True)
            board.pop()
            best_value = min(best_value, value)
            beta = min(beta, value)
            if beta <= alpha:
                break  
        return best_value

def choose_best_move(board: chess.Board, depth: int) -> chess.Move:
    """Find the best move using Minimax + Alpha-Beta pruning."""
    best_move = None
    highest_value = -float('inf')

    for move in board.legal_moves:
        board.push(move)
        value = alpha_beta(board, depth - 1, -float('inf'), float('inf'), False)
        board.pop()

        if value > highest_value:
            highest_value = value
            best_move = move
    return best_move

def chess_game():
    board = chess.Board()
    max_depth = 4  
    print("♟️  Chess AI using Minimax with Alpha-Beta Pruning ♟️")
    print(f"Search Depth: {max_depth}\n")
    print(board)
    user_is_white = input("Do you want to play as White (y/n)? ").strip().lower() == "y"
    if not user_is_white:
        ai_move = choose_best_move(board, max_depth)
        board.push(ai_move)
        print(f"\nAI plays first: {ai_move}")
        print(board)

    while not board.is_game_over():
       
        if (board.turn == chess.WHITE and user_is_white) or (board.turn == chess.BLACK and not user_is_white):
            move = None
            while move not in board.legal_moves:
                move_input = input("Enter your move (UCI format, e.g., e2e4): ").strip()
                try:
                    move = chess.Move.from_uci(move_input)
                except:
                    print("Invalid move format. Try again.")
                    continue
                if move not in board.legal_moves:
                    print("Illegal move. Try again.")
            board.push(move)
            print("\nYour move played:")
        else:
         
            ai_move = choose_best_move(board, max_depth)
            board.push(ai_move)
            print(f"\nAI plays: {ai_move}")

        print(board)

    print("\nGame Over!")
    print("Result:", board.result())

if __name__ == "__main__":
    chess_game()
