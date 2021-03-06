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

const int kHashTableSize = 53; // 分母となる値を素数にするとハッシュ値の偏りが少なくなる
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

int GetHashKey(const char *name)
{
    const int length = strlen(name);
    int sum = 0;
    for (int i = 0; i < length; ++i)
    {
        sum += name[i];
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

    const int index = GetHashKey(src->name_);
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

/**
 * SymbolData 実体のポインタ配列
 */
SymbolData* symbol_pointer_array[cci::symbol::kMaxSymbol];

/**
 * SymbolData 実体のポインタ配列の最終位置
 */
static int symbol_pointer_array_index = 0;

/**
 * 局所変数記号表空き
 */
static const int kEmptyLocalSymbol = 9999;

/**
 * 局所変数の開始位置
 */
static int start_local_symbol = kEmptyLocalSymbol;

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
    p->args_ = p->arrayLength_ = p->address_ = p->level_ = 0;
    ++symbol_pointer_array_index;
    return p;
}

bool Enter(const SymbolData *data)
{
    return EntryHashTable(data);
}

const SymbolData* SearchSymbolByName(const char* name)
{
    const int index = GetHashKey(name);
    SymbolDataLinkItem* current = hash_table[index];
    SymbolDataLinkItem* prev = nullptr;
    if (current == nullptr)
    {
        return nullptr;
    }
    else
    {
        while (current != nullptr)
        {
            prev = current;
            if (std::strcmp(current->data_->name_, name) == 0)
            {
                return current->data_;
            }
            current = current->next_;
        }
    }
    return nullptr;
}

int GetCodeFlag(const SymbolData *data)
{
    if (data->level_ == 0)
    {
        return 0;
    }
    else
    {
        return 1;
    }
}

void OpenLocalSymbol()
{
    //start_local_symbol = symbol_pointer_array_index + 1;
}

void CloseLocalSymbol(const SymbolData* data)
{
    //symbol_pointer_array_index = start_local_symbol - 1 + data->args_;
    //start_local_symbol = kEmptyLocalSymbol;
}

#ifdef _DEBUG

void DumpTable()
{
    for (int i = 0; i < kHashTableSize; ++i)
    {
        if (hash_table[i] == nullptr || hash_table[i]->data_ == nullptr) continue;
        
        SymbolDataLinkItem* p = hash_table[i];
        while(p != nullptr)
        {
            std::cout << i << ": ";

            if (p->data_->kind_ == kVar)
            {
                std::cout << "Var - ";
            }
            else 
            {
                std::cout << "Function";
                if (p->data_->args_ > 0)
                {
                    std::cout << " (args: " << static_cast<int>(p->data_->args_) << ")";
                }
                std::cout << " - ";
            }
            std::cout << p->data_->name_ << std::endl;
            p = p->next_;
        }
    }
}

#endif // _DEBUG

} // namespace symbol
} // namespace cci
