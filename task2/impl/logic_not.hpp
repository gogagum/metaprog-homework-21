//
// Created by gogagum on 14.01.2022.
//

#ifndef TASK2_LOGIC_NOT_HPP
#define TASK2_LOGIC_NOT_HPP

namespace TypeLists::Impl {
    /*
     * ApplyLogicNotToValue
     * --------------------
     * Value now is not value
     */
    template <class P>
    struct ApplyLogicNotToValue {
        const static bool Value = !P::Value;
    };
}

#endif //TASK2_LOGIC_NOT_HPP
