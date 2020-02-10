#pragma once

template <typename>
struct IsDataSpecialization: std::false_type {};

template <size_t S>
struct IsDataSpecialization<Data<S>>: std::true_type {};

