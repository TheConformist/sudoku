#include <iostream>
#include <fstream>
#include <vector>
#include <cstdlib>
#include <ctime>
#include <cstring>
#include <string>

const int SIZE = 9;
const int MIN_HOLES = 20;
const int MAX_HOLES = 55;

// 检查在指定位置是否可以放置数字num
bool isValid(const std::vector<std::vector<int>>& board, int row, int col, int num) {
    // 检查行和列
    for (int i = 0; i < SIZE; i++) {
        if (board[row][i] == num || board[i][col] == num) {
            return false;
        }
    }

    // 检查所在的九宫格
    int startRow = row - row % 3;
    int startCol = col - col % 3;
    for (int i = 0; i < 3; i++) {
        for (int j = 0; j < 3; j++) {
            if (board[startRow + i][startCol + j] == num) {
                return false;
            }
        }
    }

    return true;
}

// 使用回溯法求解数独问题
bool solveSudoku(std::vector<std::vector<int>>& board) {
    for (int row = 0; row < SIZE; row++) {
        for (int col = 0; col < SIZE; col++) {
            if (board[row][col] == 0) {
                for (int num = 1; num <= SIZE; num++) {
                    if (isValid(board, row, col, num)) {
                        board[row][col] = num;
                        if (solveSudoku(board)) {
                            return true;
                        }
                        board[row][col] = 0;  // 回溯
                    }
                }
                return false;
            }
        }
    }
    return true;  // 数独已解
}

// 生成随机数独终局
void generateSudoku(std::vector<std::vector<int>>& board) {
    std::srand(static_cast<unsigned>(std::time(nullptr)));

    // 初始化数独棋盘
    for (int i = 0; i < SIZE; i++) {
        std::vector<int> row;
        for (int j = 0; j < SIZE; j++) {
            row.push_back(0);
        }
        board.push_back(row);
    }

    // 填充数独棋盘
    solveSudoku(board);
}

// 随机挖洞
void createHoles(std::vector<std::vector<int>>& board, int holes) {
    while (holes > 0) {
        int row = std::rand() % SIZE;
        int col = std::rand() % SIZE;
        if (board[row][col] != 0) {
            board[row][col] = 0;
            holes--;
        }
    }
}

// 打印数独棋盘
void printSudoku(const std::vector<std::vector<int>>& board) {
    for (int i = 0; i < SIZE; i++) {
        for (int j = 0; j < SIZE; j++) {
            std::cout << board[i][j] << " ";
        }
        std::cout << std::endl;
    }
}

//
// 将数独问题保存到文件
void saveSudokuToFile(const std::vector<std::vector<int>>& board, const std::string& filename) {
    std::ofstream file(filename);
    if (file.is_open()) {
        for (int i = 0; i < SIZE; i++) {
            for (int j = 0; j < SIZE; j++) {
                file << board[i][j] << " ";
            }
            file << std::endl;
        }
        file.close();
        std::cout << "数独问题已保存到文件：" << filename << std::endl;
    }
    else {
        std::cout << "无法保存数独问题到文件：" << filename << std::endl;
    }
}

// 从文件加载数独问题
std::vector<std::vector<int>> loadSudokuFromFile(const std::string& filename) {
    std::vector<std::vector<int>> board;
    std::ifstream file(filename);
    if (file.is_open()) {
        int num;
        while (file >> num) {
            std::vector<int> row;
            row.push_back(num);
            for (int i = 1; i < SIZE; i++) {
                file >> num;
                row.push_back(num);
            }
            board.push_back(row);
        }
        file.close();
        std::cout << "从文件加载数独问题成功：" << filename << std::endl;
    }
    else {
        std::cout << "无法从文件加载数独问题：" << filename << std::endl;
    }
    return board;
}

int main(int argc, char* argv[]) {
    std::vector<std::vector<int>> board;

    if (argc < 3) {
        std::cout << "参数不足，请参考正确的参数格式：" << std::endl;
        std::cout << "生成数独终盘示例：sudoku.exe -c 20" << std::endl;
        std::cout << "求解数独游戏示例：sudoku.exe -s game.txt" << std::endl;
        std::cout << "生成数独游戏示例：sudoku.exe -n 1000 -m 1 -r 20~55 -u" << std::endl;
        return 1;
    }

    std::string option = argv[1];

    if (option == "-c") {
        if (argc != 3) {
            std::cout << "参数不正确，请参考正确的参数格式：" << std::endl;
            std::cout << "生成数独终盘示例：sudoku.exe -c 20" << std::endl;
            return 1;
        }

        int num = std::atoi(argv[2]);
        for (int i = 0; i < num; i++) {
            board.clear();
            generateSudoku(board);
            std::string filename = "sudoku_" + std::to_string(i) + ".txt";
            saveSudokuToFile(board, filename);
        }
    }
    else if (option == "-s") {
        if (argc != 3) {
            std::cout << "参数不正确，请参考正确的参数格式：" << std::endl;
            std::cout << "求解数独游戏示例：sudoku.exe -s game.txt" << std::endl;
            return 1;
        }

        std::string filename = argv[2];
        std::vector<std::vector<int>> problem = loadSudokuFromFile(filename);
        std::cout << "数独问题：" << std::endl;
        printSudoku(problem);

        solveSudoku(problem);

        // 将求解结果保存到文件
        saveSudokuToFile(problem, "sudoku_solution.txt");

        std::cout << "数独问题的解已保存到文件：sudoku_solution.txt" << std::endl;
    }
    else if (option == "-n") {
        if (argc < 4 || argc > 8) {
            std::cout << "参数不正确，请参考正确的参数格式：" << std::endl;
            std::cout << "生成数独游戏示例：sudoku.exe -n 1000 -m 1 -r 20~55 -u" << std::endl;
            return 1;
        }

        int numGames = std::atoi(argv[2]);

        int difficulty = -1;
        int numHoles = -1;
        bool uniqueSolution = false;

        for (int i = 3; i < argc; i++) {
            std::string arg = argv[i];
            if (arg == "-m") {
                if (i + 1 < argc) {
                    difficulty = std::atoi(argv[i + 1]);
                }
                else {
                    std::cout << "参数不正确，请提供难度级别（1-3）：" << std::endl;
                    return 1;
                }
            }
            else if (arg == "-r") {
                if (i + 1 < argc) {
                    std::string range = argv[i + 1];
                    size_t pos = range.find("~");
                    if (pos != std::string::npos) {
                        std::string minStr = range.substr(0, pos);
                        std::string maxStr = range.substr(pos + 1);
                        int minHoles = std::atoi(minStr.c_str());
                        int maxHoles = std::atoi(maxStr.c_str());
                        if (minHoles >= MIN_HOLES && maxHoles <= MAX_HOLES && minHoles <= maxHoles) {
                            numHoles = std::rand() % (maxHoles - minHoles + 1) + minHoles;
                        }
                        else {
                            std::cout << "参数不正确，请提供有效的挖洞范围（20-55）：" << std::endl;
                            return 1;
                        }
                    }
                    else {
                        std::cout << "参数不正确，请提供有效的挖洞范围（20-55）：" << std::endl;
                        return 1;
                    }
                }
                else {
                    std::cout << "参数不正确，请提供挖洞范围（20-55）：" << std::endl;
                    return 1;
                }
            }
            else if (arg == "-u") {
                uniqueSolution = true;
            }
        }

        if (difficulty < 1 || difficulty > 3) {
            std::cout << "参数不正确，请提供有效的难度级别（1-3）：" << std::endl;
            return 1;
        }

        for (int i = 0; i < numGames; i++) {
            board.clear();
            generateSudoku(board);
            std::string filename = "sudoku_game_" + std::to_string(i) + ".txt";
            saveSudokuToFile(board, filename);
        }
    }
    else {
        std::cout << "无效的选项，请参考正确的参数格式：" << std::endl;
        std::cout << "生成数独终盘示例：sudoku.exe -c 20" << std::endl;
        std::cout << "求解数独游戏示例：sudoku.exe -s game.txt" << std::endl;
        std::cout << "生成数独游戏示例：sudoku.exe -n 1000 -m 1 -r 20~55 -u" << std::endl;
        return 1;
    }

    return 0;
}

