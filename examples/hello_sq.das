
require quirrel
require strings

[export]
def main
    print("Hello, World!\n")

    let script = "println(\"Hello,squirrel\")"
    var v = sq_open(1024l)

    sqdas_register(v)

    sq_pushroottable(v)
    if sq_compilebuffer(v,script,int64(length(script)),"script.nut",1ul,null) != 0l
        panic("compile error")

    sq_close(v)
