#pragma once
#include "cell.h"
#include "common.h"

#include <functional>
#include <unordered_map>

struct CellHasher {
    size_t operator()(const Position pos) const {
        return  pos.col  + pos.row * 37;
    }
};

struct CellComparator {
    bool operator()(const Position& lhs, const Position& rhs) const {
        return lhs == rhs;
    }
};

class Sheet : public SheetInterface {
public:
    using Table = std::unordered_map<Position, std::unique_ptr<Cell>, CellHasher, CellComparator>;

    ~Sheet();

    void SetCell(Position pos, std::string text) override;
    const Cell* GetCellPtr(Position pos) const;
    Cell* GetCellPtr(Position pos);
    const CellInterface* GetCell(Position pos) const override;
    CellInterface* GetCell(Position pos) override;

    void ClearCell(Position pos) override;

    Size GetPrintableSize() const override;

    void PrintValues(std::ostream& output) const override;
    void PrintTexts(std::ostream& output) const override;

private:
	Table cells_;
};