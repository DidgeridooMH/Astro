#include <iostream>
#include "build_system/Tokenizer.h"
#include "build_system/Program.h"
#include "utils/Logger.h"

#define VERSION "0.0.2a"

using namespace astro;

int main() {
    utils::Logger debug_log("MAIN", LOG_LEVEL_INFO);
    debug_log.Info("Astro v%s", VERSION);
    
    Tokenizer *tokenizer = new Tokenizer(std::string("../examples/test.ast"));

    Program *program = new Program();
    program->AddFileBlock(std::string("test.ast"), tokenizer);

    delete tokenizer;
    delete program;

    return 0;
}