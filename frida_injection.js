var module = Module.load("/data/local/tmp/libsuper-hook.so")

new NativeFunction(module.findExportByName("main"), "int", [])()


Interceptor.replace(module.findExportByName("hnit"), new NativeCallback((dst, src, len) => {

    console.log(dst);
    console.log(src);
    console.log(len);
}, "void", ["uint64", "uint64", "uint64"]))
