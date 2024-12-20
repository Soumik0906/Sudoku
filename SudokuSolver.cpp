#include <iostream>
#include <vector>
#include <thread>
#include <chrono>
#include <fstream>

class SudokuSolver
{
    std::vector<std::vector<char>> originalBoard; // To track the initial board

    static bool canBePlaced(const int i, const int j, const char k, const std::vector<std::vector<char>>& board)
    {
        const int startRow = i / 3 * 3;
        const int startCol = j / 3 * 3;

        for (int a = 0; a < 9; ++a)
        {
            if (board[i][a] == k || board[a][j] == k || board[startRow + a / 3][startCol + a % 3] == k)
                return false;
        }
        return true;
    }

    bool sudokuSolverUtil(const int x, const int y, std::vector<std::vector<char>>& board)
    {
        for (int i = x; i < 9; ++i)
        {
            for (int j = (i == x ? y : 0); j < 9; ++j)
            {
                // Reset y for new rows
                if (board[i][j] == '.')
                {
                    for (char k = '1'; k <= '9'; ++k)
                    {
                        if (canBePlaced(i, j, k, board))
                        {
                            board[i][j] = k;
                            printBoard(board);
                            if (sudokuSolverUtil(i, j + 1, board))
                                return true;
                            board[i][j] = '.';
                        }
                    }
                    return false;
                }
            }
        }
        return true;
    }

    void printBoard(const std::vector<std::vector<char>>& board) const
    {
        for (int i = 0; i < 9; ++i)
        {
            for (int j = 0; j < 9; ++j)
            {
                // Check if the cell is a newly added number
                if (originalBoard[i][j] == '.' && board[i][j] != '.')
                {
                    // Print new numbers in green
                    std::cout << "\033[32m" << board[i][j] << " \033[0m";
                }
                else
                {
                    // Print original numbers in default color
                    std::cout << board[i][j] << ' ';
                }
            }
            std::cout << '\n';
        }
        std::cout << "-----------------" << std::endl;

        std::this_thread::sleep_for(std::chrono::milliseconds(700));
    }

public:
    void solveSudoku(std::vector<std::vector<char>>& board)
    {
        originalBoard = board; // Store the original board
        sudokuSolverUtil(0, 0, board);
    }
};

std::vector<std::vector<char>> fetchSudoku(const std::string& filename)
{
    std::vector board(9, std::vector<char>(9));
    std::ifstream inputFile(filename);
    if (!inputFile.is_open())
    {
        std::cerr << "Error opening file: " << filename << std::endl;
        exit(1);
    }

    for (int i = 0; i < 9; ++i)
    {
        std::string line;
        getline(inputFile, line);
        for (int j = 0; j < 9; ++j)
        {
            board[i][j] = line[j]; // Assume '.' for empty cells
        }
    }

    inputFile.close();
    return board;
}


int main()
{
    // absolute path to text file containing the board
    std::string filename = "/home/soumik/sudoku_catcher/daily_sudoku.txt";
    std::vector board{ fetchSudoku(filename) };

    auto sol{ SudokuSolver() };
    sol.solveSudoku(board);

    return 0;
}
