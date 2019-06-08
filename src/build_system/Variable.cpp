//
// Created by daniel on 6/8/19.
//

#include "Variable.h"

namespace astro {
    Variable::Variable(uint64_t id)
        : m_type(PT_INT32), m_name(""), m_id(id) {}
}