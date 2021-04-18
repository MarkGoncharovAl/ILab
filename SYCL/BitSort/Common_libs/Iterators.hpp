#pragma once
#include <iostream>
#include "Errors.hpp"

namespace MLib
{
    template <class Type_t>
    struct Iterator
    {
        class basic_iterator
        {
        public:
            //  !Constructions
            ////////////////////////////////////////////////////////////////
            basic_iterator() = delete;

            basic_iterator(const basic_iterator &that) = default;
            basic_iterator(basic_iterator &&that) = default;
            basic_iterator &operator=(const basic_iterator &that) = default;

            basic_iterator(Type_t *data) noexcept : data_(data) {}
            ////////////////////////////////////////////////////////////////

            //  !Memory
            ////////////////////////////////////////////////////////////////
            virtual Type_t operator*() const = 0;
            ////////////////////////////////////////////////////////////////

            //  !Moving
            ////////////////////////////////////////////////////////////////
            void operator++() noexcept { data_++; }
            void operator++(int) noexcept { data_++; }
            void operator--() noexcept { data_--; }
            void operator--(int) noexcept { data_--; }
            ////////////////////////////////////////////////////////////////

            virtual ~basic_iterator() noexcept {}

        protected:
            Type_t *data_;
        };

        class ordinary_iterator final : public basic_iterator
        {
        public:
            using basic_iterator::data_;

            //  !Constructions
            ////////////////////////////////////////////////////////////////
            ordinary_iterator() = delete;

            ordinary_iterator(const ordinary_iterator &that) = default;
            ordinary_iterator(ordinary_iterator &&that) = default;
            ordinary_iterator &operator=(const ordinary_iterator &that) = default;

            ordinary_iterator(Type_t *data) noexcept : basic_iterator(data) {}
            ////////////////////////////////////////////////////////////////

            //  !Memory
            ////////////////////////////////////////////////////////////////
            Type_t operator*() const override { return *(data_); }
            Type_t &operator*() { return *(data_); }
            ////////////////////////////////////////////////////////////////

            //  !Comparing
            ////////////////////////////////////////////////////////////////
            bool operator==(const ordinary_iterator &that) const noexcept { return data_ == that.data_; }
            bool operator!=(const ordinary_iterator &that) const noexcept { return data_ != that.data_; }
            ////////////////////////////////////////////////////////////////
        };

        class const_iterator final : public basic_iterator
        {
        public:
            using basic_iterator::data_;

            //  !Constructions
            ////////////////////////////////////////////////////////////////
            const_iterator() = delete;

            const_iterator(const const_iterator &that) = default;
            const_iterator(const_iterator &&that) = default;
            const_iterator &operator=(const const_iterator &that) = default;

            const_iterator(Type_t *data) noexcept : basic_iterator(data) {}
            ////////////////////////////////////////////////////////////////

            //  !Memory
            ////////////////////////////////////////////////////////////////
            Type_t operator*() const override { return *(data_); }
            ////////////////////////////////////////////////////////////////

            //  !Comparing
            ////////////////////////////////////////////////////////////////
            bool operator==(const const_iterator &that) const noexcept { return data_ == that.data_; }
            bool operator!=(const const_iterator &that) const noexcept { return data_ != that.data_; }
            ////////////////////////////////////////////////////////////////
        };

        class safe_iterator final : public basic_iterator
        {
        public:
            using basic_iterator::data_;

            //  !Constructions
            ////////////////////////////////////////////////////////////////
            safe_iterator() = delete;

            safe_iterator(const safe_iterator &that) = default;
            safe_iterator(safe_iterator &&that) = default;
            safe_iterator &operator=(const safe_iterator &that) = default;

            safe_iterator(Type_t *data, Type_t *end_data) noexcept : basic_iterator(data), end_data_(end_data) {}
            ////////////////////////////////////////////////////////////////

            //  !Memory
            ////////////////////////////////////////////////////////////////
            Type_t operator*() const override
            {
                if (data_ >= end_data_)
                {
                    WARNING("Reaching out of massive! Returning default object!");
                    return Type_t{};
                }

                return *(data_);
            }
            Type_t &operator*()
            {
                if (data_ >= end_data_)
                    throw std::out_of_range("Reaching the edge of massive of iterators!"); // I can't return anything!!! -> kill

                return *(data_);
            }
            ////////////////////////////////////////////////////////////////

            //  !Comparing
            ////////////////////////////////////////////////////////////////
            bool operator==(const safe_iterator &that) const noexcept { return data_ == that.data_; }
            bool operator!=(const safe_iterator &that) const noexcept { return data_ != that.data_; }
            ////////////////////////////////////////////////////////////////

        private:
            Type_t *end_data_;
        };
    };

    template <class T>
    using M_iterator = typename Iterator<T>::ordinary_iterator;
    template <class T>
    using MC_iterator = typename Iterator<T>::const_iterator;
    template <class T>
    using MS_iterator = typename Iterator<T>::safe_iterator;

} // namespace MLib