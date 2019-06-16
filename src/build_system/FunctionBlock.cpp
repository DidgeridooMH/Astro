//
// Created by daniel on 6/8/19.
//

#include <iostream>
#include "FunctionBlock.h"

namespace astro {
    bool FunctionBlock::ParseBlock(astro::Tokenizer *tokenizer) {
        Token start_token = tokenizer->GetToken();
        if(!(start_token.value == "func")) {
            std::cerr << "Error parsing function: expected \"func\" instead of \""
                << start_token.value << "\"" << std::endl;
            return false;
        }

        Token delim;
        do {
            Token arg = tokenizer->GetToken();
            if(arg.type != TT_PRIM_TYPE) {
                std::cerr << "Error parsing function: expected a type instead of \""
                    << arg.value << "\"" << std::endl;
                return false;
            }

            PrimType type = StringToPrimType(arg.value);
            if(type == PT_UNKNOWN) {
                std::cerr << "Error parsing function: expected a type instead of \""
                          << arg.value << "\"" << std::endl;
                return false;
            }

            m_ret_count++;
            m_ret_types.push_back(type);
            delim = tokenizer->GetToken();
        } while(delim.type == TT_DELIMITER);

        if(delim.type != TT_FUNC_DECLARE) {
            std::cerr << "Error parsing function: expected a function delimiter "
                 << "instead of \"" << delim.value << "\"" << std::endl;
            return false;
        }

        Token function_name = tokenizer->GetToken();
        if(function_name.type != TT_IDENTIFIER) {
            std::cerr << "Error parsing function: expected an identifier instead"
                << " of \"" << function_name.value << "\"" << std::endl;
            return false;
        }

        return false;
    }
}
