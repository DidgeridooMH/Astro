#include <iostream>
#include "build_system/Tokenizer.h"
#include "build_system/Program.h"

using namespace astro;

int main() {
    Tokenizer* tokenizer = new Tokenizer(std::string("../examples/test.ast"));

    Program *program = new Program();
    program->AddFileBlock(std::string("test.ast"), tokenizer);

    return 0;
}