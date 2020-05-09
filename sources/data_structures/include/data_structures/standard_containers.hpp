#pragma once

#include <array>
#include <deque>
#include <forward_list>
#include <list>
#include <map>
#include <queue>
#include <set>
#include <stack>
#include <unordered_map>
#include <unordered_set>
#include <vector>

#include <fundamental/types.hpp>

namespace argon
{
template <typename T, sizet N>
using array = std::array<T, N>;

template <typename T, typename Allocator = std::allocator<T>>
using deque = std::deque<T, Allocator>;

template <typename T, typename Allocator = std::allocator<T>>
using list = std::list<T, Allocator>;

template <typename T, typename Allocator = std::allocator<T>>
using forward_list = std::forward_list<T, Allocator>;

template <typename Key, typename T, typename Compare = std::less<Key>,
																		typename Allocator = std::allocator<std::pair<const Key, T>>>
using map = std::map<Key, T, Compare, Allocator>;

template <typename Key, typename T, typename Compare = std::less<Key>,
																		typename Allocator = std::allocator<std::pair<const Key, T>>>
using multimap = std::multimap<Key, T, Compare, Allocator>;

template <typename Key, typename Compare = std::less<Key>, typename Allocator = std::allocator<Key>>
using multiset = std::multiset<Key, Compare, Allocator>;

template <typename T, typename Allocator = std::allocator<T>>
using vector = std::vector<T, Allocator>;

template <typename T, typename Container = vector<T>,
											typename Compare = std::less<typename Container::value_type>>
using priority_queue = std::priority_queue<T, Container, Compare>;

template <typename T, typename Container = deque<T>>
using queue = std::queue<T, Container>;

template <typename Key, typename Compare = std::less<Key>, typename Allocator = std::allocator<Key>>
using set = std::set<Key, Compare, Allocator>;

template <typename T, typename Container = deque<T>>
using stack = std::stack<T, Container>;

template <typename Key, typename T, typename Hash = std::hash<Key>,
																		typename KeyEqual = std::equal_to<Key>,
																		typename Allocator = std::allocator<std::pair<const Key, T>>>
using unordered_map = std::unordered_map<Key, T, Hash, KeyEqual, Allocator>;

template <typename Key, typename T, typename Hash = std::hash<Key>,
																		typename KeyEqual = std::equal_to<Key>,
																		typename Allocator = std::allocator<std::pair<const Key, T>>>
using unordered_multimap = std::unordered_multimap<Key, T, Hash, KeyEqual, Allocator>;

template <typename Key, typename Hash = std::hash<Key>, typename KeyEqual = std::equal_to<Key>,
																												typename Allocator = std::allocator<Key>>
using unordered_multiset = std::unordered_multiset<Key, Hash, KeyEqual, Allocator>;

template <typename Key, typename Hash = std::hash<Key>, typename KeyEqual = std::equal_to<Key>,
																												typename Allocator = std::allocator<Key>>
using unordered_set = std::unordered_set<Key, Hash, KeyEqual, Allocator>;
} //namespace argon
