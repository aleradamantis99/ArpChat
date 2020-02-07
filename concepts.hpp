#pragma once

template <typename T, template <size_t> typename Temp>
struct is_specialization: std::false_type {};

template <template <size_t> typename Temp, size_t S>
struct is_specialization<Temp<S>, Temp>: std::true_type {};

