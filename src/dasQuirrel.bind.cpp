#include "daScript/misc/platform.h"

#include "daScript/ast/ast.h"
#include "daScript/ast/ast_interop.h"
#include "daScript/ast/ast_typefactory_bind.h"
#include "daScript/src/builtin/module_builtin_rtti.h"
#include "daScript/ast/ast_handle.h"

#include "dasQUIRREL.h"

#include "cb_dasQuirrel.h"

namespace das {

struct SqFuncDesc {
    string name;
    string paramsCheck;
    int paramsNum;
};

static das_hash_map<uint64_t, Context*> bindedFunctions;
static das_hash_map<uint64_t, SqFuncDesc> bindedFunctionNames;

void sqdas_bind_func( Context &ctx, uint64_t fnHash, const char * name, int paramsNum, const char * paramsCheck) {
    bindedFunctions[fnHash] = &ctx;
    bindedFunctionNames[fnHash] = { name, paramsCheck, paramsNum};
}

SQInteger call_binded_func(HSQUIRRELVM vm) {
    SQInteger funcHash;
    sq_getinteger(vm, -1, &funcHash);
    auto fnCtx = bindedFunctions.find(funcHash);
    if (fnCtx == bindedFunctions.end()) {
        return sq_throwerror(vm, string(string::CtorSprintf{}, "Internal error: unknown function with hash '%@'. Maybe function was removed", funcHash).c_str());
    }
    Context *ctx = fnCtx->second;
    if (!ctx) {
        return sq_throwerror(vm, string(string::CtorSprintf{},
                    "Unable to call function '%s', context is null. Unloaded function or compilation error",
                    bindedFunctionNames[funcHash].name.c_str()).c_str());
    }

    SimFunction *simFn = ctx->fnByMangledName(funcHash);
    if (!simFn) {
        return sq_throwerror(vm, string(string::CtorSprintf{},
                    "Internal error: unable to find function '%s'. Maybe compilation error",
                    bindedFunctionNames[funcHash].name.c_str()).c_str());
    }

    vec4f args[1];
    args[0] = cast<HSQUIRRELVM>::from(vm);
    vec4f res{};
    ctx->tryRestartAndLock();
    if ( !ctx->ownStack ) {
        StackAllocator sharedStack(8*1024);
        SharedStackGuard guard(*ctx, sharedStack);
        res = ctx->evalWithCatch(simFn, args);
    } else {
        res = ctx->evalWithCatch(simFn, args);
    }
    if (auto exp = ctx->getException()) {
        ctx->stackWalk(&ctx->exceptionAt, true, true);
        ctx->unlock();
        return sq_throwerror(vm, exp);
    }
    ctx->unlock();
    return cast<SQInteger>::to(res);
}

void register_bound_funcs(HSQUIRRELVM vm, HSQOBJECT tab) {
    assert(sq_istable(tab));
    sq_pushobject(vm, tab);
    for (auto &pair : bindedFunctionNames) {
        sq_pushstring(vm, pair.second.name.c_str(), -1);
        sq_pushinteger(vm, pair.first); // push func hash
        sq_newclosure(vm, call_binded_func, 1);
        sq_setparamscheck(vm, pair.second.paramsNum, pair.second.paramsCheck.c_str());
        sq_setnativeclosurename(vm, -1, pair.second.name.c_str());
        sq_newslot(vm, -3, SQFalse);
    }
    sq_pop(vm, 1);
}

void Module_dasQUIRREL::initBind() {

    addExtern<DAS_BIND_FUN(sqdas_bind_func)>(*this,lib,"sqdas_bind_func",
        SideEffects::modifyExternal, "sqdas_bind_func");

    das::onDestroyCppDebugAgent(name.c_str(), [](das::Context *ctx) {
        for (auto &it : bindedFunctions) {
          if (it.second == ctx) {
            it.second = nullptr;
            LOG tout(LogLevel::info);
            tout << "unlink quirrel binding: " << bindedFunctionNames[it.first].name << "\n";
          }
        }
    });
}

}

