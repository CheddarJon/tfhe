#include <pybind11/embed.h>
namespace py = pybind11;

#define OVERLAY "PYTHON_OVERLAY"
#define OVERLAY_FUNC "execute"
#define OVERLAY_FUNC_ARGC 2

#define RUNPY(overlay, func, a, b) ({\
        char *mod = getenv(overlay);\
        if (mod != NULL) {\
        try {\
            py::scoped_interpreter guard{};\
            py::module o = py::module::import(mod);\
            py::object r = o.attr(func)(a, b);}\
        catch(...){}}})
