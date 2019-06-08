#include <iostream>
#include "build_system/Tokenizer.h"

using namespace astro;

int main() {
    Tokenizer* tokenizer = new Tokenizer(std::string("../examples/test.ast"));

    Token token;
    while((token = tokenizer->GetToken()).value.length() > 0) {
        std::cout << token.value << " : " << TokenTypeToString(token.type) << std::endl;
    }

    return 0;
}