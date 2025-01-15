#include <cstddef>
#include <cstdio>
#include <fstream>
#include <vector>

bool check_horizontal(const std::vector<std::vector<char>>& matrix, int y,
                      int x, const std::string& word) {

    if (x + word.size() > matrix.size()) return false;

    for (int i = 0; i < word.length(); i++)
        if (word[i] != matrix[y][x + i]) return false;

    return true;
}

bool check_vertical(const std::vector<std::vector<char>>& matrix, int y, int x,
                    const std::string& word) {

    if (y + word.size() > matrix.size()) return false;

    for (int i = 0; i < word.length(); i++)
        if (word[i] != matrix[y + i][x]) return false;

    return true;
}

bool check_diagonal_1(const std::vector<std::vector<char>>& matrix, int y,
                      int x, const std::string& word) {

    if (x + word.size() > matrix.size()) return false;
    if (y + word.size() > matrix.size()) return false;

    for (int i = 0; i < word.length(); i++)
        if (word[i] != matrix[y + i][x + i]) return false;

    return true;
}

bool check_diagonal_2(const std::vector<std::vector<char>>& matrix, int y,
                      int x, const std::string& word) {

    if (x - static_cast<int>(word.size()) < -1) return false;
    if (y + word.size() > matrix.size()) return false;

    for (int i = 0; i < word.length(); i++) {
        if (word[i] != matrix[y + i][x - i]) return false;
    }

    return true;
}

int main() {
    std::fstream file("input.txt");
    std::vector<std::vector<char>> matrix;
    const std::vector<std::string> words{"XMAS", "SAMX"};
    const std::vector<std::string> task_2_words{"SAM", "MAS"};
    std::string line{};
    int result_1 = 0;
    int result_2 = 0;

    while (std::getline(file, line))
        matrix.emplace_back(line.begin(), line.end());

    for (size_t y = 0; y < matrix.size(); y++) {
        for (size_t x = 0; x < matrix.at(y).size(); x++) {

            for (auto word: words) {
                result_1 += check_diagonal_1(matrix, y, x, word) +
                            check_diagonal_2(matrix, y, x, word) +
                            check_vertical(matrix, y, x, word) +
                            check_horizontal(matrix, y, x, word);
            }

            bool flag_01 = false, flag_02 = false;

            for (auto word: task_2_words) {
                flag_01 |= check_diagonal_1(matrix, y, x, word);
                flag_02 |=
                    check_diagonal_2(matrix, y, x + word.length() - 1, word);
            }

            result_2 += ( int )(flag_01 && flag_02);
        }
    }

    printf("Task 1 result: [%d]\n", result_1);
    printf("Task 2 result: [%d]\n", result_2);

    return 0;
}
