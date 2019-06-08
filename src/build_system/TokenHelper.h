//
// Created by daniel on 6/7/19.
//

#ifndef ASTRO_TOKEN_HELPER_H
#define ASTRO_TOKEN_HELPER_H

#include <map>
#include <string>

namespace astro {
    enum TokenType {
        TT_IDENTIFIER = 0,
        TT_OPERATOR = 1,
        TT_DEFINE = 2,
        TT_SOURCE = 3,
        TT_ASSIGN = 4,
        TT_PRIM_TYPE = 5,
        TT_KEYWORD = 6,
        TT_DELIMITER = 7,
        TT_GROUP_OPEN = 8,
        TT_GROUP_CLOSE = 9,
        TT_BLOCK_OPEN = 10,
        TT_BLOCK_CLOSE = 11,
        TT_FUNC_DECLARE = 12,
        TT_EXPR_END = 13,
        TT_CONSTANT = 14,
        TT_UNKNOWN = 15
    };

    struct Token {
        std::string value;
        TokenType type;
    };

    typedef std::map<std::string, std::string> type_map;
    typedef std::pair<std::string, std::string> type_pair;
    typedef std::map<std::string, TokenType> classify_map;
    typedef std::pair<std::string, TokenType> classify_pair;

    static std::string TokenTypeToString(TokenType type) {
        std::string token_type_str[] = {
            "TT_IDENTIFIER",
            "TT_OPERATOR",
            "TT_DEFINE",
            "TT_SOURCE",
            "TT_ASSIGN",
            "TT_PRIM_TYPE",
            "TT_KEYWORD",
            "TT_DELIMITER",
            "TT_GROUP_OPEN",
            "TT_GROUP_CLOSE",
            "TT_BLOCK_OPEN",
            "TT_BLOCK_CLOSE",
            "TT_FUNC_DECLARE",
            "TT_EXPR_END",
            "TT_CONSTANT",
            "TT_UNKNOWN"
        };
        return token_type_str[type];
    }
}

#endif