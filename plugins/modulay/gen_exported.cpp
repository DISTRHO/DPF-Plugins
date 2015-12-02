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
	Delay m_delay_2;
	Delay m_delay_3;
	SineCycle m_cycle_14;
	SineData __sinedata;
	double m_bleed_9;
	double m_rate_11;
	double m_time_12;
	double m_cutoff_10;
	double m_feedback_13;
	double m_repeats_8;
	double m_depth_6;
	double m_mix_5;
	double m_feedforward_7;
	double m_y_1;
	double samplerate;
	double m_delay_4;
	int vectorsize;
	int __exception;
	// re-initialize all member variables;
	inline void reset(double __sr, int __vs) { 
		__exception = 0;
		vectorsize = __vs;
		samplerate = __sr;
		m_y_1 = 0;
		m_delay_2.reset("m_delay_2", 44100);
		m_delay_3.reset("m_delay_3", 44100);
		m_delay_4 = 0;
		m_mix_5 = 1;
		m_depth_6 = 0;
		m_feedforward_7 = 1;
		m_repeats_8 = 0.75;
		m_bleed_9 = 0;
		m_cutoff_10 = 4000;
		m_rate_11 = 2;
		m_time_12 = 500;
		m_feedback_13 = 0;
		m_cycle_14.reset(samplerate, 0);
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
		double mstosamps_4908 = (m_time_12 * (samplerate * 0.001));
		double expr_4909 = safediv(((m_cutoff_10 * 2) * 3.1415926535898), 44100);
		double sin_4889 = sin(expr_4909);
		double clamp_4890 = ((sin_4889 <= 1e-05) ? 1e-05 : ((sin_4889 >= 0.99999) ? 0.99999 : sin_4889));
		// the main sample loop;
		while ((__n--)) { 
			const double in1 = (*(__in1++));
			double tap_4907 = m_delay_3.read_linear(mstosamps_4908);
			double mix_4936 = (m_y_1 + (clamp_4890 * (tap_4907 - m_y_1)));
			double mix_4887 = mix_4936;
			double mul_4904 = (mix_4887 * m_repeats_8);
			m_cycle_14.freq(m_rate_11);
			double cycle_4893 = m_cycle_14(__sinedata);
			double cycleindex_4894 = m_cycle_14.phase();
			double add_4892 = (cycle_4893 + 1);
			double mul_4891 = (add_4892 * 0.5);
			double mul_4896 = (m_depth_6 * mul_4891);
			double add_4895 = (m_delay_4 + mul_4896);
			double mstosamps_4886 = (add_4895 * (samplerate * 0.001));
			double tap_4901 = m_delay_2.read_linear(mstosamps_4886);
			double mul_4899 = (tap_4901 * m_feedforward_7);
			double mul_4897 = (tap_4901 * m_feedback_13);
			double add_4903 = (mix_4887 + mul_4897);
			double mul_4898 = (add_4903 * m_bleed_9);
			double add_4902 = (mul_4898 + mul_4899);
			double mul_4905 = (add_4902 * m_mix_5);
			double out1 = (mul_4905 + in1);
			double y0_next_4910 = mix_4887;
			m_delay_3.write((mul_4904 + in1));
			m_delay_2.write(add_4903);
			m_y_1 = y0_next_4910;
			m_delay_2.step();
			m_delay_3.step();
			// assign results to output buffer;
			(*(__out1++)) = out1;
			
		};
		return __exception;
		
	};
	inline void set_delay(double _value) {
		m_delay_4 = (_value < 0 ? 0 : (_value > 30 ? 30 : _value));
	};
	inline void set_mix(double _value) {
		m_mix_5 = (_value < 0 ? 0 : (_value > 1 ? 1 : _value));
	};
	inline void set_depth(double _value) {
		m_depth_6 = (_value < 0 ? 0 : (_value > 5 ? 5 : _value));
	};
	inline void set_feedforward(double _value) {
		m_feedforward_7 = (_value < 0.7 ? 0.7 : (_value > 1 ? 1 : _value));
	};
	inline void set_repeats(double _value) {
		m_repeats_8 = (_value < 0 ? 0 : (_value > 0.99 ? 0.99 : _value));
	};
	inline void set_bleed(double _value) {
		m_bleed_9 = (_value < 0 ? 0 : (_value > 0.7 ? 0.7 : _value));
	};
	inline void set_cutoff(double _value) {
		m_cutoff_10 = (_value < 0 ? 0 : (_value > 6000 ? 6000 : _value));
	};
	inline void set_rate(double _value) {
		m_rate_11 = (_value < 0.1 ? 0.1 : (_value > 10 ? 10 : _value));
	};
	inline void set_time(double _value) {
		m_time_12 = (_value < 10 ? 10 : (_value > 1000 ? 1000 : _value));
	};
	inline void set_feedback(double _value) {
		m_feedback_13 = (_value < -0.7 ? -0.7 : (_value > 0.7 ? 0.7 : _value));
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
int num_params() { return 10; }

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
		case 0: self->set_delay(value); break;
		case 1: self->set_mix(value); break;
		case 2: self->set_depth(value); break;
		case 3: self->set_feedforward(value); break;
		case 4: self->set_repeats(value); break;
		case 5: self->set_bleed(value); break;
		case 6: self->set_cutoff(value); break;
		case 7: self->set_rate(value); break;
		case 8: self->set_time(value); break;
		case 9: self->set_feedback(value); break;
		
		default: break;
	}
}

/// Get the value of a parameter of a State object 

void getparameter(CommonState *cself, long index, double *value) {
	State *self = (State *)cself;
	switch (index) {
		case 0: *value = self->m_delay_4; break;
		case 1: *value = self->m_mix_5; break;
		case 2: *value = self->m_depth_6; break;
		case 3: *value = self->m_feedforward_7; break;
		case 4: *value = self->m_repeats_8; break;
		case 5: *value = self->m_bleed_9; break;
		case 6: *value = self->m_cutoff_10; break;
		case 7: *value = self->m_rate_11; break;
		case 8: *value = self->m_time_12; break;
		case 9: *value = self->m_feedback_13; break;
		
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
	self->__commonstate.params = (ParamInfo *)genlib_sysmem_newptr(10 * sizeof(ParamInfo));
	self->__commonstate.numparams = 10;
	// initialize parameter 0 ("m_delay_4")
	pi = self->__commonstate.params + 0;
	pi->name = "delay";
	pi->paramtype = GENLIB_PARAMTYPE_FLOAT;
	pi->defaultvalue = self->m_delay_4;
	pi->defaultref = 0;
	pi->hasinputminmax = false;
	pi->inputmin = 0; 
	pi->inputmax = 1;
	pi->hasminmax = true;
	pi->outputmin = 0;
	pi->outputmax = 30;
	pi->exp = 0;
	pi->units = "";		// no units defined
	// initialize parameter 1 ("m_mix_5")
	pi = self->__commonstate.params + 1;
	pi->name = "mix";
	pi->paramtype = GENLIB_PARAMTYPE_FLOAT;
	pi->defaultvalue = self->m_mix_5;
	pi->defaultref = 0;
	pi->hasinputminmax = false;
	pi->inputmin = 0; 
	pi->inputmax = 1;
	pi->hasminmax = true;
	pi->outputmin = 0;
	pi->outputmax = 1;
	pi->exp = 0;
	pi->units = "";		// no units defined
	// initialize parameter 2 ("m_depth_6")
	pi = self->__commonstate.params + 2;
	pi->name = "depth";
	pi->paramtype = GENLIB_PARAMTYPE_FLOAT;
	pi->defaultvalue = self->m_depth_6;
	pi->defaultref = 0;
	pi->hasinputminmax = false;
	pi->inputmin = 0; 
	pi->inputmax = 1;
	pi->hasminmax = true;
	pi->outputmin = 0;
	pi->outputmax = 5;
	pi->exp = 0;
	pi->units = "";		// no units defined
	// initialize parameter 3 ("m_feedforward_7")
	pi = self->__commonstate.params + 3;
	pi->name = "feedforward";
	pi->paramtype = GENLIB_PARAMTYPE_FLOAT;
	pi->defaultvalue = self->m_feedforward_7;
	pi->defaultref = 0;
	pi->hasinputminmax = false;
	pi->inputmin = 0; 
	pi->inputmax = 1;
	pi->hasminmax = true;
	pi->outputmin = 0.7;
	pi->outputmax = 1;
	pi->exp = 0;
	pi->units = "";		// no units defined
	// initialize parameter 4 ("m_repeats_8")
	pi = self->__commonstate.params + 4;
	pi->name = "repeats";
	pi->paramtype = GENLIB_PARAMTYPE_FLOAT;
	pi->defaultvalue = self->m_repeats_8;
	pi->defaultref = 0;
	pi->hasinputminmax = false;
	pi->inputmin = 0; 
	pi->inputmax = 1;
	pi->hasminmax = true;
	pi->outputmin = 0;
	pi->outputmax = 0.99;
	pi->exp = 0;
	pi->units = "";		// no units defined
	// initialize parameter 5 ("m_bleed_9")
	pi = self->__commonstate.params + 5;
	pi->name = "bleed";
	pi->paramtype = GENLIB_PARAMTYPE_FLOAT;
	pi->defaultvalue = self->m_bleed_9;
	pi->defaultref = 0;
	pi->hasinputminmax = false;
	pi->inputmin = 0; 
	pi->inputmax = 1;
	pi->hasminmax = true;
	pi->outputmin = 0;
	pi->outputmax = 0.7;
	pi->exp = 0;
	pi->units = "";		// no units defined
	// initialize parameter 6 ("m_cutoff_10")
	pi = self->__commonstate.params + 6;
	pi->name = "cutoff";
	pi->paramtype = GENLIB_PARAMTYPE_FLOAT;
	pi->defaultvalue = self->m_cutoff_10;
	pi->defaultref = 0;
	pi->hasinputminmax = false;
	pi->inputmin = 0; 
	pi->inputmax = 1;
	pi->hasminmax = true;
	pi->outputmin = 0;
	pi->outputmax = 6000;
	pi->exp = 0;
	pi->units = "";		// no units defined
	// initialize parameter 7 ("m_rate_11")
	pi = self->__commonstate.params + 7;
	pi->name = "rate";
	pi->paramtype = GENLIB_PARAMTYPE_FLOAT;
	pi->defaultvalue = self->m_rate_11;
	pi->defaultref = 0;
	pi->hasinputminmax = false;
	pi->inputmin = 0; 
	pi->inputmax = 1;
	pi->hasminmax = true;
	pi->outputmin = 0.1;
	pi->outputmax = 10;
	pi->exp = 0;
	pi->units = "";		// no units defined
	// initialize parameter 8 ("m_time_12")
	pi = self->__commonstate.params + 8;
	pi->name = "time";
	pi->paramtype = GENLIB_PARAMTYPE_FLOAT;
	pi->defaultvalue = self->m_time_12;
	pi->defaultref = 0;
	pi->hasinputminmax = false;
	pi->inputmin = 0; 
	pi->inputmax = 1;
	pi->hasminmax = true;
	pi->outputmin = 10;
	pi->outputmax = 1000;
	pi->exp = 0;
	pi->units = "";		// no units defined
	// initialize parameter 9 ("m_feedback_13")
	pi = self->__commonstate.params + 9;
	pi->name = "feedback";
	pi->paramtype = GENLIB_PARAMTYPE_FLOAT;
	pi->defaultvalue = self->m_feedback_13;
	pi->defaultref = 0;
	pi->hasinputminmax = false;
	pi->inputmin = 0; 
	pi->inputmax = 1;
	pi->hasminmax = true;
	pi->outputmin = -0.7;
	pi->outputmax = 0.7;
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
