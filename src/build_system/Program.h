//
// Created by daniel on 6/7/19.
//

#ifndef ASTRO_PROGRAM_H
#define ASTRO_PROGRAM_H

#include <map>
#include <string>
#include "FileBlock.h"
#include "Tokenizer.h"

namespace astro {
    class Program {
    public:
        ~Program();

        bool        AddFileBlock(const std::string& filename, Tokenizer *file_block);
        FileBlock*  GetFileBlock(const std::string& filename);

    private:
        std::map<std::string, FileBlock*> m_file_blocks;
    };
}

#endif //ASTRO_PROGRAM_H
