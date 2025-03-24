
require quirrel
require strings

def call_foo(v : HSQUIRRELVM; n : int; f : float; s : string)
    var top = sq_gettop(v)
    sq_pushroottable(v)
    sq_pushstring(v, "foo", -1l)
    if sq_get(v, -2l) != -1l
        sq_pushroottable(v)
        sq_pushinteger(v, int64(n))
        sq_pushfloat(v, f)
        sq_pushstring(v, s, -1l)
        sq_call(v, 4l, SQFalse, SQTrue)
    else
        panic("can't find foo\n")
    sq_settop(v, top)

[export]
def main
    var v = sq_open(1024l)
    sqstd_seterrorhandlers(v)
    sqdas_register(v)
    sq_pushroottable(v)
    if sqstd_dofile(v, "test.nut", SQFalse, SQTrue) == -1l
        panic("can't compile\n")
    call_foo(v, 1, 2.5, "teststring")
    sq_pop(v, 1l)
    sq_close(v)
