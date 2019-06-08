//
// Created by daniel on 6/8/19.
//

#ifndef ASTRO_PRIMITIVETYPES_H
#define ASTRO_PRIMITIVETYPES_H

#include <string>

namespace astro {
    enum PrimType {
        PT_INT8,
        PT_INT16,
        PT_INT32,
        PT_INT64,
        PT_UINT8,
        PT_UINT16,
        PT_UINT32,
        PT_UINT64,
        PT_BOOLEAN,
        PT_UNKNOWN
    };

    static PrimType StringToPrimType(const std::string& type) {
        if(type == "int8") {
            return PT_INT8;
        } else if(type == "int16") {
            return PT_INT16;
        } else if(type == "int32") {
            return PT_INT32;
        } else if(type == "int64") {
            return PT_INT64;
        } else if(type == "uint8") {
            return PT_UINT8;
        } else if(type == "uint16") {
            return PT_UINT16;
        } else if(type == "uint32") {
            return PT_UINT32;
        } else if(type == "uint64") {
            return PT_UINT64;
        } else if(type == "boolean") {
            return PT_BOOLEAN;
        }

        return PT_UNKNOWN;
    }
}
#endif //ASTRO_PRIMITIVETYPES_H
