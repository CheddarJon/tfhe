#include <pybind11/embed.h>
#include <sstream>
namespace py = pybind11;

#define OVERLAY "PYTHON_OVERLAY"
#define OVERLAY_FUNC "execute"
#define OVERLAY_FUNC_ARGC 2

PYBIND11_EMBEDDED_MODULE(tfhe_py, m) {
    /* Classes required to send data to python. */
    py::class_<TLweParams>(m, "TLweParams")
        .def(py::init<int32_t, int32_t, double, double>());

    py::class_<TLweSample>(m, "TLweSample")
        .def(py::init<const TLweParams *>())
        .def("__repr__",
            [](const TLweSample &o) {
                std::ostringstream os;
                os << "<TLweSample: 2 TorusPolynomial arrays size "
                    << std::to_string(o.k);
                if (o.k > 0) {
                    os << "<TorusPolynomial> 1 array size "
                        << std::to_string(std::max(o.a[0].N, o.b[0].N));
                }
                os << ">";
                return os.str();
            }
        );

    py::class_<TGswParams>(m, "TGswParams")
        .def(py::init<int32_t, int32_t, const TLweParams *>())
        .def("__repr__",
            [](const TGswParams &o) {
                std::ostringstream os;
                os << "<TGswParams: 'l = " << std::to_string(o.l) << "' ";
                os << "'#of rows ((k+1)*l) = " << std::to_string(o.kpl) << "' ";
                if (o.kpl > 0) {
                    os << "'k = " << std::to_string(o.tlwe_params[0].k) << "' ";
                    os << "'N = " << std::to_string(o.tlwe_params[0].N) << "' ";
                } else {
                    os << "'k = " <<  "?', ";
                    os << "'N = " << "?'";
                }
                os << ">";
                return os.str();
            }
        );

    py::class_<LagrangeHalfCPolynomial>(m, "LagrangeHalfCPolynomial")
        .def(py::init<>());

    py::class_<TLweSampleFFT>(m, "TLweSampleFFT")
        .def(py::init<const TLweParams *, LagrangeHalfCPolynomial *, double>());

    py::class_<TGswSampleFFT>(m, "TGswSampleFFT")
        .def(py::init<const TGswParams *, TLweSampleFFT *>());

    /* Functions used to expand tfhe_blindRoate_FFT in python. */
    m.def("tLweMulByXaiMinusOne",
            &tLweMulByXaiMinusOne,
            py::return_value_policy::reference
    );

    m.def("tGswFFTExternMulToLwe",
            &tGswFFTExternMulToTLwe,
            py::return_value_policy::reference
    );

    m.def("tLweAddTo",
            &tLweAddTo,
            py::return_value_policy::reference
    );

    m.def("swap",
        [](TLweSample *a, TLweSample *b) {std::swap(a, b);}
    );
}

#define RUNPY(overlay, func, a, b) ({\
        char *mod = getenv(overlay);\
        if (mod != NULL) {\
        try {\
            py::scoped_interpreter guard{};\
            py::module o = py::module::import(mod);\
            py::object r = o.attr(func)(a, b);}\
        catch(...){}}})


