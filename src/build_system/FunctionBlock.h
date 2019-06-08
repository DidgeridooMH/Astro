//
// Created by daniel on 6/8/19.
//

#ifndef ASTRO_FUNCTIONBLOCK_H
#define ASTRO_FUNCTIONBLOCK_H

#include "Tokenizer.h"
#include "PrimitiveTypes.h"

namespace astro {
    class FunctionBlock {
    public:
        bool ParseBlock(Tokenizer *tokenizer);

        std::string m_name;

    private:
        int m_ret_count;
        std::vector<PrimType> m_ret_types;
    };
}


#endif //ASTRO_FUNCTIONBLOCK_H
