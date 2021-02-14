#pragma once
#include <iostream>
#include <fstream>
#include <cstdlib>
#include <ctime>
#include <map>
#include <unordered_map>

//I got a base from lesson and upgrade it
//Now it's LFU



template <class T,
          class KeyT = int>
class cache_t {

	//Useful using's
	//------------------------------------------
	using multimap_iter_t = typename std::multimap<int, T>::iterator;
	using hash_iter_t     = typename std::unordered_map<KeyT, multimap_iter_t>::iterator;
	//------------------------------------------

	public:

		explicit cache_t(size_t init_sz) noexcept :
			sz_   { init_sz },
			hash_ {},
			cache_{}
		{}

		////////////////
		// return true if we found list is full
		////////////////
		bool is_full() const noexcept;

		////////////////
		// return true if we found an element inside cache
		////////////////
		bool lookup(const T* elem);

		void increase_freq(multimap_iter_t& obj, const T* elem);

		////////////////
		// print list elements - for debugging
		////////////////
		void print_cash(std::ofstream& out) const;

	private:

		size_t sz_;

		std::unordered_map
			<KeyT, multimap_iter_t> hash_;

		//second param - frequency
		std::multimap <int, T>    cache_;
};



//Relization functions in class cache_t
/////////////////////////////////////////////////////////////
template <class T,
          class KeyT>
bool cache_t<T, KeyT>::is_full() const noexcept
{
	return hash_.size() == sz_;
}

template <class T,
          class KeyT /* = int*/>
bool cache_t<T, KeyT>::lookup(const T* elem) 
{
	hash_iter_t hit = hash_.find(elem->get_id());

	if (hit == hash_.end()) { //elem wasn't found

		if (is_full()) {
			//we have to delete the most useless and oldest
			auto cur_deleter = cache_.begin();

			hash_.erase (cur_deleter->second.get_id());
			cache_.erase(cur_deleter);
		}

		//just for understanding
		//////
		constexpr int first_found = 1;
		multimap_iter_t new_iter = cache_.insert(std::make_pair(first_found, *elem));
		//////

		hash_[elem->get_id()] = new_iter;
		return false;
	}

	//element was found
	increase_freq(hit->second, elem);

	return true;
}

template <class T,
          class KeyT>
void cache_t<T, KeyT>::increase_freq(multimap_iter_t& obj, const T* elem)
{
	int new_freq = obj->first + 1;
	cache_.erase(obj);

	multimap_iter_t new_iter = cache_.insert(std::make_pair(new_freq, *elem));
	hash_[elem->get_id()] = new_iter;
}

template <class T,
          class KeyT>
void cache_t<T, KeyT>::print_cash(std::ofstream& out) const
{
	for (auto elem : cache_) {
		out << "(" << elem.second.get_id() << ", " << elem.first << ") ";
	}
	out << std::endl;
}
/////////////////////////////////////////////////////////////