//
// Created by gogagum on 24.12.2021.
//

#ifndef TASK2__COMMONS_METAFUNCTIONS_HPP_
#define TASK2__COMMONS_METAFUNCTIONS_HPP_
#pragma once

#define CURRY(NameSpace, Name, K1, K2)                 \
template<K1 k1>                             \
struct C##Name {                            \
	template<K2 k2>                         \
	using Curry = NameSpace::Name<k1, k2>;  \
}

#endif //TASK2__COMMONS_METAFUNCTIONS_HPP_
