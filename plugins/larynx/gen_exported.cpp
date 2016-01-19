#include "gen_exported.h"

namespace gen_exported {


/*******************************************************************************************************************
Copyright (c) 2012 Cycling '74

Permission is hereby granted, free of charge, to any person obtaining a copy of this software
and associated documentation files (the "Software"), to deal in the Software without restriction,
including without limitation the rights to use, copy, modify, merge, publish, distribute, sublicense,
and/or sell copies of the Software, and to permit persons to whom the Software is furnished to do so,
subject to the following conditions:

The above copyright notice and this permission notice shall be included in all copies
or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED,
INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.
IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY,
WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE
OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
*******************************************************************************************************************/


// global noise generator
Noise noise;
static const int GENLIB_LOOPCOUNT_BAIL = 100000;


// The State struct contains all the state and procedures for the gendsp kernel
typedef struct State { 
	CommonState __commonstate;
	Delay m_delay_3;
	SineCycle m_cycle_7;
	SineData __sinedata;
	double m_depth_5;
	double m_rate_6;
	double samplerate;
	double m_tone_4;
	double m_y_2;
	double m_smth_1;
	int vectorsize;
	int __exception;
	// re-initialize all member variables;
	inline void reset(double __sr, int __vs) { 
		__exception = 0;
		vectorsize = __vs;
		samplerate = __sr;
		m_smth_1 = 0;
		m_y_2 = 0;
		m_delay_3.reset("m_delay_3", 44100);
		m_tone_4 = 6000;
		m_depth_5 = 1;
		m_rate_6 = 5;
		m_cycle_7.reset(samplerate, 0);
		genlib_reset_complete(this);
		
	};
	// the signal processing routine;
	inline int perform(t_sample ** __ins, t_sample ** __outs, int __n) { 
		vectorsize = __n;
		const t_sample * __in1 = __ins[0];
		t_sample * __out1 = __outs[0];
		if (__exception) { 
			return __exception;
			
		} else if (( (__in1 == 0) || (__out1 == 0) )) { 
			__exception = GENLIB_ERR_NULL_BUFFER;
			return __exception;
			
		};
		double expr_226 = safediv(((m_tone_4 * 2) * 3.1415926535898), 48000);
		double sin_217 = sin(expr_226);
		double clamp_218 = ((sin_217 <= 1e-05) ? 1e-05 : ((sin_217 >= 0.99999) ? 0.99999 : sin_217));
		// the main sample loop;
		while ((__n--)) { 
			const double in1 = (*(__in1++));
			double mix_229 = (m_y_2 + (clamp_218 * (in1 - m_y_2)));
			double mix_215 = mix_229;
			m_cycle_7.freq(m_rate_6);
			double cycle_221 = m_cycle_7(__sinedata);
			double cycleindex_222 = m_cycle_7.phase();
			double add_220 = (cycle_221 + 1);
			double mul_219 = (add_220 * 0.5);
			double mul_223 = (m_depth_5 * mul_219);
			double mstosamps_214 = (mul_223 * (samplerate * 0.001));
			double mix_230 = (mstosamps_214 + (0.999 * (m_smth_1 - mstosamps_214)));
			double mix_213 = mix_230;
			double tap_225 = m_delay_3.read_linear(mix_213);
			double out1 = tap_225;
			double y0_next_227 = mix_215;
			double smth_next_228 = mix_213;
			m_delay_3.write(mix_215);
			m_y_2 = y0_next_227;
			m_smth_1 = smth_next_228;
			m_delay_3.step();
			// assign results to output buffer;
			(*(__out1++)) = out1;
			
		};
		return __exception;
		
	};
	inline void set_tone(double _value) {
		m_tone_4 = (_value < 500 ? 500 : (_value > 12000 ? 12000 : _value));
	};
	inline void set_depth(double _value) {
		m_depth_5 = (_value < 0.1 ? 0.1 : (_value > 5 ? 5 : _value));
	};
	inline void set_rate(double _value) {
		m_rate_6 = (_value < 0.1 ? 0.1 : (_value > 10 ? 10 : _value));
	};
	
} State;


/// 
///	Configuration for the genlib API
///

/// Number of signal inputs and outputs 

int gen_kernel_numins = 1;
int gen_kernel_numouts = 1;

int num_inputs() { return gen_kernel_numins; }
int num_outputs() { return gen_kernel_numouts; }
int num_params() { return 3; }

/// Assistive lables for the signal inputs and outputs 

const char * gen_kernel_innames[] = { "in1" };
const char * gen_kernel_outnames[] = { "out1" };

/// Invoke the signal process of a State object

int perform(CommonState *cself, t_sample **ins, long numins, t_sample **outs, long numouts, long n) { 
	State * self = (State *)cself;
	return self->perform(ins, outs, n);
}

/// Reset all parameters and stateful operators of a State object

void reset(CommonState *cself) { 
	State * self = (State *)cself;
	self->reset(cself->sr, cself->vs); 
}

/// Set a parameter of a State object 

void setparameter(CommonState *cself, long index, double value, void *ref) {
	State * self = (State *)cself;
	switch (index) {
		case 0: self->set_tone(value); break;
		case 1: self->set_depth(value); break;
		case 2: self->set_rate(value); break;
		
		default: break;
	}
}

/// Get the value of a parameter of a State object 

void getparameter(CommonState *cself, long index, double *value) {
	State *self = (State *)cself;
	switch (index) {
		case 0: *value = self->m_tone_4; break;
		case 1: *value = self->m_depth_5; break;
		case 2: *value = self->m_rate_6; break;
		
		default: break;
	}
}

/// Allocate and configure a new State object and it's internal CommonState:

void * create(double sr, long vs) {
	State *self = new State;
	self->reset(sr, vs);
	ParamInfo *pi;
	self->__commonstate.inputnames = gen_kernel_innames;
	self->__commonstate.outputnames = gen_kernel_outnames;
	self->__commonstate.numins = gen_kernel_numins;
	self->__commonstate.numouts = gen_kernel_numouts;
	self->__commonstate.sr = sr;
	self->__commonstate.vs = vs;
	self->__commonstate.params = (ParamInfo *)genlib_sysmem_newptr(3 * sizeof(ParamInfo));
	self->__commonstate.numparams = 3;
	// initialize parameter 0 ("m_tone_4")
	pi = self->__commonstate.params + 0;
	pi->name = "tone";
	pi->paramtype = GENLIB_PARAMTYPE_FLOAT;
	pi->defaultvalue = self->m_tone_4;
	pi->defaultref = 0;
	pi->hasinputminmax = false;
	pi->inputmin = 0; 
	pi->inputmax = 1;
	pi->hasminmax = true;
	pi->outputmin = 500;
	pi->outputmax = 12000;
	pi->exp = 0;
	pi->units = "";		// no units defined
	// initialize parameter 1 ("m_depth_5")
	pi = self->__commonstate.params + 1;
	pi->name = "depth";
	pi->paramtype = GENLIB_PARAMTYPE_FLOAT;
	pi->defaultvalue = self->m_depth_5;
	pi->defaultref = 0;
	pi->hasinputminmax = false;
	pi->inputmin = 0; 
	pi->inputmax = 1;
	pi->hasminmax = true;
	pi->outputmin = 0.1;
	pi->outputmax = 5;
	pi->exp = 0;
	pi->units = "";		// no units defined
	// initialize parameter 2 ("m_rate_6")
	pi = self->__commonstate.params + 2;
	pi->name = "rate";
	pi->paramtype = GENLIB_PARAMTYPE_FLOAT;
	pi->defaultvalue = self->m_rate_6;
	pi->defaultref = 0;
	pi->hasinputminmax = false;
	pi->inputmin = 0; 
	pi->inputmax = 1;
	pi->hasminmax = true;
	pi->outputmin = 0.1;
	pi->outputmax = 10;
	pi->exp = 0;
	pi->units = "";		// no units defined
	
	return self;
}

/// Release all resources and memory used by a State object:

void destroy(CommonState *cself) { 
	State * self = (State *)cself;
	genlib_sysmem_freeptr(cself->params);
		
	delete self; 
}


} // gen_exported::
