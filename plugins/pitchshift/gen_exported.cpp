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
	Delay m_delay_5;
	Delta m_delta_14;
	Delta m_delta_11;
	Delta m_delta_20;
	Delta m_delta_17;
	Phasor m_phasor_10;
	Sah m_sah_13;
	Sah m_sah_12;
	Sah m_sah_15;
	Sah m_sah_16;
	Sah m_sah_21;
	Sah m_sah_19;
	Sah m_sah_18;
	Sah m_sah_22;
	t_sample samples_to_seconds;
	t_sample m_ratio_8;
	t_sample m_xfade_9;
	t_sample m_history_1;
	t_sample samplerate;
	t_sample m_history_3;
	t_sample m_history_2;
	t_sample m_blur_6;
	t_sample m_window_7;
	t_sample m_history_4;
	int vectorsize;
	int __exception;
	// re-initialize all member variables;
	inline void reset(t_sample __sr, int __vs) {
		__exception = 0;
		vectorsize = __vs;
		samplerate = __sr;
		m_history_1 = 0;
		m_history_2 = 0;
		m_history_3 = 0;
		m_history_4 = 0;
		m_delay_5.reset("m_delay_5", 88200);
		m_blur_6 = 0;
		m_window_7 = 100;
		m_ratio_8 = 1;
		m_xfade_9 = 1;
		samples_to_seconds = (1 / samplerate);
		m_phasor_10.reset(0);
		m_delta_11.reset(0);
		m_sah_12.reset(0);
		m_sah_13.reset(0);
		m_delta_14.reset(0);
		m_sah_15.reset(0);
		m_sah_16.reset(0);
		m_delta_17.reset(0);
		m_sah_18.reset(0);
		m_sah_19.reset(0);
		m_delta_20.reset(0);
		m_sah_21.reset(0);
		m_sah_22.reset(0);
		genlib_reset_complete(this);
		
	};
	// the signal processing routine;
	inline int perform(t_sample ** __ins, t_sample ** __outs, int __n) { 
		vectorsize = __n;
		const t_sample * __in1 = __ins[0];
		t_sample * __out1 = __outs[0];
		t_sample * __out2 = __outs[1];
		if (__exception) { 
			return __exception;
			
		} else if (( (__in1 == 0) || (__out1 == 0) || (__out2 == 0) )) { 
			__exception = GENLIB_ERR_NULL_BUFFER;
			return __exception;
			
		};
		t_sample mstosamps_1223 = (m_window_7 * (samplerate * 0.001));
		t_sample rsub_1209 = (1 - m_ratio_8);
		t_sample mul_1208 = (rsub_1209 * 1000);
		t_sample div_1207 = safediv(mul_1208, m_window_7);
		samples_to_seconds = (1 / samplerate);
		// the main sample loop;
		while ((__n--)) { 
			const t_sample in1 = (*(__in1++));
			t_sample noise_1159 = noise();
			t_sample abs_1177 = fabs(noise_1159);
			t_sample mul_1183 = (abs_1177 * m_blur_6);
			t_sample noise_1161 = noise();
			t_sample abs_1179 = fabs(noise_1161);
			t_sample mul_1189 = (abs_1179 * m_blur_6);
			t_sample noise_1160 = noise();
			t_sample abs_1178 = fabs(noise_1160);
			t_sample mul_1186 = (abs_1178 * m_blur_6);
			t_sample noise_1158 = noise();
			t_sample abs_1176 = fabs(noise_1158);
			t_sample mul_1180 = (abs_1176 * m_blur_6);
			t_sample phasor_1229 = m_phasor_10(div_1207, samples_to_seconds);
			t_sample add_1206 = ((m_history_4 + phasor_1229) + 0.5);
			t_sample mod_1205 = safemod(add_1206, 1);
			t_sample delta_1185 = m_delta_11(mod_1205);
			t_sample sah_1164 = m_sah_12(mul_1183, delta_1185, 0);
			t_sample sah_1184 = m_sah_13(mstosamps_1223, delta_1185, 0);
			t_sample mul_1173 = (sah_1184 * mod_1205);
			t_sample sub_1204 = (mod_1205 - 0.5);
			t_sample mul_1203 = (sub_1204 * 3.1415926535898);
			t_sample cos_1202 = cos(mul_1203);
			t_sample mul_1192 = (cos_1202 * cos_1202);
			t_sample add_1228 = ((m_history_3 + phasor_1229) + 0);
			t_sample mod_1227 = safemod(add_1228, 1);
			t_sample delta_1169 = m_delta_14(mod_1227);
			t_sample sah_1168 = m_sah_15(mul_1189, delta_1169, 0);
			t_sample sah_1190 = m_sah_16(mstosamps_1223, delta_1169, 0);
			t_sample mul_1175 = (sah_1190 * mod_1227);
			t_sample sub_1226 = (mod_1227 - 0.5);
			t_sample mul_1225 = (sub_1226 * 3.1415926535898);
			t_sample cos_1224 = cos(mul_1225);
			t_sample mul_1194 = (cos_1224 * cos_1224);
			t_sample add_1222 = ((m_history_2 + phasor_1229) + 0.25);
			t_sample mod_1221 = safemod(add_1222, 1);
			t_sample delta_1188 = m_delta_17(mod_1221);
			t_sample sah_1166 = m_sah_18(mul_1186, delta_1188, 0);
			t_sample sah_1187 = m_sah_19(mstosamps_1223, delta_1188, 0);
			t_sample mul_1174 = (sah_1187 * mod_1221);
			t_sample sub_1220 = (mod_1221 - 0.5);
			t_sample mul_1219 = (sub_1220 * 3.1415926535898);
			t_sample cos_1218 = cos(mul_1219);
			t_sample mul_1193 = (cos_1218 * cos_1218);
			t_sample add_1200 = ((m_history_1 + phasor_1229) + 0.75);
			t_sample mod_1199 = safemod(add_1200, 1);
			t_sample delta_1182 = m_delta_20(mod_1199);
			t_sample sah_1162 = m_sah_21(mul_1180, delta_1182, 0);
			t_sample sah_1181 = m_sah_22(mstosamps_1223, delta_1182, 0);
			t_sample mul_1172 = (sah_1181 * mod_1199);
			t_sample tap_1214 = m_delay_5.read_linear(mul_1175);
			t_sample tap_1215 = m_delay_5.read_linear(mul_1174);
			t_sample tap_1216 = m_delay_5.read_linear(mul_1173);
			t_sample tap_1217 = m_delay_5.read_linear(mul_1172);
			t_sample mul_1212 = (tap_1214 * mul_1194);
			t_sample mul_1201 = (tap_1216 * mul_1192);
			t_sample add_1245 = (mul_1201 + mul_1212);
			t_sample mix_1244 = (in1 + (m_xfade_9 * (add_1245 - in1)));
			t_sample out1 = mix_1244;
			t_sample mul_1211 = (tap_1215 * mul_1193);
			t_sample sub_1198 = (mod_1199 - 0.5);
			t_sample mul_1197 = (sub_1198 * 3.1415926535898);
			t_sample cos_1196 = cos(mul_1197);
			t_sample mul_1191 = (cos_1196 * cos_1196);
			t_sample mul_1195 = (tap_1217 * mul_1191);
			t_sample add_1247 = (mul_1195 + mul_1211);
			t_sample mix_1246 = (in1 + (m_xfade_9 * (add_1247 - in1)));
			t_sample out2 = mix_1246;
			t_sample history_1165_next_1240 = sah_1164;
			t_sample history_1170_next_1241 = sah_1168;
			t_sample history_1167_next_1242 = sah_1166;
			t_sample history_1163_next_1243 = sah_1162;
			m_delay_5.write(in1);
			m_history_4 = history_1165_next_1240;
			m_history_3 = history_1170_next_1241;
			m_history_2 = history_1167_next_1242;
			m_history_1 = history_1163_next_1243;
			m_delay_5.step();
			// assign results to output buffer;
			(*(__out1++)) = out1;
			(*(__out2++)) = out2;
			
		};
		return __exception;
		
	};
	inline void set_blur(t_sample _value) {
		m_blur_6 = (_value < 0 ? 0 : (_value > 0.25 ? 0.25 : _value));
	};
	inline void set_window(t_sample _value) {
		m_window_7 = (_value < 0.1 ? 0.1 : (_value > 1000 ? 1000 : _value));
	};
	inline void set_ratio(t_sample _value) {
		m_ratio_8 = (_value < 0.25 ? 0.25 : (_value > 4 ? 4 : _value));
	};
	inline void set_xfade(t_sample _value) {
		m_xfade_9 = (_value < 0 ? 0 : (_value > 1 ? 1 : _value));
	};
	
} State;


/// 
///	Configuration for the genlib API
///

/// Number of signal inputs and outputs 

const int gen_kernel_numins = 1;
const int gen_kernel_numouts = 2;

int num_inputs() { return gen_kernel_numins; }
int num_outputs() { return gen_kernel_numouts; }
int num_params() { return 4; }

/// Assistive lables for the signal inputs and outputs 

const char * gen_kernel_innames[] = { "in1" };
const char * gen_kernel_outnames[] = { "out1", "out2" };

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

void setparameter(CommonState *cself, long index, t_param value, void *ref) {
	State * self = (State *)cself;
	switch (index) {
		case 0: self->set_blur(value); break;
		case 1: self->set_window(value); break;
		case 2: self->set_ratio(value); break;
		case 3: self->set_xfade(value); break;
		
		default: break;
	}
}

/// Get the value of a parameter of a State object 

void getparameter(CommonState *cself, long index, t_param *value) {
	State *self = (State *)cself;
	switch (index) {
		case 0: *value = self->m_blur_6; break;
		case 1: *value = self->m_window_7; break;
		case 2: *value = self->m_ratio_8; break;
		case 3: *value = self->m_xfade_9; break;
		
		default: break;
	}
}

/// Allocate and configure a new State object and it's internal CommonState:

void * create(t_param sr, long vs) {
	State *self = new State;
	self->reset(sr, vs);
	ParamInfo *pi;
	self->__commonstate.inputnames = gen_kernel_innames;
	self->__commonstate.outputnames = gen_kernel_outnames;
	self->__commonstate.numins = gen_kernel_numins;
	self->__commonstate.numouts = gen_kernel_numouts;
	self->__commonstate.sr = sr;
	self->__commonstate.vs = vs;
	self->__commonstate.params = (ParamInfo *)genlib_sysmem_newptr(4 * sizeof(ParamInfo));
	self->__commonstate.numparams = 4;
	// initialize parameter 0 ("m_blur_6")
	pi = self->__commonstate.params + 0;
	pi->name = "blur";
	pi->paramtype = GENLIB_PARAMTYPE_FLOAT;
	pi->defaultvalue = self->m_blur_6;
	pi->defaultref = 0;
	pi->hasinputminmax = false;
	pi->inputmin = 0; 
	pi->inputmax = 1;
	pi->hasminmax = true;
	pi->outputmin = 0;
	pi->outputmax = 0.25;
	pi->exp = 0;
	pi->units = "";		// no units defined
	// initialize parameter 1 ("m_window_7")
	pi = self->__commonstate.params + 1;
	pi->name = "window";
	pi->paramtype = GENLIB_PARAMTYPE_FLOAT;
	pi->defaultvalue = self->m_window_7;
	pi->defaultref = 0;
	pi->hasinputminmax = false;
	pi->inputmin = 0; 
	pi->inputmax = 1;
	pi->hasminmax = true;
	pi->outputmin = 0.1;
	pi->outputmax = 1000;
	pi->exp = 0;
	pi->units = "";		// no units defined
	// initialize parameter 2 ("m_ratio_8")
	pi = self->__commonstate.params + 2;
	pi->name = "ratio";
	pi->paramtype = GENLIB_PARAMTYPE_FLOAT;
	pi->defaultvalue = self->m_ratio_8;
	pi->defaultref = 0;
	pi->hasinputminmax = false;
	pi->inputmin = 0; 
	pi->inputmax = 1;
	pi->hasminmax = true;
	pi->outputmin = 0.25;
	pi->outputmax = 4;
	pi->exp = 0;
	pi->units = "";		// no units defined
	// initialize parameter 3 ("m_xfade_9")
	pi = self->__commonstate.params + 3;
	pi->name = "xfade";
	pi->paramtype = GENLIB_PARAMTYPE_FLOAT;
	pi->defaultvalue = self->m_xfade_9;
	pi->defaultref = 0;
	pi->hasinputminmax = false;
	pi->inputmin = 0; 
	pi->inputmax = 1;
	pi->hasminmax = true;
	pi->outputmin = 0;
	pi->outputmax = 1;
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
