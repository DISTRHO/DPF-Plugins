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
	DCBlock m_dcblock_13;
	Delay m_delay_5;
	Delay m_delay_4;
	SineCycle m_cycle_14;
	SineData __sinedata;
	double m_morph_9;
	double m_tone_10;
	double m_mix_12;
	double m_repeats_11;
	double m_rate_8;
	double m_time_6;
	double m_smth_3;
	double m_depth_7;
	double m_smth_1;
	double samplerate;
	double m_y_2;
	int vectorsize;
	int __exception;
	// re-initialize all member variables;
	inline void reset(double __sr, int __vs) { 
		__exception = 0;
		vectorsize = __vs;
		samplerate = __sr;
		m_smth_1 = 0;
		m_y_2 = 0;
		m_smth_3 = 0;
		m_delay_4.reset("m_delay_4", 48000);
		m_delay_5.reset("m_delay_5", 48000);
		m_time_6 = 500;
		m_depth_7 = 1;
		m_rate_8 = 2;
		m_morph_9 = 50;
		m_tone_10 = 3000;
		m_repeats_11 = 75;
		m_mix_12 = 100;
		m_dcblock_13.reset();
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
		double mul_12532 = (m_mix_12 * 0.01);
		double mstosamps_12544 = (m_time_6 * (samplerate * 0.001));
		double expr_12576 = safediv(((m_tone_10 * 2) * 3.1415926535898), 48000);
		double sin_12539 = sin(expr_12576);
		double clamp_12540 = ((sin_12539 <= 1e-05) ? 1e-05 : ((sin_12539 >= 0.99999) ? 0.99999 : sin_12539));
		double mul_12536 = (m_repeats_11 * 0.01);
		double mul_12557 = (m_morph_9 * -0.8);
		double add_12555 = (mul_12557 + 40);
		double abs_12556 = fabs(add_12555);
		double mul_12560 = (abs_12556 * 0.01);
		int lt_12547 = (m_morph_9 < 50);
		double mul_12529 = (m_morph_9 * -0.8);
		double add_12527 = (mul_12529 + 40);
		double abs_12528 = fabs(add_12527);
		double rsub_12526 = (80 - abs_12528);
		double mul_12530 = (rsub_12526 * 0.01);
		double mul_12546 = (m_morph_9 * -0.4);
		double add_12545 = (mul_12546 + 20);
		double mul_12548 = (lt_12547 * add_12545);
		double mul_12559 = (m_morph_9 * 0.8);
		double sub_12558 = (mul_12559 - 40);
		double mul_12561 = (sub_12558 * 0.01);
		int gt_12553 = (m_morph_9 > 50);
		double mul_12552 = (gt_12553 * m_morph_9);
		double sub_12551 = (mul_12552 - 50);
		double clamp_12550 = ((sub_12551 <= 0) ? 0 : ((sub_12551 >= 50) ? 50 : sub_12551));
		double mul_12554 = (clamp_12550 * 0.04);
		double add_12549 = (mul_12548 + mul_12554);
		// the main sample loop;
		while ((__n--)) { 
			const double in1 = (*(__in1++));
			double mix_12584 = (mstosamps_12544 + (0.999 * (m_smth_3 - mstosamps_12544)));
			double mix_12533 = mix_12584;
			double tap_12543 = m_delay_5.read_linear(mix_12533);
			double mix_12585 = (m_y_2 + (clamp_12540 * (tap_12543 - m_y_2)));
			double mix_12537 = mix_12585;
			double mul_12541 = (mix_12537 * mul_12536);
			double dcblock_12534 = m_dcblock_13(mul_12541);
			double clamp_12535 = ((dcblock_12534 <= -1) ? -1 : ((dcblock_12534 >= 1) ? 1 : dcblock_12534));
			double mul_12566 = (mix_12537 * mul_12560);
			m_cycle_14.freq(m_rate_8);
			double cycle_12571 = m_cycle_14(__sinedata);
			double cycleindex_12572 = m_cycle_14.phase();
			double add_12570 = (cycle_12571 + 1);
			double mul_12569 = (add_12570 * 0.5);
			double mul_12573 = (m_depth_7 * mul_12569);
			double add_12562 = (mul_12573 + add_12549);
			double mstosamps_12568 = (add_12562 * (samplerate * 0.001));
			double mix_12586 = (mstosamps_12568 + (0.999 * (m_smth_1 - mstosamps_12568)));
			double mix_12567 = mix_12586;
			double tap_12575 = m_delay_4.read_linear(mix_12567);
			double mul_12563 = (tap_12575 * mul_12530);
			double mul_12531 = ((mul_12563 + mul_12566) * mul_12532);
			double out1 = (mul_12531 + in1);
			double mul_12564 = (tap_12575 * mul_12561);
			double add_12565 = (mix_12537 + mul_12564);
			double smth1_next_12577 = mix_12533;
			double y0_next_12578 = mix_12537;
			double smth2_next_12579 = mix_12567;
			m_delay_5.write((clamp_12535 + in1));
			m_delay_4.write(add_12565);
			m_smth_3 = smth1_next_12577;
			m_y_2 = y0_next_12578;
			m_smth_1 = smth2_next_12579;
			m_delay_4.step();
			m_delay_5.step();
			// assign results to output buffer;
			(*(__out1++)) = out1;
			
		};
		return __exception;
		
	};
	inline void set_time(double _value) {
		m_time_6 = (_value < 20 ? 20 : (_value > 1000 ? 1000 : _value));
	};
	inline void set_depth(double _value) {
		m_depth_7 = (_value < 0 ? 0 : (_value > 3 ? 3 : _value));
	};
	inline void set_rate(double _value) {
		m_rate_8 = (_value < 0.1 ? 0.1 : (_value > 5 ? 5 : _value));
	};
	inline void set_morph(double _value) {
		m_morph_9 = (_value < 0 ? 0 : (_value > 100 ? 100 : _value));
	};
	inline void set_tone(double _value) {
		m_tone_10 = (_value < 500 ? 500 : (_value > 6000 ? 6000 : _value));
	};
	inline void set_repeats(double _value) {
		m_repeats_11 = (_value < 0 ? 0 : (_value > 110 ? 110 : _value));
	};
	inline void set_mix(double _value) {
		m_mix_12 = (_value < 0 ? 0 : (_value > 100 ? 100 : _value));
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
int num_params() { return 7; }

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
		case 0: self->set_time(value); break;
		case 1: self->set_depth(value); break;
		case 2: self->set_rate(value); break;
		case 3: self->set_morph(value); break;
		case 4: self->set_tone(value); break;
		case 5: self->set_repeats(value); break;
		case 6: self->set_mix(value); break;
		
		default: break;
	}
}

/// Get the value of a parameter of a State object 

void getparameter(CommonState *cself, long index, double *value) {
	State *self = (State *)cself;
	switch (index) {
		case 0: *value = self->m_time_6; break;
		case 1: *value = self->m_depth_7; break;
		case 2: *value = self->m_rate_8; break;
		case 3: *value = self->m_morph_9; break;
		case 4: *value = self->m_tone_10; break;
		case 5: *value = self->m_repeats_11; break;
		case 6: *value = self->m_mix_12; break;
		
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
	self->__commonstate.params = (ParamInfo *)genlib_sysmem_newptr(7 * sizeof(ParamInfo));
	self->__commonstate.numparams = 7;
	// initialize parameter 0 ("m_time_6")
	pi = self->__commonstate.params + 0;
	pi->name = "time";
	pi->paramtype = GENLIB_PARAMTYPE_FLOAT;
	pi->defaultvalue = self->m_time_6;
	pi->defaultref = 0;
	pi->hasinputminmax = false;
	pi->inputmin = 0; 
	pi->inputmax = 1;
	pi->hasminmax = true;
	pi->outputmin = 20;
	pi->outputmax = 1000;
	pi->exp = 0;
	pi->units = "";		// no units defined
	// initialize parameter 1 ("m_depth_7")
	pi = self->__commonstate.params + 1;
	pi->name = "depth";
	pi->paramtype = GENLIB_PARAMTYPE_FLOAT;
	pi->defaultvalue = self->m_depth_7;
	pi->defaultref = 0;
	pi->hasinputminmax = false;
	pi->inputmin = 0; 
	pi->inputmax = 1;
	pi->hasminmax = true;
	pi->outputmin = 0;
	pi->outputmax = 3;
	pi->exp = 0;
	pi->units = "";		// no units defined
	// initialize parameter 2 ("m_rate_8")
	pi = self->__commonstate.params + 2;
	pi->name = "rate";
	pi->paramtype = GENLIB_PARAMTYPE_FLOAT;
	pi->defaultvalue = self->m_rate_8;
	pi->defaultref = 0;
	pi->hasinputminmax = false;
	pi->inputmin = 0; 
	pi->inputmax = 1;
	pi->hasminmax = true;
	pi->outputmin = 0.1;
	pi->outputmax = 5;
	pi->exp = 0;
	pi->units = "";		// no units defined
	// initialize parameter 3 ("m_morph_9")
	pi = self->__commonstate.params + 3;
	pi->name = "morph";
	pi->paramtype = GENLIB_PARAMTYPE_FLOAT;
	pi->defaultvalue = self->m_morph_9;
	pi->defaultref = 0;
	pi->hasinputminmax = false;
	pi->inputmin = 0; 
	pi->inputmax = 1;
	pi->hasminmax = true;
	pi->outputmin = 0;
	pi->outputmax = 100;
	pi->exp = 0;
	pi->units = "";		// no units defined
	// initialize parameter 4 ("m_tone_10")
	pi = self->__commonstate.params + 4;
	pi->name = "tone";
	pi->paramtype = GENLIB_PARAMTYPE_FLOAT;
	pi->defaultvalue = self->m_tone_10;
	pi->defaultref = 0;
	pi->hasinputminmax = false;
	pi->inputmin = 0; 
	pi->inputmax = 1;
	pi->hasminmax = true;
	pi->outputmin = 500;
	pi->outputmax = 6000;
	pi->exp = 0;
	pi->units = "";		// no units defined
	// initialize parameter 5 ("m_repeats_11")
	pi = self->__commonstate.params + 5;
	pi->name = "repeats";
	pi->paramtype = GENLIB_PARAMTYPE_FLOAT;
	pi->defaultvalue = self->m_repeats_11;
	pi->defaultref = 0;
	pi->hasinputminmax = false;
	pi->inputmin = 0; 
	pi->inputmax = 1;
	pi->hasminmax = true;
	pi->outputmin = 0;
	pi->outputmax = 110;
	pi->exp = 0;
	pi->units = "";		// no units defined
	// initialize parameter 6 ("m_mix_12")
	pi = self->__commonstate.params + 6;
	pi->name = "mix";
	pi->paramtype = GENLIB_PARAMTYPE_FLOAT;
	pi->defaultvalue = self->m_mix_12;
	pi->defaultref = 0;
	pi->hasinputminmax = false;
	pi->inputmin = 0; 
	pi->inputmax = 1;
	pi->hasminmax = true;
	pi->outputmin = 0;
	pi->outputmax = 100;
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
