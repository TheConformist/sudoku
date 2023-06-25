#include <iostream>
#include <vector>
#include <algorithm>
#include <ctime>
#include <random>
#include <fstream>
#include <string>
#include <cstdlib>
#include <sstream>
using namespace std;
std::string vectorToString(const std::vector<std::vector<char>>& vec) {
    std::string str;
    for (const auto& innerVec : vec) {
        for (char c : innerVec) {
            str  += c;
        }
        str += '\n';
    }
    return str;
}
std::vector<std::vector<std::vector<char>>> readBoardsFromFile(const std::string& filePath) {
    std::ifstream inFile(filePath);
    if (!inFile) {
        std::cerr << "Error: Failed to open file for reading." << std::endl;
        return {};
    }

    std::vector<std::vector<std::vector<char>>> boards;
    std::vector<std::vector<char>> board;
    std::string line;

    while (std::getline(inFile, line)) {
        if (line.empty()) {
            if (!board.empty()) {
                boards.push_back(board);
                board.clear();
            }
        } else {
            std::vector<char> row;
            for (char c : line) {
                if (c != ' ') {
                    row.push_back(c);
                }
            }
            board.push_back(row);
        }
    }

    if (!board.empty()) {
        boards.push_back(board);
    }

    inFile.close();
    return boards;
}

void writeBoardToFile(const std::vector<std::vector<std::vector<char>>>& res_v, const std::string& filePath) {
    std::ofstream outFile(filePath, std::ios::app);
    if (!outFile) {
        std::cerr << "Error: Failed to open file for writing." << std::endl;
        return;
    }

    for (const auto& board : res_v) {
        for (const auto& row : board) {
            for (char cell : row) {
                outFile << cell << ' ';
            }
            outFile << '\n';
        }
        outFile << '\n';
    }

    outFile.close();
}
class SudokuSolver{
public:
    bool solve(std::string s){
      std::vector<std::vector<char>> board(9,std::vector<char>(9,'$'));
      for(int i=0;i<9;++i){
          for(int j=0;j<9;++j){
              board[i][j]=s[i*9+j];
          }
      }
      return solveSudoku(board);
    }
   

private:
    bool solveSudoku(std::vector<std::vector<char>>& board){
      return dfs(board,0,0);
    }

    bool dfs(std::vector<std::vector<char>>& board,int row,int col){
      if(col==9){
          return dfs(board,row+1,0);
      }
      if(row==9){
          return true;
      }
      if(board[row][col]!='$'){
          return dfs(board,row,col+1);
      }
      for(int i=1;i<=9;++i){
          if(!isValid(board,row,col,i+'0')){
              continue;
          }
          board[row][col]=i+'0';
          if(dfs(board,row,col+1)){
              return true;
          }
          board[row][col]='$';
      }
      return false;
    }

    bool isValid(std::vector<std::vector<char>>& board,int row,int col,char c){
      for(int i=0;i<9;++i){
          if(board[i][col]==c){
              return false;
          }
          if(board[row][i]==c){
              return false;
          }
          if(board[(row/3)*3+i/3][(col/3)*3+i%3]==c){
              return false;
          }
      }
      return true;
    }

};

class SudokuGenerator {
public:
  int difficulty;
    SudokuGenerator(int difficulty) {
        
            this->difficulty = difficulty;
    
    }
	void digHoles(std::vector<std::vector<char>>& board, int numHoles) {
    srand(time(0));
    int count = 0;
     
    while (count < numHoles) {
        int row = rand() % 9;
        int col = rand() % 9;
        if (board[row][col] != '$') {
            board[row][col] = '$';
            count++;
       	 }
   	 }
	}
    std::vector<std::vector<char>> generate() {
        std::vector<std::vector<char>> board(9, std::vector<char>(9, '$'));
        solveSudoku(board);

        return board;
    }
      std::vector<std::vector<char>> generate_game() {
        std::vector<std::vector<char>> board(9, std::vector<char>(9, '$'));
        solveSudoku(board);
        digHoles(board,this->difficulty);

        return board;
    }
bool isUnique(std::vector<std::vector<char>>& board){
      std::string s="";
      for(auto row:board){
          for(auto cell:row){
              s+=cell;
          }
      }
      SudokuSolver solver;
      return solver.solve(s);
    }

   

    void solveSudoku(std::vector<std::vector<char>>& board) {
        dfs(board, 0, 0);
    }

    bool dfs(std::vector<std::vector<char>>& board, int row, int col) {
        if (col == 9) {
            return dfs(board, row + 1, 0);
        }
        if (row == 9) {
            return true;
        }
        if (board[row][col] != '$') {
            return dfs(board, row, col + 1);
        }
        std::vector<int> nums{1, 2, 3, 4, 5, 6, 7, 8, 9};
        std::shuffle(nums.begin(), nums.end(), std::random_device());
        for (int i : nums) {
            if (!isValid(board, row, col, i + '0')) {
                continue;
            }
            board[row][col] = i + '0';
            if (dfs(board, row, col + 1)) {
                return true;
            }
            board[row][col] = '$';
        }
        return false;
    }

    bool isValid(std::vector<std::vector<char>>& board, int row, int col, char c) {
        for (int i = 0; i < 9; ++i) {
            if (board[i][col] == c) {
                return false;
            }
            if (board[row][i] == c) {
                return false;
            }
            if (board[(row / 3) * 3 + i / 3][(col / 3) * 3 + i % 3] == c) {
                return false;
            }
        }
        return true;
    }

    

};


int main(int argc, char* argv[]) {
    int numSudokuEndgames = 0; // ������������
    std::string sudokuBoardFilePath; // ���������ļ�·��
    int numGames = 0; // ��Ϸ����
    int difficulty = 1; // ��Ϸ�Ѷ�
    int numHolesRange = 20; // �ڿ�������Χ
    bool uniqueSolution = false; // ��Ϸ��Ψһ

    for (int i = 1; i < argc; i++) {
        std::string arg = argv[i];
        if (arg == "-c") { // ���� -c ѡ��
            if (i + 1 < argc) {
                numSudokuEndgames = std::atoi(argv[++i]);
                if (numSudokuEndgames < 1 || numSudokuEndgames > 1000000) {
                    std::cerr << "����-c ѡ���ֵ��Ч��ֵ������ 1 �� 1000000 ֮�䡣" << std::endl;
                    return -1;
                }
            } else {
                std::cerr << "����-c ѡ����Ҫһ��������" << std::endl;
                return -1;
            }
        } else if (arg == "-s") { // ���� -s ѡ��
            if (i + 1 < argc) {
                sudokuBoardFilePath = argv[++i];
            } else {
                std::cerr << "����-s ѡ����Ҫһ��������" << std::endl;
                return -1;
            }
        } else if (arg == "-n") { // ���� -n ѡ��
            if (i + 1 < argc) {
                numGames = std::atoi(argv[++i]);
                if (numGames < 1 || numGames > 10000) {
                    std::cerr << "����-n ѡ���ֵ��Ч��ֵ������ 1 �� 10000 ֮�䡣" << std::endl;
                    return -1;
                }
            } else {
                std::cerr << "����-n ѡ����Ҫһ��������" << std::endl;
                return -1;
            }
        } else if (arg == "-m") { // ���� -m ѡ��
            if (i + 1 < argc) {
                difficulty = std::atoi(argv[++i]);
                if (difficulty < 1 || difficulty > 3) {
                    std::cerr << "����-m ѡ���ֵ��Ч��ֵ������ 1 �� 3 ֮�䡣" << std::endl;
                    return -1;
                }
            } else {
                std::cerr << "����-m ѡ����Ҫһ��������" << std::endl;
                return -1;
            }
        } else if (arg == "-r") { // ���� -r ѡ��
            if (i + 1 < argc) {
                numHolesRange = std::atoi(argv[++i]);
                if (numHolesRange < 20 || numHolesRange > 55) {
                    std::cerr << "����-r ѡ���ֵ��Ч��ֵ������ 20 �� 55֮�䡣" << std::endl;
                    return -1;
                }
            } else {
                std::cerr << "����-r ѡ����Ҫһ��������" << std::endl;
                return -1;
            }
        } else if (arg == "-u") { // ���� -u ѡ��
            uniqueSolution = true;
        }
    }
    if ((uniqueSolution && numGames == 0) ) {
    std::cerr << "���� -n ѡ��ֻ�ܺ�-u ��ͬʱʹ�á�" << std::endl;
    return -1;
}
SudokuGenerator generator(0);

generator.difficulty=(int)(numHolesRange);



std::vector<std::vector<std::vector<char>>> res_v;
    srand(time(0));

    if(numSudokuEndgames){
while (numSudokuEndgames--) {
    auto res = generator.generate();

    res_v.push_back(res);
}
writeBoardToFile(res_v,"����.txt");
    }

std::vector<std::vector<std::vector<char>>> res_v2;
if(numGames){
    while (numGames--)
    {

        auto res = generator.generate_game();
        while (!generator.isUnique(res))
        {
          res = generator.generate_game();
        }
        
        res_v2.push_back(res);
        
    }
    writeBoardToFile(res_v2,"��Ϸ.txt");
}
std::vector<std::vector<std::vector<char>>> res_v3;

if(sudokuBoardFilePath!=""){
    auto res = readBoardsFromFile(sudokuBoardFilePath);
  
    // cout<<vectorToString(r)<<endl;
    for(auto r : res){
    generator.solveSudoku(r);
    res_v3.push_back(r);
    }
    
}
    writeBoardToFile(res_v3,"���.txt");

    return 0;
}



