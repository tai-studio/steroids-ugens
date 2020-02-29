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
    void next(int nSamples);

    // Member variables
};

} // namespace Steroids
