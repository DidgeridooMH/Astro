//
// Created by daniel on 6/7/19.
//

#ifndef ASTRO_FILEBLOCK_H
#define ASTRO_FILEBLOCK_H

#include "Tokenizer.h"
#include "FunctionBlock.h"
#include "Variable.h"

namespace astro {

    typedef std::map<std::string, FunctionBlock*> func_map;
    typedef std::pair<std::string, FunctionBlock*> func_pair;
    typedef std::map<std::string, Variable*> var_map;
    typedef std::pair<std::string, Variable*> var_pair;

    class FileBlock {
    public:
        explicit FileBlock(const std::string& filename);
        ~FileBlock();

        bool ParseFile(Tokenizer *tokenizer);

        std::string m_filename;

    private:
        func_map    m_functions;
        var_map     m_variables;
    };
}


#endif //ASTRO_FILEBLOCK_H
