
require quirrel
require strings

[export]
def main
    let script = "println(\"Hello,squirrel\")"
    var v = sq_open(1024l)
    sqstd_seterrorhandlers(v)
    sqdas_register(v)
    if sq_compilebuffer(v,script,int64(length(script)),"script.nut",1ul,null) != 0l
        panic("compile error")
    sq_pushroottable(v)
    if sq_call(v,1l,SQFalse,SQTrue) != 0l
        panic("call error")
    sq_close(v)
