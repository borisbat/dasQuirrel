#pragma once

#include "cb_dasQuirrel.h"

__forceinline SQRESULT sq_getstring(HSQUIRRELVM v,SQInteger idx,const char * const *c) {
    return sq_getstring(v,idx,(const SQChar **)c);
}
