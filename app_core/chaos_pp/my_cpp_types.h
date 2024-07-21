/*
 * my_cpp_types.hpp
 *
 *  Created on: Jul 16, 2024
 *      Author: admin
 */

#ifndef CHAOSPP_MY_CPP_TYPES_HPP_
#define CHAOSPP_MY_CPP_TYPES_HPP_

#include <cstdint>  	// For integer types
#include <cstddef>  	// For size_t and ptrdiff_t
#include <type_traits>  // For static_assert

// Define all types --------------------------------------------------------

using uni = void;

using u8 = std::uint8_t;
using u16 = std::uint16_t;
using u24 = std::uint32_t; // Assuming no native 24-bit type, use uint32_t
using u32 = std::uint32_t;
using u64 = std::uint64_t;

using c8 = char;
using i8 = std::int8_t;
using i16 = std::int16_t;
using i24 = std::int32_t; // Assuming no native 24-bit type, use int32_t
using i32 = std::int32_t;
using i64 = std::int64_t;

using f32 = float;
using f64 = double;
using f128 = long double; // Platform-dependent type

using b = bool;

// Platform-dependent register types --------------------------------------

using reg = std::size_t;
using sreg = std::ptrdiff_t;

// User types checks ------------------------------------------------------

static_assert(sizeof(u8) == 1, "size of uint8 type must be equal to 1, change typedef u8");
static_assert(sizeof(u16) == 2, "size of uint16 type must be equal to 2, change typedef u16");
static_assert(sizeof(u24) == 4, "size of uint24 type must be equal to 4, change typedef u24");
static_assert(sizeof(u32) == 4, "size of uint32 type must be equal to 4, change typedef u32");
static_assert(sizeof(u64) == 8, "size of uint64 type must be equal to 8, change typedef u64");

static_assert(sizeof(c8) == 1, "size of char type must be equal to 1, change typedef c8");
static_assert(sizeof(i8) == 1, "size of int8 type must be equal to 1, change typedef i8");
static_assert(sizeof(i16) == 2, "size of int16 type must be equal to 2, change typedef i16");
static_assert(sizeof(i24) == 4, "size of int24 type must be equal to 4, change typedef i24");
static_assert(sizeof(i32) == 4, "size of int32 type must be equal to 4, change typedef i32");
static_assert(sizeof(i64) == 8, "size of int64 type must be equal to 8, change typedef i64");

static_assert(sizeof(f32) == 4, "size of float type must be equal to 4, change typedef f32");
static_assert(sizeof(f64) == 8, "size of double type must be equal to 8, change typedef f64");
//static_assert(sizeof(f128) == 16, "size of long double type must be equal to 16, change typedef f128"); // Platform-dependent

static_assert(sizeof(float) == sizeof(u32), "Unexpected float format");
static_assert(sizeof(double) == sizeof(u64), "Unexpected double format");

static_assert(sizeof(b) == 1, "size of bool type must be equal to 1, change typedef b");

static_assert(sizeof(reg) == sizeof(uni*), "size of reg type must be equal to pointer size, change typedef reg");
static_assert(sizeof(sreg) == sizeof(uni*), "size of sreg type must be equal to pointer size, change typedef sreg");

#endif /* CHAOSPP_MY_CPP_TYPES_HPP_ */
