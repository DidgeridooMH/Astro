/**
 * This component reads in data from a file and break up its content into tokens.
 * A token being a string of symbols, letters, and numbers that be placed
 * together according to the grammar of the language.
 *
 * Usage:
 *  Initialize the tokenizer with a filename.
 *  Call GetToken until an empty string is returned.
 *
 * Author: Daniel Simpkins
 * Date: 6/5/19
 */

#ifndef ASTRO_TOKENIZER_H
#define ASTRO_TOKENIZER_H

#include <iostream>
#include <list>
#include <vector>
#include <map>

namespace astro {
    class Tokenizer {
    public:
        explicit Tokenizer(const std::string& filename);

        std::string GetToken();
    private:
        void Tokenize(char *p_buffer, size_t length);
        void SmartTokenize(const std::string& token,
            std::map<std::string, std::string> types);

        std::vector<std::string> m_tokens;
        long                     m_cursor;
    };
}

#endif //ASTRO_TOKENIZER_H
