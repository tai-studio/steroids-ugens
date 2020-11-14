// PluginSSinOscFB.hpp
// LFSaw (lfsaw@lfsaw.de)

#pragma once

#include "SC_PlugIn.hpp"

namespace Steroids {

class SSinOscFB : public SCUnit {
public:
    SSinOscFB();

    // Destructor
    // ~SSinOscFB();

private:
    // Calc function
    void next_aa(int nSamples);
    void next_ak(int nSamples);
    void next_ka(int nSamples);
    void next_kk(int nSamples);

    // Member variables
	// from BufUnit
    SndBuf* m_buf;
    float m_fbufnum;

	// from TableLookup
	double m_cpstoinc, m_radtoinc;
    int32 mTableSize;
    int32 m_lomask;

    // SSinOscFB specific
    int32 m_phase;
    float m_prevout, m_feedback;

};

} // namespace Steroids
