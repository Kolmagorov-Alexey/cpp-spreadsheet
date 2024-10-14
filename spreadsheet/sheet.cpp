
#include "cell.h"
#include "common.h"
#include "sheet.h"

#include <algorithm>
#include <functional>
#include <iostream>
#include <optional>

using namespace std::literals;

Sheet::~Sheet() {}

void Sheet::SetCell(Position pos, std::string text) {
    if (!pos.IsValid()) { throw InvalidPositionException("Invalid position"); }
    
    const auto& cell = cells_.find(pos);
    if (cell == cells_.end()) { cells_.emplace(pos, std::make_unique<Cell>(*this)); }
    cells_.at(pos)->Set(std::move(text));
}

CellInterface* Sheet::GetCell(Position pos) {
    return GetCellPtr(pos);
}
const CellInterface* Sheet::GetCell(Position pos) const {
    return GetCellPtr(pos);
}

void Sheet::ClearCell(Position pos) {
    if (!pos.IsValid()) { throw InvalidPositionException("Invalid position"); }

    const auto& cell = cells_.find(pos);
    if (cell != cells_.end() && cell->second != nullptr) {
        cell->second->Clear();
        if (!cell->second->IsReferenced()) {
            cell->second.reset();
        }
    }
}

Size Sheet::GetPrintableSize() const {
    Size result{ 0, 0 };
    
    for (auto it = cells_.begin(); it != cells_.end(); ++it) {
        if (it->second != nullptr) {
            const int col = it->first.col;
            const int row = it->first.row;
            result.cols = std::max(result.cols, col + 1);
            result.rows = std::max(result.rows, row + 1);  
        }
    }
    return { result.rows, result.cols };
}

void Sheet::PrintTexts(std::ostream& output) const {
    Size size = GetPrintableSize();
    for (int row = 0; row < size.rows; ++row) {
        for (int col = 0; col < size.cols; ++col) {
            if (col > 0) output << "\t";
            const auto& it = cells_.find({ row, col });
            if (it != cells_.end() && it->second != nullptr && !it->second->GetText().empty()) {
                output << it->second->GetText();
            }
        }
        output << "\n";
    }
}
void Sheet::PrintValues(std::ostream& output) const {
    Size size = GetPrintableSize();
    for (int row = 0; row < size.rows; ++row) {
        for (int col = 0; col < size.cols; ++col) {
            if (col > 0) output << "\t";
            const auto& it = cells_.find({ row, col });
            if (it != cells_.end() && it->second != nullptr && !it->second->GetText().empty()) {
                std::visit([&](const auto value) { output << value; }, it->second->GetValue());
            }
        }
        output << "\n";
    }
}

const Cell* Sheet::GetCellPtr(Position pos) const {
    return const_cast<Sheet*>(this)->GetCellPtr(pos);
    
}

Cell* Sheet::GetCellPtr(Position pos) {
    if (!pos.IsValid()) { throw InvalidPositionException("Invalid position"); }

    const auto cell = cells_.find(pos);
    if (cell == cells_.end()) {
        return nullptr;
    }
    return cells_.at(pos).get();
}

std::unique_ptr<SheetInterface> CreateSheet() {
    return std::make_unique<Sheet>();
}