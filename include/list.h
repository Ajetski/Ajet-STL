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
#include <iostream>

namespace ajet {
    using std::allocator, std::allocator_traits, std::size_t,
          std::shared_ptr, std::make_shared, std::initializer_list;

    template<typename T>
    struct list_node {
        T data;
        shared_ptr<list_node<T>> next;
        shared_ptr<list_node<T>> previous;
        list_node(T data) : data{ data }, next{ nullptr }, previous{ nullptr } {}
        list_node(T data, shared_ptr<list_node<T>> next) :
            data{ data }, next{ next }, previous{ nullptr } {}
        list_node(T data, shared_ptr<list_node<T>> next, shared_ptr<list_node<T>> previous) :
            data{ data }, next{ next }, previous{ previous } {}
        list_node(const list_node& rhs) : data{ rhs.data }, next{ rhs.next }, previous{ rhs.previous } {}
        list_node operator=(const list_node& rhs) {
            data = rhs.data;
            next = rhs.next;
            previous = rhs.previous;
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
            list_iterator copy = *this;
            ptr = ptr->next.get();
            return copy;
        }

        constexpr list_iterator& operator--() {
            ptr = ptr->previous.get();
            return *this;
        }

        constexpr list_iterator& operator--(int) {
            list_iterator copy = *this;
            ptr = ptr->previous.get();
            return copy;
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

        //for_degugging
        list_node<T>* getNodePtr() {
            return ptr;
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
        using const_iterator = list_iterator<value_type>; // to be implemented/changed
        using reverse_iterator = std::reverse_iterator<iterator>;
        using const_reverse_iterator = std::reverse_iterator<const_iterator>;

        // [list.cons], construct/copy/destroy
        /*list() : list(Allocator()) { }
        explicit list(const Allocator&);
        explicit list(size_type n, const Allocator & = Allocator());
        list(size_type n, const T& value, const Allocator & = Allocator());
        template <class InputIterator>
        list(InputIterator first, InputIterator last, const Allocator & = Allocator());*/

        list(const list& x) {
            for (auto iter = x.begin(); iter != x.end(); ++iter) {
                push_back(*iter);
            }
        }

        list(list&& x) {
            for (auto iter = x.begin(); iter != x.end(); ++iter) {
                push_back(*iter);
            }
        }

        /*
        list(const list&, const Allocator&);
        list(list&&, const Allocator&);*/

        list(initializer_list<T> vals, const Allocator & = Allocator()) { assign(vals); }

        ~list() {
            while (!empty()) {
                pop_front();
            }
        }

        /*list& operator=(const list& x);
        list& operator=(list&& x)
            noexcept(allocator_traits<Allocator>::is_always_equal::value);
        list& operator=(initializer_list<T>);
        template <class InputIterator>
        void assign(InputIterator first, InputIterator last);*/

        void assign(size_type n, const_reference t) {
            auto iter = begin();
            for (int i = 0; i < n; ++iter, ++i) {}
            *iter = t;
        }

        void assign(initializer_list<T> vals) {
            for (T val : vals)
                push_back(val);
        }

        allocator_type get_allocator() const noexcept { return allocator; }

        // iterators:
        iterator               begin() noexcept { return iterator(head.get()); }
        const_iterator         begin() const noexcept { return iterator(head.get()); }
        iterator               end() noexcept { return iterator(tail.get()->next.get()); }
        const_iterator         end() const noexcept { return iterator(tail.get()->next.get()); }
        //reverse_iterator       rbegin() noexcept {}
        //const_reverse_iterator rbegin() const noexcept;
        //reverse_iterator       rend() noexcept;
        //const_reverse_iterator rend() const noexcept;

        //const_iterator         cbegin() const noexcept;
        //const_iterator         cend() const noexcept;
        //const_reverse_iterator crbegin() const noexcept;
        //const_reverse_iterator crend() const noexcept;*/

        // [list.capacity], capacity
        bool      empty() const noexcept {
            return _size == 0;
        }

        size_type size() const noexcept {
            return _size;
        }

        size_type max_size() const noexcept {
            return INT_MAX;
        }

        /*void      resize(size_type sz);
        void      resize(size_type sz, const T& c);*/

        // element access:
        reference       front() {
            return head.get()->data;
        }

        const_reference front() const {
            return head.get()->data;
        }

        reference       back() {
            return tail.get()->data;
        }

        const_reference back() const {
            return tail.get()->data;
        }

        // [list.modifiers], modifiers
        /*template <class... Args> reference emplace_front(Args&&... args);
        template <class... Args> reference emplace_back(Args&&... args);*/

        void push_front(const_reference x) { _push_front(x); }

        void push_front(T&& x) { _push_front(x); }

        void pop_front() {
            _size -= 1;
            if (_size == 0) {
                head = nullptr;
                tail = nullptr;
                return;
            }

            head = std::move(head.get()->next);
            head.get()->previous = nullptr;
            
        }

        void push_back(const_reference x) { _push_back(x); }

        void push_back(T&& x) { _push_back(x); }

        void pop_back() {
            _size -= 1;
            if (_size == 0) {
                head = nullptr;
                tail = nullptr;
                return;
            }
            tail = std::move(tail.get()->previous);
            tail.get()->next = nullptr;
        }

        //template <class... Args> iterator emplace(const_iterator position, Args&&... args);
        //iterator insert(const_iterator position, const T& x);
        //iterator insert(const_iterator position, T&& x);
        //iterator insert(const_iterator position, size_type n, const T& x);
        //template <class InputIterator>
        //iterator insert(const_iterator position, InputIterator first,
        //    InputIterator last);
        //iterator insert(const_iterator position, initializer_list<T> il);

        //iterator erase(const_iterator position);
        //iterator erase(const_iterator position, const_iterator last);
        //void     swap(list&)
        //    noexcept(allocator_traits<Allocator>::is_always_equal::value);
        //void     clear() noexcept;

        //// [list.ops], list operations
        //void splice(const_iterator position, list& x);
        //void splice(const_iterator position, list&& x);
        //void splice(const_iterator position, list& x, const_iterator i);
        //void splice(const_iterator position, list&& x, const_iterator i);
        //void splice(const_iterator position, list& x,
        //    const_iterator first, const_iterator last);
        //void splice(const_iterator position, list&& x,
        //    const_iterator first, const_iterator last);

        //void remove(const T& value);
        //template <class Predicate> void remove_if(Predicate pred);

        //void unique();
        //template <class BinaryPredicate>
        //void unique(BinaryPredicate binary_pred);

        //void merge(list& x);
        //void merge(list&& x);
        //template <class Compare> void merge(list& x, Compare comp);
        //template <class Compare> void merge(list&& x, Compare comp);

        //void sort();
        //template <class Compare> void sort(Compare comp);

        //void reverse() noexcept;

    private:
        using Node = list_node<value_type>;
        shared_ptr<Node> head{ nullptr };
        shared_ptr<Node> tail{ nullptr };
        size_type _size{};
        allocator_type allocator;

        void _push_front(const_reference x) {
            head = std::move(shared_ptr<Node>{ make_shared<Node>(x, std::move(head)) });
            head.get()->next.get()->previous = head;
            _size += 1;
        }

        void _push_back(const_reference x) {
            _size += 1;
            if (head == nullptr) {
                head = std::move(shared_ptr<Node>{ make_shared<Node>(x) });
                tail = head;
            }
            else {
                tail = std::move(shared_ptr<Node>{ make_shared<Node>(x, nullptr, std::move(tail)) });
                tail.get()->previous.get()->next = tail;
            }
        }
    };

    /*template<class InputIterator,
        class Allocator = allocator<typename std::iterator_traits<InputIterator>::value_type>>
        list(InputIterator, InputIterator, Allocator = Allocator())
        ->list<typename std::iterator_traits<InputIterator>::value_type, Allocator>;

    template <class T, class Allocator>
    bool operator==(const list<T, Allocator>& x, const list<T, Allocator>& y);
    template <class T, class Allocator>
    bool operator< (const list<T, Allocator>& x, const list<T, Allocator>& y);
    template <class T, class Allocator>
    bool operator!=(const list<T, Allocator>& x, const list<T, Allocator>& y);
    template <class T, class Allocator>
    bool operator> (const list<T, Allocator>& x, const list<T, Allocator>& y);
    template <class T, class Allocator>
    bool operator>=(const list<T, Allocator>& x, const list<T, Allocator>& y);
    template <class T, class Allocator>
    bool operator<=(const list<T, Allocator>& x, const list<T, Allocator>& y);*/

    // [list.special], specialized algorithms
    /*template <class T, class Allocator>
    void swap(list<T, Allocator>& x, list<T, Allocator>& y)
        noexcept(noexcept(x.swap(y)));*/
}

#endif //LIST_H
