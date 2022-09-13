#include "daScript/misc/platform.h"

#include "daScript/ast/ast.h"
#include "daScript/ast/ast_interop.h"
#include "daScript/ast/ast_typefactory_bind.h"
#include "daScript/ast/ast_handle.h"

#include "dasQUIRREL.h"

#include "cb_dasQuirrel.h"

#include "dasQUIRREL.struct.decl.inc"

namespace das {

static SQInteger squirrel_print_args(HSQUIRRELVM v) {
    SQInteger nargs = sq_gettop(v); //number of arguments
    for(SQInteger n=1;n<=nargs;n++) {
        printf("arg %d is ",int(n));
        switch(sq_gettype(v,n)) {
            case OT_NULL:           printf("null");break;
            case OT_INTEGER:        printf("integer");break;
            case OT_FLOAT:          printf("float");break;
            case OT_STRING:         printf("string");break;
            case OT_TABLE:          printf("table");break;
            case OT_ARRAY:          printf("array");break;
            case OT_USERDATA:       printf("userdata");break;
            case OT_CLOSURE:        printf("closure(function)");break;
            case OT_NATIVECLOSURE:  printf("native closure(C function)");break;
            case OT_GENERATOR:      printf("generator");break;
            case OT_USERPOINTER:    printf("userpointer");break;
            case OT_CLASS:          printf("class");break;
            case OT_INSTANCE:       printf("instance");break;
            case OT_WEAKREF:        printf("weak reference");break;
            default:                return sq_throwerror(v,"invalid param"); //throws an exception
        }
    }
    printf("\n");
    sq_pushinteger(v,nargs); //push the number of arguments as return value
    return 1; //1 because 1 value is returned
}

static SQInteger squirrel_register_global_func(HSQUIRRELVM v,SQFUNCTION f,const char *fname) {
    sq_pushroottable(v);
    sq_pushstring(v,fname,-1);
    sq_newclosure(v,f,0); //create a new function
    sq_newslot(v,-3,SQFalse);
    sq_pop(v,1); //pops the root table
    return 0;
}

static void squirrel_print_function(HSQUIRRELVM, const SQChar *format, ...) {
    va_list args;
    va_start(args, format);
    vfprintf(stdout, format, args);
    va_end(args);
}

static void squirrel_compiler_error_function(HSQUIRRELVM,const SQChar * desc,const SQChar * source,SQInteger line,SQInteger column) {
    printf("error: %s at %s(%d,%d)\n", desc, source, int(line), int(column));
}

void sqdas_register(HSQUIRRELVM v) {
    sq_setprintfunc(v, squirrel_print_function, squirrel_print_function);
    sq_setcompilererrorhandler(v, squirrel_compiler_error_function);
    squirrel_register_global_func(v, squirrel_print_args, "print_args");
}

void Module_dasQUIRREL::initMain() {
    addConstant<uint64_t>(*this,"SQTrue",1ul);
    addConstant<uint64_t>(*this,"SQFalse",0ul);

    addExtern<DAS_BIND_FUN(sqdas_register)>(*this,lib,"sqdas_register",
        SideEffects::modifyExternal, "sqdas_register");
}

ModuleAotType Module_dasQUIRREL::aotRequire ( TextWriter & tw ) const {
    tw << "#include \"../modules/dasQuirrel/src/cb_dasQuirrel.h\"\n";
    return ModuleAotType::cpp;
}

}


