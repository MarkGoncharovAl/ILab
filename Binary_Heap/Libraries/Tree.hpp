#pragma once

#include <iostream>
#include <vector>
#include <unordered_set>

struct DefaultComarator
{
    template <class U>
    bool operator()(const U &lhs, const U &rhs)
    {
        return lhs < rhs;
    }
};
//
//
//
//
//
//
template <class Type_t, class Comp = DefaultComarator>
class Binary_Heap
{
public:
    Binary_Heap() = default;
    Binary_Heap(const Binary_Heap &) = delete;
    Binary_Heap(Binary_Heap &&) = delete;
    Binary_Heap &operator=(const Binary_Heap &) = delete;

    //  RETUNRNS: true if element was added
    bool insert(const Type_t &new_elem);
    //void balance();

    explicit operator bool() const { return balanced_; }
    bool IsBalanced() const { return balanced_; }

    //  USING: for tests only
    std::vector<Type_t> GetFullData() const;

    ~Binary_Heap();

private:
    size_t last_elem_ = 0;
    std::vector<std::vector<Type_t *>> data_;
    std::unordered_set<Type_t> hash_;
    bool balanced_ = true;
};
//
//
//
//
//
//
//
//
//
//
template <class Type_t, class Comp>
bool Binary_Heap<Type_t, Comp>::insert(const Type_t &new_elem)
{
    if (hash_.find(new_elem) != hash_.cend())
        return false;
    else
        hash_.insert(new_elem);

    balanced_ = false;
    if (data_.empty() ||
        last_elem_ == data_.back().size())
    {
        last_elem_ *= 2;
        if (data_.empty())
            last_elem_ = 1;

        std::vector<Type_t *> new_mas(last_elem_, nullptr);
        new_mas[0] = new Type_t{new_elem};
        data_.push_back(new_mas);

        last_elem_ = 1;
        return true;
    }

    //!now las_elem_ != last_size_
    data_.back()[last_elem_] = new Type_t{new_elem};
    last_elem_++;
    return true;
}

template <class Type_t, class Comp>
std::vector<Type_t> Binary_Heap<Type_t, Comp>::GetFullData() const
{
    std::vector<Type_t> output;
    for (const auto &mas : data_)
    {
        for (const Type_t *elem : mas)
            if (elem)
                output.push_back(*elem);
    }
    return output;
}

template <class Type_t, class Comp>
Binary_Heap<Type_t, Comp>::~Binary_Heap()
{
    for (auto &mas : data_)
    {
        for (Type_t *elem : mas)
            if (elem)
                delete elem;
    }
}