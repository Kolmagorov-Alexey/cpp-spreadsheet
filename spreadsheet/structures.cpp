#include "common.h"

#include <algorithm>
#include <cctype>
#include <sstream>

const int LETTERS = 26;
const int MAX_POS_LETTER_COUNT = 3;
const int MAX_POSITION_LENGTH = 17;

const Position Position::NONE = { -1, -1 };

bool Position::operator==(const Position rhs) const {
    return row == rhs.row && col == rhs.col;
}

bool Position::operator<(const Position rhs) const {
    return std::tie(row, col) < std::tie(rhs.row, rhs.col);
}

bool Position::IsValid() const {
    return row >= 0 && col >= 0 && row < MAX_ROWS && col < MAX_COLS;
}
Position Position::FromString(std::string_view str) {
    auto it = std::find_if(str.begin(), str.end(), [](const char c) {
        return !(std::isalpha(c) && std::isupper(c));
        });
    std::string_view letters = str.substr(0, it - str.begin());
    std::string_view digits = str.substr(it - str.begin());

    if (letters.empty() || digits.empty()) {
        return Position::NONE;
    } 
    if (!std::isdigit(digits[0])) {
        return Position::NONE;
    }
    if (letters.size() > MAX_POS_LETTER_COUNT) {
        return Position::NONE;
    }
    int row;
    std::istringstream row_in{ std::string{digits} };
    if (!(row_in >> row) || !row_in.eof()) {
        return Position::NONE;
    }

    int col = 0;
    for (char ch : letters) {
        col *= LETTERS;
        col += ch - 'A' + 1;
    }

    return { row - 1, col - 1 };
}

std::string Position::ToString() const {
    if (!IsValid()) {
        return "";
    }
    std::string str;
    str.reserve(MAX_POSITION_LENGTH);
    int val = col;
    while (val >= 0) {
        str.insert(str.begin(), 'A' + val % LETTERS);
        val = val / LETTERS - 1;
    }

    str += std::to_string(row + 1);

    return str;
}

bool Size::operator==(Size rhs) const {
    return cols == rhs.cols && rows == rhs.rows;
}