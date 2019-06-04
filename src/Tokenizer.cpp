#include <cstring>
#include <fstream>
#include "Tokenizer.h"

#define SEPARATOR_SET "\t\n \r\v"

namespace astro {
    Tokenizer::Tokenizer(const std::string &filename)
        : m_cursor(0)
    {
        /* Open the source file. */
        std::ifstream file_in(filename, std::ios::ate);

        if(!file_in.is_open()) {
            std::cerr << "Failed to open file \"" << filename << "\"..."
                << std::endl;
            return;
        }

        /* Calculate it's length. */
        long length = file_in.tellg();
        file_in.seekg(std::ios::beg);

        /* Allocate and read the contents into a buffer. */
        char *file_buffer = new char[length];
        file_in.read(file_buffer, length);

        /* Close the file. */
        file_in.close();
    }

    void Tokenizer::Tokenize(char *pBuffer) {
        /* TODO: Utilize nextWordOrSeparator algorithm to token buffer */
    }

    std::string Tokenizer::GetToken() {
        if(m_cursor < m_tokens.size())
            return m_tokens[m_cursor++];
        else
            return std::string("");
    }
}