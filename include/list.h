/*
* array.h
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
          std::unique_ptr, std::make_unique, std::initializer_list;

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
      /*using difference_type = ;
        using iterator = ;
        using const_iterator = ;
        using reverse_iterator = std::reverse_iterator<iterator>;
        using const_reverse_iterator = std::reverse_iterator<const_iterator>;*/

        list() = default;
        
        list(initializer_list<T> vals) {
            this->assign(vals);
        }

        ~list() {

        }


        /*void assign(size_type n, const T& t) {

        }*/

        void assign(initializer_list<T> vals) {
            for (T val : vals)
                this->push_back(val);
        }

        void push_back(const T& x) {
            auto temp = unique_ptr<Node>{ make_unique<Node>(x) };
            if (head == nullptr) {
                temp->next = std::move(head);
                head = std::move(temp);
            }
            else {
                temp->next = std::move(tail);
                tail = std::move(temp);
            }
        }

        void push_back(T&& x) {
            auto temp = unique_ptr<Node>{ make_unique<Node>(x) };
            if (head == nullptr){
                temp->next = std::move(head);
                head = std::move(temp);
            }
            else {
                temp->next = std::move(tail);
                tail = std::move(temp);
            }
        }

    private:
        struct Node {
            T data;
            unique_ptr<Node> next;
            Node(int data) : data{ data }, next{ nullptr } {}
        };
        unique_ptr<Node> head{ nullptr };
        unique_ptr<Node> tail{ nullptr };
	};
}

#endif //LIST_H
