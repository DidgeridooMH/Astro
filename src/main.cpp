#include <iostream>
#include "Tokenizer.h"

using namespace astro;

int main() {
    Tokenizer* tokenizer = new Tokenizer(std::string("../examples/test.ast"));

    std::string token;
    while((token = tokenizer->GetToken()).length() > 0) {
        std::cout << token << std::endl;
    }

    return 0;
}