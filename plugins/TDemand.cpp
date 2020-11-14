/*
    SuperCollider real time audio synthesis system
    Copyright (c) 2002 James McCartney. All rights reserved.
    http://www.audiosynth.com

    This program is free software; you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation; either version 2 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program; if not, write to the Free Software
    Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301  USA
*/

#include "SC_PlugIn.h"
#include <cstdio>
#include <cmath>
#include <limits>

using std::floor;
using std::numeric_limits;

static InterfaceTable* ft;

struct TDemand : public Unit {
    float m_prevtrig;
    float m_prevreset;
    //float* m_prevout;
    float** m_out;
};



void TDemand_next_aa(TDemand* unit, int inNumSamples) {
    float* trig = ZIN(0);
    float* reset = ZIN(1);

    float** out = unit->m_out;
    // float* prevout = unit->m_prevout;

    for (int i = 0; i < unit->mNumOutputs; ++i) {
        out[i] = OUT(i);
    }

    float prevtrig = unit->m_prevtrig;
    float prevreset = unit->m_prevreset;

    // Print("TDemand_next_aa %d  %g\n", inNumSamples, prevtrig);
    for (int i = 0; i < inNumSamples; ++i) {
        float ztrig = ZXP(trig);
        float zreset = ZXP(reset);
        if (zreset > 0.f && prevreset <= 0.f) {
            for (int j = 2; j < unit->mNumInputs; ++j) {
                RESETINPUT(j);
            }
        }
        if (ztrig > 0.f && prevtrig <= 0.f) {
            // Print("triggered\n");
            for (int j = 2, k = 0; j < unit->mNumInputs; ++j, ++k) {
                float x = DEMANDINPUT_A(j, i + 1);
                // printf("in  %d %g\n", k, x);
                if (sc_isnan(x)) {
                    x = 0.f;
                    unit->mDone = true;
                } else
                    // prevout[k] = x;
                out[k][i] = x;
            }
        } else {
            for (int j = 2, k = 0; j < unit->mNumInputs; ++j, ++k) {
                out[k][i] = 0.f;
            }
        }
        prevtrig = ztrig;
        prevreset = zreset;
    }

    unit->m_prevtrig = prevtrig;
    unit->m_prevreset = prevreset;
}


void TDemand_next_ak(TDemand* unit, int inNumSamples) {
    float* trig = ZIN(0);
    float zreset = IN0(1);

    float** out = unit->m_out;
    // float* prevout = unit->m_prevout;

    for (int i = 0; i < unit->mNumOutputs; ++i) {
        out[i] = OUT(i);
    }

    float prevtrig = unit->m_prevtrig;
    float prevreset = unit->m_prevreset;

    for (int i = 0; i < inNumSamples; ++i) {
        float ztrig = ZXP(trig);
        if (zreset > 0.f && prevreset <= 0.f) {
            for (int j = 2; j < unit->mNumInputs; ++j) {
                RESETINPUT(j);
            }
        }

        if (ztrig > 0.f && prevtrig <= 0.f) {
            for (int j = 2, k = 0; j < unit->mNumInputs; ++j, ++k) {
                float x = DEMANDINPUT_A(j, i + 1);
                if (sc_isnan(x)) {
                    x = 0.f;
                    unit->mDone = true;
                } else
                    // prevout[k] = x;
                out[k][i] = x;
            }

        } else {
            for (int j = 2, k = 0; j < unit->mNumInputs; ++j, ++k) {
                out[k][i] = 0.f;
            }
        }
        prevtrig = ztrig;
        prevreset = zreset;
    }

    unit->m_prevtrig = prevtrig;
    unit->m_prevreset = prevreset;
}


void TDemand_next_ka(TDemand* unit, int inNumSamples) {
    float ztrig = IN0(0);
    float* reset = ZIN(1);

    float** out = unit->m_out;
    // float* prevout = unit->m_prevout;

    for (int i = 0; i < unit->mNumOutputs; ++i) {
        out[i] = OUT(i);
    }

    float prevtrig = unit->m_prevtrig;
    float prevreset = unit->m_prevreset;

    for (int i = 0; i < inNumSamples; ++i) {
        float zreset = ZXP(reset);
        if (zreset > 0.f && prevreset <= 0.f) {
            for (int j = 2; j < unit->mNumInputs; ++j) {
                RESETINPUT(j);
            }
        }
        if (ztrig > 0.f && prevtrig <= 0.f) {
            for (int j = 2, k = 0; j < unit->mNumInputs; ++j, ++k) {
                float x = DEMANDINPUT_A(j, i + 1);
                if (sc_isnan(x)) {
                    x = 0.f;
                    unit->mDone = true;
                } else
                    // prevout[k] = x;
                out[k][i] = x;
            }
        }
        prevtrig = ztrig;
        prevreset = zreset;
    }

    unit->m_prevtrig = prevtrig;
    unit->m_prevreset = prevreset;
}


void TDemand_Ctor(TDemand* unit) {
    // Print("TDemand_Ctor\n");
    if (INRATE(0) == calc_FullRate) {
        if (INRATE(1) == calc_FullRate) {
            SETCALC(TDemand_next_aa);
        } else {
            SETCALC(TDemand_next_ak);
        }
    } else {
        if (INRATE(1) == calc_FullRate) {
            SETCALC(TDemand_next_ka);
        } else {
            SETCALC(TDemand_next_aa);
        }
    }

    for (int i = 0; i < unit->mNumOutputs; ++i)
        OUT0(i) = 0.f;

    char* memoryChunk = (char*)RTAlloc(unit->mWorld, unit->mNumOutputs * (sizeof(float) + sizeof(float*)));

    if (!memoryChunk) {
        Print("TDemand: RT memory allocation failed\n");
        SETCALC(ClearUnitOutputs);
        return;
    }

    // unit->m_prevout = (float*)memoryChunk;
    unit->m_out = (float**)(memoryChunk + unit->mNumOutputs * sizeof(float));

    unit->m_prevtrig = 0.f;
    unit->m_prevreset = 0.f;
    // std::fill_n(unit->m_prevout, unit->mNumOutputs, 0.f);
}

// void TDemand_Dtor(TDemand* unit) {
    // if (unit->m_prevout)
    //     RTFree(unit->mWorld, unit->m_prevout);
// }

//////////////////////////////////////////////////////


PluginLoad(TDemand) {
    ft = inTable;

    DefineSimpleCantAliasUnit(TDemand);
}

