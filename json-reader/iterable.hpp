#pragma once

namespace json {
	/**
	* @brief Iterable interface.
	*/
	template<class Container>
	struct iterable {
	/**
	* @brief Type alias to the array's iterator.
	*/
	using iterator = typename Container::iterator;
	/**
	* @brief Type alias to the array's const iterator.
	*/
	using const_iterator = typename Container::const_iterator;
	/**
	* @brief Type alias to the array's reverse iterator.
	*/
	using reverse_iterator = typename Container::reverse_iterator;
	/**
	* @brief Type alias to the array's const reverse iterator.
	*/
	using const_reverse_iterator = typename Container::const_reverse_iterator;
	/**
	* @brief Begin iterator.
	* @return The begin iterator of the container.
	*/
	virtual iterator begin() noexcept = 0;
	/**
	* @brief End iterator.
	* @return The end iterator of the container.
	*/
	virtual iterator end() noexcept = 0;
	/**
	* @brief Reverse begin iterator.
	* @return The beginning of the reverse iterator of the container.
	*/
	virtual reverse_iterator rbegin() noexcept = 0;
	/**
	* @brief Reverse end iterator.
	* @return The end of the reverse iterator of the container.
	*/
	virtual reverse_iterator rend() noexcept = 0;
	/**
	* @brief Begin const iterator.
	* @return The beginning of the const iterator of the container.
	*/
	virtual const_iterator cbegin() const noexcept = 0;
	/**
	* @brief End const iterator.
	* @return The end of the reverse iterator of the container.
	*/
	virtual const_iterator cend() const noexcept = 0;
	/**
	* @brief Begin const reverse iterator.
	* @return The beginning of the const reverse iterator of the container.
	*/
	virtual const_reverse_iterator crbegin() const noexcept = 0;
	/**
	* @brief End const reverse iterator.
	* @return The end of the const reverse iterator of the container.
	*/
	virtual const_reverse_iterator crend() const noexcept = 0;
	};
}
