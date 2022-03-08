/*
 * Copyright 2021-2022 Hewlett Packard Enterprise Development LP
 * Other additional copyright holders may be indicated within.
 *
 * The entirety of this work is licensed under the Apache License,
 * Version 2.0 (the "License"); you may not use this file except
 * in compliance with the License.
 *
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#ifndef CHPL_UTIL_ITERATION_H
#define CHPL_UTIL_ITERATION_H

#include <iterator>
#include <cassert>
#include <vector>
#include <type_traits>

namespace chpl {

/**
 Defines a read-only iterator over elements of a container type C
 */
template <typename C> class Iterable {
 private:
  typename C::const_iterator begin_;
  typename C::const_iterator end_;

 public:
  Iterable(const C &c) : begin_(c.cbegin()), end_(c.cend()) {}

  typename C::const_iterator begin() const { return begin_; }
  typename C::const_iterator end() const { return end_; }
};

/// \cond DO_NOT_DOCUMENT
namespace detail{

//
// Both ExpandingIteratorWrapper and UptoNullptrIteratorWrapper are to ease the
// transition from the forv_Vec and for_vector_allowing_0s family of macros
// which cannot always be replaced by a range for.
// In both cases, the Wrapper part is the begin()+end() pair and the
// inner struct is the actual iterator.

template<typename Container>
struct ExpandingIteratorWrapper {
  // ExpandingIterator keeps a reference to the container and an index, so that
  // dereferencing is always by index.
  struct ExpandingIterator {
    using iterator_category = std::forward_iterator_tag;
    using difference_type = std::ptrdiff_t;
    using value_type = typename Container::value_type;
    using pointer = typename Container::pointer;
    using reference = typename Container::reference;

    ExpandingIterator(Container& container, ssize_t i)
        : container_(container), i_(i) {}

    reference operator*() const { return container_[i_]; }
    pointer operator->() { return &container_[i_]; }

    // Prefix increment
    ExpandingIterator& operator++() {
      assert(i_ >= 0); // UB anyways to increment the end()
      i_ += 1;
      if ((size_t)i_ == container_.size()) {
        i_ = -1;
      }
      return *this;
    }

    // Postfix increment
    ExpandingIterator operator++(int) {
      ExpandingIterator tmp = *this;
      ++(*this);
      return tmp;
    }

    // Note, these are not checking the container has the same identity
    friend bool operator==(const ExpandingIterator& a,
                           const ExpandingIterator& b) {
      return a.i_ == b.i_;
    };
    friend bool operator!=(const ExpandingIterator& a,
                           const ExpandingIterator& b) {
      return a.i_ != b.i_;
    };

   private:
    Container& container_;
    // This should maybe use Container::size_type but we need a bit to
    // distinguish the end
    ssize_t i_;
  };

  ExpandingIteratorWrapper(Container& container) : container_(container) {}

  ExpandingIterator begin() {
    if (container_.size() == 0) return end();
    return ExpandingIterator(container_, 0);
  }
  ExpandingIterator end() {return ExpandingIterator(container_, -1);}

 private:
  Container& container_;
};

template<typename Container>
struct UptoNullptrIteratorWrapper {

  struct UptoNullptrIterator {
    using iterator_category = std::forward_iterator_tag;
    using difference_type = typename Container::difference_type;
    using value_type = typename Container::value_type;
    using pointer = typename Container::const_pointer;
    using reference = typename Container::const_reference;

    using iterator = typename Container::const_iterator;

    UptoNullptrIterator(iterator it, iterator end) : it_(it), end_(end) {}
    UptoNullptrIterator(const UptoNullptrIterator&) = default;

    const reference operator*() const { return *it_; }
    pointer operator->() { return it_; }

    // Prefix increment
    UptoNullptrIterator& operator++() {
      ++it_;
      if (it_ != end_ && *it_ == nullptr) {
        it_ = end_;
      }
      return *this;
    }

    // Postfix increment
    UptoNullptrIterator operator++(int) {
      UptoNullptrIterator tmp = *this;
      ++(*this);
      return tmp;
    }

    // Note, these are not checking the container has the same identity
    friend bool operator==(const UptoNullptrIterator& a,
                           const UptoNullptrIterator& b) {
      return a.it_ == b.it_;
    };
    friend bool operator!=(const UptoNullptrIterator& a,
                           const UptoNullptrIterator& b) {
      return a.it_ != b.it_;
    };

   private:
    iterator it_;
    iterator end_;
  };

  UptoNullptrIteratorWrapper(const Container& container) : container_(container) {}

  UptoNullptrIterator begin() {
    auto begin = container_.begin();
    auto end = container_.end();
    // We have to check if the first element is a nullptr to return end right away
    if (begin != end && *begin == nullptr) return this->end();
    return UptoNullptrIterator(begin, end);
  }
  UptoNullptrIterator end() {return UptoNullptrIterator(container_.end(), container_.end());}

 private:
  const Container& container_;
};

template <typename N>
struct is_vector { static const int value = 0; };

template <typename N, typename A>
struct is_vector<std::vector<N, A>> { static const int value = 1; };

}; // end namespace detail

// create an expandingIterator range which visits all elements of a vector, even
// those which are appended during iteration
// container has to be over a vector
// (this is more restrictive than necessary, but suits our usecase)
template <typename Container> auto expandingIterator(Container& container) {
  static_assert(detail::is_vector<Container>::value,
                "expandingIterator should only be used on std::vector today");
  return detail::ExpandingIteratorWrapper<Container>(container);
}

// create an uptoNullptrIterator range which visits all elements of a vector up
// to the first nullptr
// container has to be over a pointer type
template <typename Container> auto uptoNullptrIterator(const Container& container) {
  static_assert(detail::is_vector<Container>::value,
                "uptoNullptrIterator should only be used on std::vector today");
  static_assert(std::is_pointer<typename Container::value_type>::value,
                "uptoNullptrIterator should only be used on container of pointers");
  return detail::UptoNullptrIteratorWrapper<Container>(container);
}
/// \endcond DO_NOT_DOCUMENT

} // namespace chpl

#endif
