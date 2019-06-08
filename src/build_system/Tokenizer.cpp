/**
 * Author: Daniel Simpkins
 * Date: 6/5/19
 */

/*
 * TODO: Finish documentation
 */

#include <cstring>
#include <fstream>
#include <string>
#include "Tokenizer.h"

#define SEPARATOR_SET "\t\n \r\v"
#define LOWER_CASE_ALPHA "abcdefghijklmnopqrstuvwxyz"
#define UPPER_CASE_ALPHA "ABCDEFGHIJKLMNOPQRSTUVWXYZ"
#define NUMBERS "0123456789"
#define IDENT_SYMBOLS "-_"
#define IDENT_START "_" LOWER_CASE_ALPHA UPPER_CASE_ALPHA
#define IDENT_CONTAINS IDENT_START IDENT_SYMBOLS NUMBERS
#define GROUPING_SYM "()"
#define BLOCK_SYM "{}"
#define DEFINITION_SCOPING ":="
#define OPERATORS "+-/*%^|&<>!=~"
#define SINGLE_OPERATORS "+-/*%^|&<>~!"
#define DELIMITER ",;"

#define CONTAINS(x, y) (x.find(y) != std::string::npos)

namespace astro {
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
    static type_map ConstructTypes() {
        type_map types_list;

        /*
         * Identifiers can start with a _ or a letter. But can have letters,
         * numbers, -, and _
         */
        types_list.insert(type_pair(IDENT_START, IDENT_CONTAINS));

        /* Number constants. */
        types_list.insert(type_pair("+-" NUMBERS, NUMBERS));

        /* Parenthesis grouping */
        types_list.insert(type_pair(GROUPING_SYM, ""));

        /* Block grouping */
        types_list.insert(type_pair(BLOCK_SYM, ""));

        /* Definitions and Scoping symbols */
        types_list.insert(type_pair(DEFINITION_SCOPING, DEFINITION_SCOPING));

        /* Operators */
        types_list.insert(type_pair(OPERATORS, OPERATORS));

        /* Delimiter */
        types_list.insert(type_pair(DELIMITER, ""));

        return types_list;
    }

    static classify_map ConstructClassMap() {
        classify_map map;

        /* Create the operators */
        std::string operators = std::string(SINGLE_OPERATORS);
        for(char c : operators) {
            map.insert(classify_pair(std::string(1, c), TT_OPERATOR));
        }
        map.insert(classify_pair(std::string("^^"), TT_OPERATOR));
        map.insert(classify_pair(std::string("||"), TT_OPERATOR));
        map.insert(classify_pair(std::string("&&"), TT_OPERATOR));
        map.insert(classify_pair(std::string("=="), TT_OPERATOR));
        map.insert(classify_pair(std::string("<="), TT_OPERATOR));
        map.insert(classify_pair(std::string("=>"), TT_OPERATOR));
        map.insert(classify_pair(std::string("!="), TT_OPERATOR));

        /* Declaration */
        map.insert(classify_pair(std::string(":="), TT_DEFINE));

        /* Assignment */
        map.insert(classify_pair(std::string("="), TT_ASSIGN));

        /* Source dereference */
        map.insert(classify_pair(std::string("::"), TT_SOURCE));

        /* Grouping */
        map.insert(classify_pair(std::string("("), TT_GROUP_OPEN));
        map.insert(classify_pair(std::string(")"), TT_GROUP_CLOSE));
        map.insert(classify_pair(std::string("{"), TT_BLOCK_OPEN));
        map.insert(classify_pair(std::string("}"), TT_BLOCK_CLOSE));

        /* Function declare */
        map.insert(classify_pair(std::string(":"), TT_FUNC_DECLARE));

        /* Delimiter */
        map.insert(classify_pair(std::string(","), TT_DELIMITER));
        map.insert(classify_pair(std::string(";"), TT_EXPR_END));

        /* Keywords */
        map.insert(classify_pair(std::string("if"), TT_KEYWORD));
        map.insert(classify_pair(std::string("else"), TT_KEYWORD));
        map.insert(classify_pair(std::string("do"), TT_KEYWORD));
        map.insert(classify_pair(std::string("while"), TT_KEYWORD));
        map.insert(classify_pair(std::string("break"), TT_KEYWORD));
        map.insert(classify_pair(std::string("continue"), TT_KEYWORD));
        map.insert(classify_pair(std::string("try"), TT_KEYWORD));
        map.insert(classify_pair(std::string("catch"), TT_KEYWORD));
        map.insert(classify_pair(std::string("return"), TT_KEYWORD));
        map.insert(classify_pair(std::string("func"), TT_KEYWORD));

        /* Types */
        map.insert(classify_pair(std::string("int8"), TT_PRIM_TYPE));
        map.insert(classify_pair(std::string("int16"), TT_PRIM_TYPE));
        map.insert(classify_pair(std::string("int32"), TT_PRIM_TYPE));
        map.insert(classify_pair(std::string("int64"), TT_PRIM_TYPE));
        map.insert(classify_pair(std::string("uint8"), TT_PRIM_TYPE));
        map.insert(classify_pair(std::string("uint16"), TT_PRIM_TYPE));
        map.insert(classify_pair(std::string("uint32"), TT_PRIM_TYPE));
        map.insert(classify_pair(std::string("uint64"), TT_PRIM_TYPE));
        map.insert(classify_pair(std::string("boolean"), TT_PRIM_TYPE));

        return map;
    }

    static bool IsIdentifier(const std::string& token) {
        /* Test to see if the token is a valid identifier */
        if(CONTAINS(std::string(IDENT_START), token[0])) {
            bool is_identifier = true;
            int offset = 0;
            while(is_identifier && offset < token.size()) {
                is_identifier = CONTAINS(std::string(IDENT_CONTAINS),
                                         token[offset]);
                offset++;
            }
            if(is_identifier)
                return true;
        }
        return false;
    }

    static bool IsNumber(const std::string& token) {
        if(CONTAINS(std::string("+-" NUMBERS), token[0])) {
            bool is_identifier = true;
            int offset = 0;
            while(is_identifier && offset < token.size()) {
                is_identifier = CONTAINS(std::string(NUMBERS), token[offset]);
                offset++;
            }
            if(is_identifier)
                return true;
        }
        return false;
    }

    static TokenType ClassifyType(const std::string& token, classify_map* map) {
        if(token.empty())
            return TT_UNKNOWN;

        if(map->count(token))
            return (*map)[token];

        if(IsIdentifier(token))
            return TT_IDENTIFIER;

        if(IsNumber(token))
            return TT_CONSTANT;

        return TT_UNKNOWN;
    }

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
     * Splits up and adds tokens from token based on the token types defined in
     * the map types.
     *
     * @param token A string of characters without whitespace.
     * @param types
     *  A map of types with the first part being a set of characters
     *  a token can start with and the second part being a set of characters a
     *  token can contain.
     */
    void Tokenizer::SmartTokenize(const std::string& token, type_map *types,
        classify_map *classes) {
        /* Iterate until end of the token is reached. */
        uint32_t offset = 0;
        while(offset < token.size()) {
            /* Find the token type in the map. */
            type_map::iterator itr;
            std::string *type_postfix = nullptr;
            for(itr = types->begin(); itr != types->end()
                && type_postfix == nullptr; itr++) {
                if (CONTAINS(itr->first, token[offset])) {
                    type_postfix = &(itr->second);
                }
            }

            /*
             * In the case that no map pair is found simply exit.
             */
            if(type_postfix != nullptr) {
                /* Find the whole token and add it to our set of tokens. */
                std::string sub_token;
                if(!type_postfix->empty()) {
                    sub_token = GetNextWordOrSeparator(token, offset,
                                                       *type_postfix);
                } else {
                    sub_token = std::string(1, token[offset]);
                }
                m_tokens.push_back({sub_token, ClassifyType(sub_token, classes)});
                offset += sub_token.size();
            } else {
                std::string sub_token = token.substr(offset,
                    token.size() - offset);
                m_tokens.push_back({sub_token, ClassifyType(sub_token, classes)});
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
        type_map types = ConstructTypes();

        classify_map classes = ConstructClassMap();

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
                SmartTokenize(token, &types, &classes);
            }
        }
    }

    /**
     * Reports the next token or empty string if none is available.
     *
     * @return The next token in the set if one is available.
     *  Otherwise returns an empty string.
     */
    struct Token Tokenizer::GetToken() {
        struct Token ret = PeekToken();
        m_cursor++;
        return ret;
    }

    struct Token Tokenizer::PeekToken() {
        if(m_cursor < m_tokens.size())
            return m_tokens[m_cursor];
        else
            return {"", TT_DONE};
    }
}