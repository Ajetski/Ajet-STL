/*
* list.h
* (c) 2021 Adam Jeniski
*
* Attribution and copyright notice must be retained.
* - Attribution may be augmented to include additional authors
* - Copyright notice cannot be altered
* Attribution and copyright info may be relocated but they must be conspicuous.
*
* Define a class template for arrays
* - see C++17 [array.overview], [array.syn]
* - https://timsong-cpp.github.io/cppwp/n4659/list
* - https://timsong-cpp.github.io/cppwp/n4659/list.syn
*/

#ifndef LIST_H
#define LIST_H

#include <memory>

namespace ajet {
    using std::allocator, std::allocator_traits, std::size_t,
          std::shared_ptr, std::make_shared, std::initializer_list;

    template<typename T>
    struct list_node {
        T data;
        shared_ptr<list_node<T>> next;
        list_node(T data) : data{ data }, next{ nullptr } {}
        list_node(const list_node& rhs) : data { rhs.data }, next{ rhs.next } {}
        list_node operator=(const list_node& rhs) {
            data = rhs.data;
            next = rhs.next;
        }
    };

    template<typename T>
    struct list_iterator {

        list_iterator() noexcept = default;
        list_iterator(list_node<T> *ptr) noexcept : ptr{ ptr } {}

        constexpr list_iterator& operator++() {
            ptr = ptr->next.get();
            return *this;
        }

        constexpr list_iterator& operator++(int) {
            ptr = ptr->next.get();
            return *this;
        }

        constexpr T& operator*() {
            if (ptr == nullptr) {
                throw "test";
            }
            return ptr->data;
        }
        //constexpr T* operator->() const noexcept { return node; }

        constexpr bool operator==(const list_iterator& rhs) const {
            return this->ptr != rhs.ptr;
        }
        constexpr bool operator!=(const list_iterator& rhs) const {
            return !(this->ptr == rhs.ptr);
        }

    private:
        list_node<T>* ptr;

    };

    template <class T, class Allocator = allocator<T>>
    struct list {
        // types:
        using value_type = T;
        using allocator_type = Allocator;
        using pointer = typename allocator_traits<Allocator>::pointer;
        using const_pointer = typename allocator_traits<Allocator>::const_pointer;
        using reference = value_type&;
        using const_reference = const value_type&;
        using size_type = size_t;
        using difference_type = ptrdiff_t;
        using iterator = list_iterator<value_type>;
        //using const_iterator = list_iterator<const list_node<value_type>;
        using reverse_iterator = std::reverse_iterator<iterator>;
        //using const_reverse_iterator = std::reverse_iterator<const_iterator>;

        list() = default;
        
        list(initializer_list<T> vals) {
            assign(vals);
        }

        void assign(size_type n, const T& t) {
            auto iter = begin();
            for (int i = 0; i < n; ++iter, ++i) {}
            *iter = t;
        }

        iterator begin() noexcept { return iterator( head.get() ); }
        //const_iterator begin() const noexcept { return iterator(*head); }

        iterator end() noexcept { return iterator( tail.get()->next.get() ); }
        //const_iterator end() const noexcept { return iterator(*tail); }

        void assign(initializer_list<T> vals) {
            for (T val : vals)
                push_back(val);
        }

        void push_back(const T& x) {
            if (head == nullptr) {
                head = std::move(shared_ptr<Node>{ make_shared<Node>(x) });
                tail = head;
            }
            else {
                tail->next = std::move(shared_ptr<Node>{ make_shared<Node>(x) });
                tail = tail->next;
            }
        }

        void push_back(T&& x) {
            if (head == nullptr){
                head = std::move(shared_ptr<Node>{ make_shared<Node>(x) });
                tail = head;
            }
            else {
                tail->next = std::move(shared_ptr<Node>{ make_shared<Node>(x) });
                tail = tail->next;
            }
        }

    private:
        using Node = list_node<value_type>;
        shared_ptr<Node> head{ nullptr };
        shared_ptr<Node> tail{ nullptr };
	};
}

#endif //LIST_H
