//
// Created by daniel on 6/7/19.
//

#include <iostream>
#include "Program.h"

namespace astro {
    Program::~Program() {
        auto itr = m_file_blocks.begin();
        while(itr != m_file_blocks.end()) {
            delete itr->second;
            itr++;
        }
    }

    bool Program::AddFileBlock(const std::string &filename,
                               Tokenizer *tokenizer) {
        if(m_file_blocks.count(filename)) {
            std::cerr << "Duplicate file block scope found: "
                << filename << std::endl;
            return false;
        }

        FileBlock *block = new FileBlock(filename);
        if(!block->ParseFile(tokenizer)) {
            delete block;
            return false;
        }

        m_file_blocks.insert(std::pair<std::string, FileBlock*>(filename, block));
        return true;
    }

    FileBlock* Program::GetFileBlock(const std::string &filename) {
        if(m_file_blocks.count(filename))
            return m_file_blocks[filename];
        return nullptr;
    }
}
