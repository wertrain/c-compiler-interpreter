#include "../include/symbol.h"

#include <iostream>
#include <cstring>

namespace
{

struct SymbolDataLinkItem
{
    const cci::symbol::SymbolData* data_;
    SymbolDataLinkItem* next_;
    SymbolDataLinkItem* prev_;
};

const int kHashTableSize = 53;
SymbolDataLinkItem* hash_table[kHashTableSize];
SymbolDataLinkItem link_data_array[cci::symbol::kMaxSymbol];
static int link_data_array_index = 0;

void ResetInner()
{
    for (int i = 0; i < kHashTableSize; ++i)
    {
        hash_table[i] = nullptr;
    }
    link_data_array_index = 0;
    for (int i = 0; i < cci::symbol::kMaxSymbol; ++i)
    {
        link_data_array[i].data_ = nullptr;
        link_data_array[i].prev_ = nullptr;
        link_data_array[i].next_ = nullptr;
    }
}

int GetHashKey(const cci::symbol::SymbolData *data)
{
    const int length = strlen(data->name_);
    int sum = 0;
    for (int i = 0; i < length; ++i)
    {
        sum += data->name_[i];
    }
    return sum % kHashTableSize;
}

bool EntryHashTable(const cci::symbol::SymbolData *src)
{
    if (link_data_array_index >= cci::symbol::kMaxSymbol)
    {
        return false;
    }

    SymbolDataLinkItem *item = &link_data_array[link_data_array_index];
    ++link_data_array_index;

    const int index = GetHashKey(src);
    SymbolDataLinkItem* current = hash_table[index];
    SymbolDataLinkItem* prev = nullptr;
    if (current == nullptr)
    {
        hash_table[index] = item;
    }
    else
    {
        while (current != nullptr)
        {
            prev = current;
            current = current->next_;
        }

    }
    current = item;
    current->data_ = src;
    current->prev_ = prev;
    current->next_ = nullptr;
    
    if (prev != nullptr)
    {
        prev->next_ = current;
    }

    return true;
}

} // namespace

namespace cci {
namespace symbol {

SymbolData* symbol_pointer_array[cci::symbol::kMaxSymbol];
static int symbol_pointer_array_index = 0;

bool Initialize()
{
    ResetInner();

    for (int i = 0; i < cci::symbol::kMaxSymbol; ++i)
    {
        symbol_pointer_array[i] = nullptr;
    }
    symbol_pointer_array_index = 0;

    return true;
}

void Finalize()
{
    for (int i = 0; i < kHashTableSize; ++i)
    {
        if (hash_table[i] == nullptr || hash_table[i]->data_ == nullptr) continue;
        
        SymbolDataLinkItem* p = hash_table[i];
        while(p != nullptr)
        {
            std::cout << i << ": " << p->data_->name_ << std::endl;
            p = p->next_;
        }
    }

    for (int i = 0; i < symbol_pointer_array_index; ++i)
    {
        if (symbol_pointer_array[i] != nullptr)
        {
            if (symbol_pointer_array[i]->name_ != nullptr)
            {
                delete symbol_pointer_array[i]->name_;
                symbol_pointer_array[i]->name_ = nullptr;
            }
            delete symbol_pointer_array[i];
            symbol_pointer_array[i] = nullptr;
        }
    }
    symbol_pointer_array_index = 0;

    ResetInner();
}

SymbolData* CreateSymbolData(const char* name)
{
    SymbolData* p = new SymbolData();
    const int name_length = strlen(name) + 1;
    p->name_ = new char[name_length];
    strcpy_s(p->name_, name_length, name);
    symbol_pointer_array[symbol_pointer_array_index] = p;
    ++symbol_pointer_array_index;
    return p;
}

SymbolData* Enter(const SymbolData *data, SymbolKind kind)
{
    EntryHashTable(data);
    return nullptr;
}

} // namespace symbol
} // namespace cci
