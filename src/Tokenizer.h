#ifndef ASTRO_TOKENIZER_H
#define ASTRO_TOKENIZER_H

#include <iostream>
#include <vector>

namespace astro {
    class Tokenizer {
    public:
        explicit Tokenizer(const std::string& filename);

        std::string GetToken();
    private:
        void Tokenize(char *pBuffer);

        std::vector<std::string> m_tokens;
        long                     m_cursor;
    };
}

#endif //ASTRO_TOKENIZER_H
