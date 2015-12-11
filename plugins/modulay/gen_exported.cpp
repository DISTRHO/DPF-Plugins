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
	DCBlock m_dcblock_15;
	Delay m_delay_3;
	Delay m_delay_4;
	SineCycle m_cycle_16;
	SineData __sinedata;
	double m_repeats_10;
	double m_mix_12;
	double m_rate_13;
	double m_depth_11;
	double m_delay_14;
	double m_cutoff_9;
	double m_time_7;
	double m_feedback_6;
	double samplerate;
	double m_bleed_8;
	double m_y_1;
	double m_feedforward_5;
	double m_history_2;
	int vectorsize;
	int __exception;
	// re-initialize all member variables;
	inline void reset(double __sr, int __vs) { 
		__exception = 0;
		vectorsize = __vs;
		samplerate = __sr;
		m_y_1 = 0;
		m_history_2 = 0;
		m_delay_3.reset("m_delay_3", 48000);
		m_delay_4.reset("m_delay_4", 48000);
		m_feedforward_5 = 100;
		m_feedback_6 = 0;
		m_time_7 = 500;
		m_bleed_8 = 0;
		m_cutoff_9 = 2500;
		m_repeats_10 = 75;
		m_depth_11 = 1;
		m_mix_12 = 100;
		m_rate_13 = 2;
		m_delay_14 = 0;
		m_dcblock_15.reset();
		m_cycle_16.reset(samplerate, 0);
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
		double mul_1174 = (m_mix_12 * 0.01);
		double mstosamps_1200 = (m_time_7 * (samplerate * 0.001));
		double expr_1204 = safediv(((m_cutoff_9 * 2) * 3.1415926535898), 48000);
		double sin_1181 = sin(expr_1204);
		double clamp_1182 = ((sin_1181 <= 1e-05) ? 1e-05 : ((sin_1181 >= 0.99999) ? 0.99999 : sin_1181));
		double mul_1176 = (m_bleed_8 * 0.01);
		double mul_1175 = (m_feedforward_5 * 0.01);
		double mul_1178 = (m_repeats_10 * 0.01);
		double mul_1177 = (m_feedback_6 * 0.01);
		// the main sample loop;
		while ((__n--)) { 
			const double in1 = (*(__in1++));
			double mix_1214 = (m_history_2 + (0.01 * (mstosamps_1200 - m_history_2)));
			double mix_1201 = mix_1214;
			double tap_1199 = m_delay_4.read_linear(mix_1201);
			double mix_1215 = (m_y_1 + (clamp_1182 * (tap_1199 - m_y_1)));
			double mix_1180 = mix_1215;
			double mul_1196 = (mix_1180 * mul_1178);
			double clamp_1205 = ((mul_1196 <= -1) ? -1 : ((mul_1196 >= 1) ? 1 : mul_1196));
			double dcblock_1203 = m_dcblock_15(mix_1180);
			m_cycle_16.freq(m_rate_13);
			double cycle_1185 = m_cycle_16(__sinedata);
			double cycleindex_1186 = m_cycle_16.phase();
			double add_1184 = (cycle_1185 + 1);
			double mul_1183 = (add_1184 * 0.5);
			double mul_1188 = (m_depth_11 * mul_1183);
			double add_1187 = (m_delay_14 + mul_1188);
			double mstosamps_1179 = (add_1187 * (samplerate * 0.001));
			double tap_1193 = m_delay_3.read_linear(mstosamps_1179);
			double mul_1191 = (tap_1193 * mul_1175);
			double mul_1189 = (tap_1193 * mul_1177);
			double add_1195 = (dcblock_1203 + mul_1189);
			double mul_1190 = (add_1195 * mul_1176);
			double add_1194 = (mul_1190 + mul_1191);
			double mul_1197 = (add_1194 * mul_1174);
			double out1 = (mul_1197 + in1);
			double history_1202_next_1206 = mix_1201;
			double y0_next_1207 = mix_1180;
			m_delay_4.write((clamp_1205 + in1));
			m_delay_3.write(add_1195);
			m_history_2 = history_1202_next_1206;
			m_y_1 = y0_next_1207;
			m_delay_3.step();
			m_delay_4.step();
			// assign results to output buffer;
			(*(__out1++)) = out1;
			
		};
		return __exception;
		
	};
	inline void set_feedforward(double _value) {
		m_feedforward_5 = (_value < 70 ? 70 : (_value > 100 ? 100 : _value));
	};
	inline void set_feedback(double _value) {
		m_feedback_6 = (_value < -70 ? -70 : (_value > 70 ? 70 : _value));
	};
	inline void set_time(double _value) {
		m_time_7 = (_value < 10 ? 10 : (_value > 1000 ? 1000 : _value));
	};
	inline void set_bleed(double _value) {
		m_bleed_8 = (_value < 0 ? 0 : (_value > 70 ? 70 : _value));
	};
	inline void set_cutoff(double _value) {
		m_cutoff_9 = (_value < 0 ? 0 : (_value > 6000 ? 6000 : _value));
	};
	inline void set_repeats(double _value) {
		m_repeats_10 = (_value < 0 ? 0 : (_value > 100 ? 100 : _value));
	};
	inline void set_depth(double _value) {
		m_depth_11 = (_value < 0 ? 0 : (_value > 5 ? 5 : _value));
	};
	inline void set_mix(double _value) {
		m_mix_12 = (_value < 0 ? 0 : (_value > 100 ? 100 : _value));
	};
	inline void set_rate(double _value) {
		m_rate_13 = (_value < 0.1 ? 0.1 : (_value > 10 ? 10 : _value));
	};
	inline void set_delay(double _value) {
		m_delay_14 = (_value < 0 ? 0 : (_value > 30 ? 30 : _value));
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
		case 0: self->set_feedforward(value); break;
		case 1: self->set_feedback(value); break;
		case 2: self->set_time(value); break;
		case 3: self->set_bleed(value); break;
		case 4: self->set_cutoff(value); break;
		case 5: self->set_repeats(value); break;
		case 6: self->set_depth(value); break;
		case 7: self->set_mix(value); break;
		case 8: self->set_rate(value); break;
		case 9: self->set_delay(value); break;
		
		default: break;
	}
}

/// Get the value of a parameter of a State object 

void getparameter(CommonState *cself, long index, double *value) {
	State *self = (State *)cself;
	switch (index) {
		case 0: *value = self->m_feedforward_5; break;
		case 1: *value = self->m_feedback_6; break;
		case 2: *value = self->m_time_7; break;
		case 3: *value = self->m_bleed_8; break;
		case 4: *value = self->m_cutoff_9; break;
		case 5: *value = self->m_repeats_10; break;
		case 6: *value = self->m_depth_11; break;
		case 7: *value = self->m_mix_12; break;
		case 8: *value = self->m_rate_13; break;
		case 9: *value = self->m_delay_14; break;
		
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
	// initialize parameter 0 ("m_feedforward_5")
	pi = self->__commonstate.params + 0;
	pi->name = "feedforward";
	pi->paramtype = GENLIB_PARAMTYPE_FLOAT;
	pi->defaultvalue = self->m_feedforward_5;
	pi->defaultref = 0;
	pi->hasinputminmax = false;
	pi->inputmin = 0; 
	pi->inputmax = 1;
	pi->hasminmax = true;
	pi->outputmin = 70;
	pi->outputmax = 100;
	pi->exp = 0;
	pi->units = "";		// no units defined
	// initialize parameter 1 ("m_feedback_6")
	pi = self->__commonstate.params + 1;
	pi->name = "feedback";
	pi->paramtype = GENLIB_PARAMTYPE_FLOAT;
	pi->defaultvalue = self->m_feedback_6;
	pi->defaultref = 0;
	pi->hasinputminmax = false;
	pi->inputmin = 0; 
	pi->inputmax = 1;
	pi->hasminmax = true;
	pi->outputmin = -70;
	pi->outputmax = 70;
	pi->exp = 0;
	pi->units = "";		// no units defined
	// initialize parameter 2 ("m_time_7")
	pi = self->__commonstate.params + 2;
	pi->name = "time";
	pi->paramtype = GENLIB_PARAMTYPE_FLOAT;
	pi->defaultvalue = self->m_time_7;
	pi->defaultref = 0;
	pi->hasinputminmax = false;
	pi->inputmin = 0; 
	pi->inputmax = 1;
	pi->hasminmax = true;
	pi->outputmin = 10;
	pi->outputmax = 1000;
	pi->exp = 0;
	pi->units = "";		// no units defined
	// initialize parameter 3 ("m_bleed_8")
	pi = self->__commonstate.params + 3;
	pi->name = "bleed";
	pi->paramtype = GENLIB_PARAMTYPE_FLOAT;
	pi->defaultvalue = self->m_bleed_8;
	pi->defaultref = 0;
	pi->hasinputminmax = false;
	pi->inputmin = 0; 
	pi->inputmax = 1;
	pi->hasminmax = true;
	pi->outputmin = 0;
	pi->outputmax = 70;
	pi->exp = 0;
	pi->units = "";		// no units defined
	// initialize parameter 4 ("m_cutoff_9")
	pi = self->__commonstate.params + 4;
	pi->name = "cutoff";
	pi->paramtype = GENLIB_PARAMTYPE_FLOAT;
	pi->defaultvalue = self->m_cutoff_9;
	pi->defaultref = 0;
	pi->hasinputminmax = false;
	pi->inputmin = 0; 
	pi->inputmax = 1;
	pi->hasminmax = true;
	pi->outputmin = 0;
	pi->outputmax = 6000;
	pi->exp = 0;
	pi->units = "";		// no units defined
	// initialize parameter 5 ("m_repeats_10")
	pi = self->__commonstate.params + 5;
	pi->name = "repeats";
	pi->paramtype = GENLIB_PARAMTYPE_FLOAT;
	pi->defaultvalue = self->m_repeats_10;
	pi->defaultref = 0;
	pi->hasinputminmax = false;
	pi->inputmin = 0; 
	pi->inputmax = 1;
	pi->hasminmax = true;
	pi->outputmin = 0;
	pi->outputmax = 100;
	pi->exp = 0;
	pi->units = "";		// no units defined
	// initialize parameter 6 ("m_depth_11")
	pi = self->__commonstate.params + 6;
	pi->name = "depth";
	pi->paramtype = GENLIB_PARAMTYPE_FLOAT;
	pi->defaultvalue = self->m_depth_11;
	pi->defaultref = 0;
	pi->hasinputminmax = false;
	pi->inputmin = 0; 
	pi->inputmax = 1;
	pi->hasminmax = true;
	pi->outputmin = 0;
	pi->outputmax = 5;
	pi->exp = 0;
	pi->units = "";		// no units defined
	// initialize parameter 7 ("m_mix_12")
	pi = self->__commonstate.params + 7;
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
	// initialize parameter 8 ("m_rate_13")
	pi = self->__commonstate.params + 8;
	pi->name = "rate";
	pi->paramtype = GENLIB_PARAMTYPE_FLOAT;
	pi->defaultvalue = self->m_rate_13;
	pi->defaultref = 0;
	pi->hasinputminmax = false;
	pi->inputmin = 0; 
	pi->inputmax = 1;
	pi->hasminmax = true;
	pi->outputmin = 0.1;
	pi->outputmax = 10;
	pi->exp = 0;
	pi->units = "";		// no units defined
	// initialize parameter 9 ("m_delay_14")
	pi = self->__commonstate.params + 9;
	pi->name = "delay";
	pi->paramtype = GENLIB_PARAMTYPE_FLOAT;
	pi->defaultvalue = self->m_delay_14;
	pi->defaultref = 0;
	pi->hasinputminmax = false;
	pi->inputmin = 0; 
	pi->inputmax = 1;
	pi->hasminmax = true;
	pi->outputmin = 0;
	pi->outputmax = 30;
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
