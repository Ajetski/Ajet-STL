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
* - https://timsong-cpp.github.io/cppwp/n4659/array
* - https://timsong-cpp.github.io/cppwp/n4659/array.syn
*/

#ifndef ARRAY_H
#define ARRAY_H

#include <iterator>
#include <stdexcept>

namespace ajet
{
	using std::size_t, std::unique_ptr;

	template <typename T, size_t N>
	struct array
	{
		using value_type = T;
		using size_type = size_t;
		using reference = value_type &;
		using const_reference = const value_type &;
		using pointer = value_type *;
		using const_pointer = const value_type *;
		using iterator = pointer;
		using const_iterator = const_pointer;
		using reverse_iterator = std::reverse_iterator<iterator>;
		using const_reverse_iterator = std::reverse_iterator<const_iterator>;

		value_type values[N]{};

		constexpr reference at(size_type pos)
		{
			if (pos < 0 || pos >= N)
				throw std::out_of_range("");
			return values[pos];
		}

		constexpr const_reference at(size_type pos) const
		{
			if (pos < 0 || pos >= N)
				throw std::out_of_range("");
			return values[pos];
		}

		constexpr reference operator[](size_type pos) { return values[pos]; }
		constexpr const_reference operator[](size_type pos) const { return values[pos]; }

		constexpr reference front() { return values[0]; }
		constexpr const_reference front() const { return values[0]; }

		constexpr reference back() { return values[N - 1]; }
		constexpr const_reference back() const { return values[N - 1]; }

		constexpr T *data() noexcept { return values; }
		constexpr const T *data() const noexcept { return values; }

		constexpr iterator begin() noexcept { return values; }
		constexpr const_iterator begin() const noexcept { return values; }
		constexpr const_iterator cbegin() const noexcept { return values; }

		constexpr iterator end() noexcept { return values + N; }
		constexpr const_iterator end() const noexcept { return values + N; }
		constexpr const_iterator cend() const noexcept { return values + N; }

		constexpr reverse_iterator rbegin() noexcept { return values + N - 1; }
		constexpr const_reverse_iterator  rbegin() const noexcept { return values + N - 1; }
		constexpr const_reverse_iterator crbegin() const noexcept { return values + N - 1; }

		constexpr iterator rend() noexcept { return values; }
		constexpr const_iterator rend() const noexcept { return values; }
		constexpr const_iterator crend() const noexcept { return values; }

		constexpr bool empty() const noexcept { return N > 0; }
		constexpr size_type size() const noexcept { return max_size(); }
		constexpr size_type max_size() const noexcept { return N; }

		void fill(const T &value) { for (reference elem : values) elem = value; }

		void swap(array &other) noexcept(std::is_nothrow_swappable_v<T>) {
			for (size_type i = 0; i < N; ++i)
				std::swap(this->at(i), other.at(i));
		}
	};

	template <class T, size_t N>
	bool operator==(const array<T, N> &lhs,
					const array<T, N> &rhs)
	{
		for(size_t i = 0; i < N; ++i) {
			if(lhs.at(i) != rhs.at(i))
				return false;
		}
		return true;
	}

	template <class T, size_t N>
	bool operator!=(const array<T, N>& lhs,
		const array<T, N>& rhs) {
		return !(lhs == rhs);
	}

	template <class T, size_t N>
	bool operator<(const array<T, N>& lhs,
		const array<T, N>& rhs)
	{
		for (size_t i = 0; i < N; ++i) {
			if (lhs.at(i) >= rhs.at(i))
				return false;
		}
		return true;
	}

	template <class T, size_t N>
	bool operator<=(const array<T, N>& lhs,
		const array<T, N>& rhs) {
		return lhs < rhs || lhs == rhs;
	}

	template <class T, size_t N>
	bool operator>(const array<T, N>& lhs,
		const array<T, N>& rhs)
	{
		for (size_t i = 0; i < N; ++i) {
			if (lhs.at(i) <= rhs.at(i))
				return false;
		}
		return true;
	}

	template <class T, size_t N>
	bool operator>=(const array<T, N>& lhs,
		const array<T, N>& rhs) {
		return lhs > rhs || lhs == rhs;
	}
}

#endif //ARRAY_H
