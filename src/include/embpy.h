#include <pybind11/embed.h>
#include <sstream>
#include <iostream>
namespace py = pybind11;

#define OVERLAY "PYTHONOVERLAY"

static py::module mod;
static int done = 0;

void set_overlay()
{
	if (done) return;

	char *overlay = getenv(OVERLAY);
	if (overlay == NULL) return;	

	mod = py::module::import(overlay);
	done = 1;
}

PYBIND11_EMBEDDED_MODULE(tfhe_py, m) {
    /* Classes required to send data to python. */
    py::class_<TLweParams>(m, "TLweParams")
        .def(py::init<int32_t, int32_t, double, double>())
        ;

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
            })
        ;

    py::class_<TGswParams>(m, "TGswParams")
        .def(py::init<int32_t, int32_t, const TLweParams *>())
        .def_readwrite("tlwe_params", &TGswParams::tlwe_params)
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
            })
        ;

    py::class_<LagrangeHalfCPolynomial, std::unique_ptr<LagrangeHalfCPolynomial, py::nodelete>>(m, "LagrangeHalfCPolynomial")
        .def(py::init<>())
        .def("__getitem__",
            [](const LagrangeHalfCPolynomial *a, int i){
                return a + i;
            })
        ;

    py::class_<TLweSampleFFT, std::unique_ptr<TLweSampleFFT, py::nodelete>>(m, "TLweSampleFFT")
        .def(py::init([](const TLweParams *params, LagrangeHalfCPolynomial *a, double cv)
            {
                return new TLweSampleFFT(params, a, cv);
            }))
        .def("__getitem__",
            [](const TLweSampleFFT *a, int i){
                return a + i;
            })
        ;

    py::class_<TGswSampleFFT>(m, "TGswSampleFFT")
        .def(py::init<const TGswParams *, TLweSampleFFT *>())
        .def_readwrite("all_samples", &TGswSampleFFT::all_samples)
        ;

    py::class_<IntPolynomial, std::unique_ptr<IntPolynomial, py::nodelete>>(m, "IntPolynomial")
        .def(py::init([](const int32_t N)
            {
                return new_IntPolynomial(N);
            })
        )
        .def("__getitem__",
            [](const IntPolynomial *a, int i){
                return a + i;
            })
        ;

    /* Functions used to expand tfhe_blindRoate_FFT in python. */
    m.def("tGswFFTExternMulToTLwe", &tGswFFTExternMulToTLwe);

    m.def("IntPolynomial_ifft", &IntPolynomial_ifft);

    m.def("tLweFromFFTConvert", &tLweFromFFTConvert);

    m.def("tLweFFTAddMulRTo", &tLweFFTAddMulRTo);
}
