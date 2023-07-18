#define STRINGIFY(x) #x
#define TOSTRING(x) STRINGIFY(x)
#define panic(message, regs) _panic(message, __FILE__, TOSTRING(__LINE__), regs)