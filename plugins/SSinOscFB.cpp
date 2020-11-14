// SSinOscFB.cpp
// LFSaw (lfsaw@lfsaw.de)

#include "SC_PlugIn.hpp"
#include "SSinOscFB.hpp"
#include <iostream>

static InterfaceTable* ft;

namespace Steroids {

SSinOscFB::SSinOscFB() {
    int tableSize2 = ft->mSineSize;
    m_lomask = (tableSize2 - 1) << 3;
    m_radtoinc = tableSize2 * (rtwopi * 65536.);
    m_cpstoinc = tableSize2 * sampleDur() * 65536.;
    m_prevout = 0.;
    m_feedback = in0(1) * m_radtoinc;

    m_phase = 0;


    if (isAudioRateIn(0)) { // ax
        if (isAudioRateIn(1)) { // aa
            mCalcFunc = make_calc_function<SSinOscFB, &SSinOscFB::next_aa>();
            next_aa(1);
        } else { // ak
            mCalcFunc = make_calc_function<SSinOscFB, &SSinOscFB::next_ak>();
            next_ak(1);
        }
    } else { // kx
        if (isAudioRateIn(1)) { // ka
            mCalcFunc = make_calc_function<SSinOscFB, &SSinOscFB::next_ka>();
            next_ka(1);
        } else { // ak
            mCalcFunc = make_calc_function<SSinOscFB, &SSinOscFB::next_kk>();
            next_kk(1);
        }
    }


}

void SSinOscFB::next_kk(int nSamples) {
    float* out = zout(0);
    float freqin = in0(0);

    float feedback = m_feedback;
    float nextFeedback = in0(1) * m_radtoinc;

    float* table0 = ft->mSineWavetable;
    float* table1 = table0 + 1;

    int32 phase = m_phase;
    int32 lomask = m_lomask;
    float prevout = m_prevout;
    float feedback_slope = calcSlope(nextFeedback, feedback);
    int32 freq = (int32)(m_cpstoinc * freqin);

    LooP(nSamples) {
        prevout = lookupi1(table0, table1, phase + (int32)(feedback * prevout), lomask);
        ZXP(out) = prevout;
        phase += freq;
        feedback += feedback_slope;
    }
    m_phase = phase;
    m_prevout = prevout;
    m_feedback = feedback;
}

void SSinOscFB::next_ak(int nSamples) {
    float* fp_out = out(0);
    const float* fp_freqin = in(0);

    float feedback = m_feedback;
    float radtoinc = m_radtoinc;
    float cpstoinc = m_cpstoinc;
    float nextFeedback = in0(1) * radtoinc;

    float* table0 = ft->mSineWavetable;
    float* table1 = table0 + 1;

    int32 phase = m_phase;
    int32 lomask = m_lomask;
    float prevout = m_prevout;

    float feedback_slope = calcSlope(nextFeedback, feedback);

    for (int i = 0; i < nSamples; ++i) {
        prevout = lookupi1(table0, table1, phase + (int32)(feedback * prevout), lomask);
        phase += (int32)(cpstoinc * fp_freqin[i]);
        feedback += feedback_slope;
        fp_out[i] = prevout;
    }
    m_phase = phase;
    m_prevout = prevout;
    m_feedback = feedback;
}

void SSinOscFB::next_aa(int nSamples) {
    float* fp_out = out(0);
    const float* fp_freqin = in(0);
    const float* fp_feedback = in(1);

    // float feedback = m_feedback;
    float radtoinc = m_radtoinc;
    float cpstoinc = m_cpstoinc;

    float* table0 = ft->mSineWavetable;
    float* table1 = table0 + 1;

    int32 phase = m_phase;
    int32 lomask = m_lomask;
    float prevout = m_prevout;

    for (int i = 0; i < nSamples; ++i) {
        float feedback = fp_feedback[i] * radtoinc;
        prevout = lookupi1(table0, table1, phase + (int32)(feedback * prevout), lomask);
        phase += (int32)(cpstoinc * fp_freqin[i]);
        fp_out[i] = prevout;
    }
    m_phase = phase;
    m_prevout = prevout;
    // m_feedback = feedback;
}

///////////////////////////

void SSinOscFB::next_ka(int nSamples) {
    float* fp_out = out(0);
    float f_freqin = in0(0);
    const float* fp_feedback = in(1);

    // float feedback = m_feedback;
    float radtoinc = m_radtoinc;
    float cpstoinc = m_cpstoinc;

    float* table0 = ft->mSineWavetable;
    float* table1 = table0 + 1;

    int32 phase = m_phase;
    int32 lomask = m_lomask;
    float prevout = m_prevout;

    int32 freq = (int32)(m_cpstoinc * f_freqin);

    for (int i = 0; i < nSamples; ++i) {
        float feedback = fp_feedback[i] * radtoinc;
        prevout = lookupi1(table0, table1, phase + (int32)(feedback * prevout), lomask);
        phase += freq;
        fp_out[i] = prevout;
    }
    m_phase = phase;
    m_prevout = prevout;
    // m_feedback = feedback;
}


} // namespace Steroids

PluginLoad(SteroidsUGens) {
    // Plugin magic
    ft = inTable;
    registerUnit<Steroids::SSinOscFB>(ft, "SSinOscFB");
}
