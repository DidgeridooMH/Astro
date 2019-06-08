//
// Created by daniel on 6/7/19.
//

#include "FileBlock.h"

namespace astro {
    FileBlock::FileBlock(const std::string &filename)
        : m_filename(filename) {}

    FileBlock::~FileBlock() {
        func_map::iterator func_itr = m_functions.begin();
        while(func_itr != m_functions.end()) {
            delete func_itr->second;
            func_itr++;
        }

        var_map::iterator var_itr = m_variables.begin();
        while(var_itr != m_variables.end()) {
            delete var_itr->second;
            var_itr++;
        }
    }

    bool FileBlock::ParseFile(astro::Tokenizer *tokenizer) {
        uint32_t var_id = 0;
        bool error = false;

        struct Token token;
        while(!error && (token = tokenizer->PeekToken()).type != TT_DONE) {
            if(token.type == TT_KEYWORD) {
                if(token.value == "func") {
                    FunctionBlock *block = new FunctionBlock();
                    error = !block->ParseBlock(tokenizer);
                    if(!error)
                        m_functions.insert(func_pair(block->m_name, block));
                    else
                        delete block;
                } else if(token.type == TT_PRIM_TYPE) {
                    Variable *variable = new Variable(var_id++);
                    error = !variable->ParseVariable(tokenizer);
                    if(!error)
                        m_variables.insert(var_pair(variable->m_name, variable));
                    else
                        delete variable;
                } else {
                    std::cerr << "Error parsing token in file \"" << m_filename
                        << "\": Unexpected token \"" << token.value << "\""
                        << std::endl;
                }
            } else {
                error = true;
            }
        }

        return error;
    }
}
