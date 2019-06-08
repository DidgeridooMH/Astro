//
// Created by daniel on 6/8/19.
//

#ifndef ASTRO_VARIABLE_H
#define ASTRO_VARIABLE_H

#include <string>
#include "Tokenizer.h"
#include "PrimitiveTypes.h"

namespace astro {
    class Variable {
    public:
        Variable(uint64_t id);

        bool ParseVariable(Tokenizer * tokenizer) {return false;};

        PrimType m_type;
        std::string m_name;
        uint64_t m_id;
    };
}

#endif //ASTRO_VARIABLE_H
