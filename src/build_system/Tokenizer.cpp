/**
 * Author: Daniel Simpkins
 * Date: 6/5/19
 */

#include <cstring>
#include <fstream>
#include <string>
#include "Tokenizer.h"

#define SEPARATOR_SET "\t\n \r\v"
#define LOWER_CASE_ALPHA "abcdefghijklmnopqrstuvwxyz"
#define UPPER_CASE_ALPHA "ABCDEFGHIJKLMNOPQRSTUVWXYZ"
#define SYMBOLS "~`!@#$%^&*()-_=+{}[]|\\:;\"\'<,>.?/"
#define NUMBERS "0123456789"
#define INDENT_SYMBOLS "-_"

#define CONTAINS(x, y) (x.find(y) != std::string::npos)

namespace astro {
    /**
     * Opens the file, filename, and breaks the contents into tokens as defined
     * by the Astro language.
     *
     * @param filename Name of the file to tokenize.
     */
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

        /* Create a set of tokens from the given file buffer. */
        Tokenize(file_buffer, (size_t)length);

        delete[] file_buffer;

        /* Close the file. */
        file_in.close();
    }

    /**
     * Reports the next string of characters or whitespace in the buffer
     * starting at offset.
     *
     * @param buffer The buffer of characters in which to retrieve string from.
     * @param offset The starting point the buffer.
     * @param separators A set of white space characters.
     * @return
     *  If the character at offset in buffer is a normal character,
     *  reports a string of characters until the end of the buffer
     *  or until a white space character is found. If the character at offset
     *  in buffer is a whitespace character, reports a string of whitespace
     *  characters until the end of the buffer or until a normal character is
     *  found.
     */
    static std::string GetNextWordOrSeparator(std::string buffer,
        uint32_t offset, const std::string& separators) {
        /* Save the starting point. */
        uint32_t start = offset;

        /* Check if the first letter is a character or whitespace. */
        bool is_sep = CONTAINS(separators, buffer[offset]);

        /* Iterate until the end of the buffer or an opposite type is found. */
        while(offset < buffer.size()
            && (is_sep == CONTAINS(separators, buffer[offset]))) {
            offset++;
        }

        /* Return chunk starting from initial offset until the end of word. */
        return buffer.substr(start, offset - start);
    }

    /**
     * Creates a map of string pairs. The first part of the pair is a set of
     * characters that a token is allowed to start with. The second part of the
     * pair is a set of characters that the token is allowed to contain after
     * the first character.
     *
     * @return
     *  A map of string pairs with the first part being a set of allowed initial
     *  characters and the second part a set of allowed containing characters.
     */
    static std::map<std::string, std::string> ConstructTypes() {
        std::map<std::string, std::string> types_list;

        /*
         * Identifiers can start with a _ or a letter. But can have letters,
         * numbers, -, and _
         */
        types_list.insert(
            std::pair<std::string, std::string>(
                "_" LOWER_CASE_ALPHA UPPER_CASE_ALPHA,
                LOWER_CASE_ALPHA UPPER_CASE_ALPHA NUMBERS INDENT_SYMBOLS)
                );

        /* Number constants. */
        types_list.insert(
            std::pair<std::string, std::string>(NUMBERS, NUMBERS)
            );

        /* Symbol strings. */
        types_list.insert(
            std::pair<std::string, std::string>(SYMBOLS, SYMBOLS)
            );

        return types_list;
    }

    /**
     * Splits up and adds tokens from token based on the token types defined in
     * the map types.
     *
     * @param token A string of characters without whitespace.
     * @param types
     *  A map of types with the first part being a set of characters
     *  a token can start with and the second part being a set of characters a
     *  token can contain.
     */
    void Tokenizer::SmartTokenize(const std::string& token,
        std::map<std::string, std::string> types) {
        /* Iterate until end of the token is reached. */
        uint32_t offset = 0;
        while(offset < token.size()) {
            /* Find the token type in the map. */
            std::map<std::string, std::string>::iterator itr;
            std::string *type_postfix = nullptr;
            for(itr = types.begin(); itr != types.end(); itr++) {
                if (CONTAINS(itr->first, token[offset]))
                    type_postfix = &(itr->second);
            }

            /*
             * In the case that no map pair is found simply exit.
             */
            if(type_postfix != nullptr) {
                /* Find the whole token and add it to our set of tokens. */
                std::string sub_token = GetNextWordOrSeparator(token, offset,
                                                               *type_postfix);
                m_tokens.push_back(sub_token);
                offset += sub_token.size();
            } else {
                std::string sub_token = token.substr(offset,
                    token.size() - offset);
                m_tokens.push_back(sub_token);
                offset += sub_token.size();
            }
        }
    }

    /**
     * Breaks up the buffer pointed to by p_buffer into tokens and add
     * them to a set of tokens in the current object.
     *
     * @param p_buffer Buffer to tokenize.
     * @param length Length of buffer pointed to by p_buffer.
     */
    void Tokenizer::Tokenize(char *p_buffer, size_t length) {
        /* Construct a set of white space characters */
        const std::string separators = std::string(SEPARATOR_SET);

        /* Construct a token type map. */
        std::map<std::string, std::string> types = ConstructTypes();

        /* Stringify the character buffer */
        std::string str_buffer = std::string(p_buffer, length);

        /*
         * Iterate through the entire buffer looking for words to add to the
         * our token set.
         */
        uint32_t offset = 0;
        while(offset < str_buffer.size()) {
            std::string token = GetNextWordOrSeparator(str_buffer, offset,
                separators);
            offset += token.size();
            if(!CONTAINS(separators, token[0])) {
                SmartTokenize(token, types);
            }
        }
    }

    /**
     * Reports the next token or empty string if none is available.
     *
     * @return The next token in the set if one is available.
     *  Otherwise returns an empty string.
     */
    std::string Tokenizer::GetToken() {
        if(m_cursor < m_tokens.size())
            return m_tokens[m_cursor++];
        else
            return std::string("");
    }
}