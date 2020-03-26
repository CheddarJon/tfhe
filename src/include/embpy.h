#include <pybind11/embed.h>
namespace py = pybind11;

#define OVERLAY "PYTHON_OVERLAY"
#define OVERLAY_FUNC "execute"
#define OVERLAY_FUNC_ARGC 2

PYBIND11_EMBEDDED_MODULE(tfhe_py, m) {
    py::class_<TLweParams>(m, "TLweParams")
        .def(py::init<int32_t, int32_t, double, double>());

    py::class_<TLweSample>(m, "TLweSample")
        .def(py::init<const TLweParams *>());

    py::class_<TGswParams>(m, "TGswParams")
        .def(py::init<int32_t, int32_t, const TLweParams *>());

    py::class_<LagrangeHalfCPolynomial>(m, "LagrangeHalfCPolynomial")
        .def(py::init<>());

    py::class_<TLweSampleFFT>(m, "TLweSampleFFT")
        .def(py::init<const TLweParams *, LagrangeHalfCPolynomial *, double>());

    py::class_<TGswSampleFFT>(m, "TGswSampleFFT")
        .def(py::init<const TGswParams *, TLweSampleFFT *>());
}

#define RUNPY(overlay, func, a, b) ({\
        char *mod = getenv(overlay);\
        if (mod != NULL) {\
        try {\
            py::scoped_interpreter guard{};\
            py::module o = py::module::import(mod);\
            py::object r = o.attr(func)(a, b);}\
        catch(...){}}})


