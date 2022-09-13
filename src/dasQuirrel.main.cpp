#include "daScript/misc/platform.h"

#include "daScript/ast/ast.h"
#include "daScript/ast/ast_interop.h"
#include "daScript/ast/ast_typefactory_bind.h"

#include "dasQUIRREL.h"

#include "cb_dasQuirrel.h"

#include "dasQUIRREL.struct.decl.inc"

namespace das {

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
}

void Module_dasQUIRREL::initMain() {
    addConstant<int64_t>(*this,"SQTrue",int64_t(1));
    addConstant<int64_t>(*this,"SQFalse",int64_t(0));

    addExtern<DAS_BIND_FUN(sqdas_register)>(*this,lib,"sqdas_register",
        SideEffects::modifyExternal, "sqdas_register");
}

ModuleAotType Module_dasQUIRREL::aotRequire ( TextWriter & tw ) const {
    tw << "#include \"../modules/dasQuirrel/src/cb_dasQuirrel.h\"\n";
    return ModuleAotType::cpp;
}

}


