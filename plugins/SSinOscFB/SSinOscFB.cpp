// PluginSSinOscFB.cpp
// LFSaw (lfsaw@lfsaw.de)

#include "SC_PlugIn.hpp"
#include "SSinOscFB.hpp"

static InterfaceTable* ft;

namespace Steroids {

SSinOscFB::SSinOscFB() {
    mCalcFunc = make_calc_function<SSinOscFB, &SSinOscFB::next>();
    next(1);
}

void SSinOscFB::next(int nSamples) {
    const float* input = in(0);
    const float* gain = in(0);
    float* outbuf = out(0);

    // simple gain function
    for (int i = 0; i < nSamples; ++i) {
        outbuf[i] = input[i] * gain[i];
    }
}

} // namespace Steroids

PluginLoad(SSinOscFBUGens) {
    // Plugin magic
    ft = inTable;
    registerUnit<Steroids::SSinOscFB>(ft, "SSinOscFB");
}
