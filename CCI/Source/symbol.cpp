#include "../include/symbol.h"

namespace
{

struct SymbolDataLinkItem
{
    cci::symbol::SymbolData* current_;
    cci::symbol::SymbolData* next_;
    cci::symbol::SymbolData* prev_;
};

const int kHashTableSize = 53;
SymbolDataLinkItem* hash_table[kHashTableSize];
SymbolDataLinkItem link_data_array[cci::symbol::kMaxSymbol];

} // namespace

namespace cci {
namespace symbol {

SymbolData symbol_data_table[kMaxSymbol];

SymbolData* enter(const SymbolData *data, SymbolKind kind)
{
    return &symbol_data_table[0];
}

} // namespace symbol
} // namespace cci
