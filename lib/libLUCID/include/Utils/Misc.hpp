///////////////////////////////////////////////////////////////////////////////
/// \file Misc.hpp
/// \author Hector Stalker <hstalker0@gmail.com>
/// \version 0.1
///
/// \brief Miscellaneous utilities for C++11
///
/// \copyright Copyright (c) 2014, Hector Stalker. All rights reserved. 
/// This file is under the Simplified (2-clause) BSD license
/// For conditions of distribution and use, see:
/// http://opensource.org/licenses/BSD-2-Clause
/// or read the 'LICENSE.md' file distributed with this code

#ifndef UTILS_MISC_HPP
#define UTILS_MISC_HPP

#include <memory>
#include <type_traits>
#include <utility>


namespace utils {

/// Uses S.T.L's make_unique code

template <typename T, typename... Args>
std::unique_ptr<T> make_unique_helper(std::false_type, Args&&... args) {
  return std::unique_ptr<T>(new T(std::forward<Args>(args)...));
}

template <typename T, typename... Args>
std::unique_ptr<T> make_unique_helper(std::true_type, Args&&... args) {
   static_assert(std::extent<T>::value == 0,
       "make_unique<T[N]>() is forbidden, please use make_unique<T[]>().");

   typedef typename std::remove_extent<T>::type U;
   return std::unique_ptr<T>(new U[sizeof...(Args)]{std::forward<Args>(args)...});
}

///////////////////////////////////////////////////////////////////////////////
/// \brief A unique_ptr version of make_shared. Heap allocates and constructs
/// the object
/// \param args The arguments for the constructor of the object being pointed
/// to
template <typename T, typename... Args>
std::unique_ptr<T> make_unique(Args&&... args) {
   return make_unique_helper<T>(std::is_array<T>(), std::forward<Args>(args)...);
}

} // utils

#endif // UTILS_MISC_HPP
