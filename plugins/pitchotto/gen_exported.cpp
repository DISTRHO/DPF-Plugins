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
	Delay m_delay_10;
	Delay m_delay_11;
	Delta m_delta_29;
	Delta m_delta_26;
	Delta m_delta_23;
	Delta m_delta_33;
	Delta m_delta_36;
	Delta m_delta_20;
	Delta m_delta_39;
	Delta m_delta_42;
	Phasor m_phasor_32;
	Phasor m_phasor_19;
	Sah m_sah_25;
	Sah m_sah_35;
	Sah m_sah_24;
	Sah m_sah_27;
	Sah m_sah_28;
	Sah m_sah_41;
	Sah m_sah_30;
	Sah m_sah_34;
	Sah m_sah_37;
	Sah m_sah_38;
	Sah m_sah_31;
	Sah m_sah_40;
	Sah m_sah_22;
	Sah m_sah_44;
	Sah m_sah_43;
	Sah m_sah_21;
	double m_y_1;
	double m_history_5;
	double m_history_6;
	double samplerate;
	double m_history_4;
	double m_history_2;
	double m_history_7;
	double m_history_3;
	double m_history_8;
	double m_ratio_12;
	double m_history_9;
	double m_delay_18;
	double samples_to_seconds;
	double m_cutoff_16;
	double m_blur_17;
	double m_delay_14;
	double m_mix_13;
	double m_ratio_15;
	int vectorsize;
	int __exception;
	// re-initialize all member variables;
	inline void reset(double __sr, int __vs) { 
		__exception = 0;
		vectorsize = __vs;
		samplerate = __sr;
		m_y_1 = 0;
		m_history_2 = 0;
		m_history_3 = 0;
		m_history_4 = 0;
		m_history_5 = 0;
		m_history_6 = 0;
		m_history_7 = 0;
		m_history_8 = 0;
		m_history_9 = 0;
		m_delay_10.reset("m_delay_10", 96000);
		m_delay_11.reset("m_delay_11", 96000);
		m_ratio_12 = 2;
		m_mix_13 = 0.5;
		m_delay_14 = 100;
		m_ratio_15 = 0.5;
		m_cutoff_16 = 2250;
		m_blur_17 = 0.25;
		m_delay_18 = 100;
		samples_to_seconds = (1 / samplerate);
		m_phasor_19.reset(0);
		m_delta_20.reset(0);
		m_sah_21.reset(0);
		m_sah_22.reset(0);
		m_delta_23.reset(0);
		m_sah_24.reset(0);
		m_sah_25.reset(0);
		m_delta_26.reset(0);
		m_sah_27.reset(0);
		m_sah_28.reset(0);
		m_delta_29.reset(0);
		m_sah_30.reset(0);
		m_sah_31.reset(0);
		m_phasor_32.reset(0);
		m_delta_33.reset(0);
		m_sah_34.reset(0);
		m_sah_35.reset(0);
		m_delta_36.reset(0);
		m_sah_37.reset(0);
		m_sah_38.reset(0);
		m_delta_39.reset(0);
		m_sah_40.reset(0);
		m_sah_41.reset(0);
		m_delta_42.reset(0);
		m_sah_43.reset(0);
		m_sah_44.reset(0);
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
		double expr_5155 = safediv(((m_cutoff_16 * 2) * 3.1415926535898), 44100);
		double sin_5012 = sin(expr_5155);
		double clamp_5013 = ((sin_5012 <= 1e-05) ? 1e-05 : ((sin_5012 >= 0.99999) ? 0.99999 : sin_5012));
		double mstosamps_5148 = (m_delay_14 * (samplerate * 0.001));
		double mstosamps_5077 = (m_delay_18 * (samplerate * 0.001));
		double rsub_5064 = (1 - m_ratio_12);
		double mul_5063 = (rsub_5064 * 1000);
		double div_5062 = safediv(mul_5063, m_delay_18);
		samples_to_seconds = (1 / samplerate);
		double rsub_5135 = (1 - m_ratio_15);
		double mul_5134 = (rsub_5135 * 1000);
		double div_5133 = safediv(mul_5134, m_delay_14);
		// the main sample loop;
		while ((__n--)) { 
			const double in1 = (*(__in1++));
			double noise_5014 = noise();
			double abs_5031 = fabs(noise_5014);
			double mul_5035 = (abs_5031 * m_blur_17);
			double noise_5086 = noise();
			double abs_5104 = fabs(noise_5086);
			double mul_5112 = (abs_5104 * m_blur_17);
			double noise_5015 = noise();
			double abs_5032 = fabs(noise_5015);
			double mul_5038 = (abs_5032 * m_blur_17);
			double noise_5084 = noise();
			double abs_5102 = fabs(noise_5084);
			double mul_5106 = (abs_5102 * m_blur_17);
			double noise_5017 = noise();
			double abs_5034 = fabs(noise_5017);
			double mul_5044 = (abs_5034 * m_blur_17);
			double noise_5087 = noise();
			double abs_5105 = fabs(noise_5087);
			double mul_5115 = (abs_5105 * m_blur_17);
			double noise_5016 = noise();
			double abs_5033 = fabs(noise_5016);
			double mul_5041 = (abs_5033 * m_blur_17);
			double noise_5085 = noise();
			double abs_5103 = fabs(noise_5085);
			double mul_5109 = (abs_5103 * m_blur_17);
			double phasor_5083 = m_phasor_19(div_5062, samples_to_seconds);
			double add_5055 = ((phasor_5083 + m_history_9) + 0.75);
			double mod_5054 = safemod(add_5055, 1);
			double delta_5037 = m_delta_20(mod_5054);
			double sah_5018 = m_sah_21(mul_5035, delta_5037, 0);
			double sah_5036 = m_sah_22(mstosamps_5077, delta_5037, 0);
			double mul_5027 = (sah_5036 * mod_5054);
			double sub_5053 = (mod_5054 - 0.5);
			double mul_5052 = (sub_5053 * 3.1415926535898);
			double cos_5051 = cos(mul_5052);
			double mul_5046 = (cos_5051 * cos_5051);
			double add_5061 = ((phasor_5083 + m_history_8) + 0.5);
			double mod_5060 = safemod(add_5061, 1);
			double delta_5040 = m_delta_23(mod_5060);
			double sah_5020 = m_sah_24(mul_5038, delta_5040, 0);
			double sah_5039 = m_sah_25(mstosamps_5077, delta_5040, 0);
			double mul_5028 = (sah_5039 * mod_5060);
			double sub_5059 = (mod_5060 - 0.5);
			double mul_5058 = (sub_5059 * 3.1415926535898);
			double cos_5057 = cos(mul_5058);
			double mul_5047 = (cos_5057 * cos_5057);
			double add_5082 = ((phasor_5083 + m_history_7) + 0);
			double mod_5081 = safemod(add_5082, 1);
			double delta_5025 = m_delta_26(mod_5081);
			double sah_5024 = m_sah_27(mul_5044, delta_5025, 0);
			double sah_5045 = m_sah_28(mstosamps_5077, delta_5025, 0);
			double mul_5030 = (sah_5045 * mod_5081);
			double sub_5080 = (mod_5081 - 0.5);
			double mul_5079 = (sub_5080 * 3.1415926535898);
			double cos_5078 = cos(mul_5079);
			double mul_5049 = (cos_5078 * cos_5078);
			double add_5076 = ((phasor_5083 + m_history_6) + 0.25);
			double mod_5075 = safemod(add_5076, 1);
			double delta_5043 = m_delta_29(mod_5075);
			double sah_5022 = m_sah_30(mul_5041, delta_5043, 0);
			double sah_5042 = m_sah_31(mstosamps_5077, delta_5043, 0);
			double mul_5029 = (sah_5042 * mod_5075);
			double tap_5068 = m_delay_11.read_linear(mul_5030);
			double tap_5069 = m_delay_11.read_linear(mul_5029);
			double tap_5070 = m_delay_11.read_linear(mul_5028);
			double tap_5071 = m_delay_11.read_linear(mul_5027);
			double mul_5050 = (tap_5071 * mul_5046);
			double mul_5056 = (tap_5070 * mul_5047);
			double mul_5066 = (tap_5068 * mul_5049);
			double sub_5074 = (mod_5075 - 0.5);
			double mul_5073 = (sub_5074 * 3.1415926535898);
			double cos_5072 = cos(mul_5073);
			double mul_5048 = (cos_5072 * cos_5072);
			double mul_5065 = (tap_5069 * mul_5048);
			double phasor_5154 = m_phasor_32(div_5133, samples_to_seconds);
			double add_5147 = ((m_history_5 + phasor_5154) + 0.25);
			double mod_5146 = safemod(add_5147, 1);
			double delta_5114 = m_delta_33(mod_5146);
			double sah_5092 = m_sah_34(mul_5112, delta_5114, 0);
			double sah_5113 = m_sah_35(mstosamps_5148, delta_5114, 0);
			double mul_5100 = (sah_5113 * mod_5146);
			double sub_5145 = (mod_5146 - 0.5);
			double mul_5144 = (sub_5145 * 3.1415926535898);
			double cos_5143 = cos(mul_5144);
			double mul_5119 = (cos_5143 * cos_5143);
			double add_5126 = ((m_history_4 + phasor_5154) + 0.75);
			double mod_5125 = safemod(add_5126, 1);
			double delta_5108 = m_delta_36(mod_5125);
			double sah_5088 = m_sah_37(mul_5106, delta_5108, 0);
			double sah_5107 = m_sah_38(mstosamps_5148, delta_5108, 0);
			double mul_5098 = (sah_5107 * mod_5125);
			double sub_5124 = (mod_5125 - 0.5);
			double mul_5123 = (sub_5124 * 3.1415926535898);
			double cos_5122 = cos(mul_5123);
			double mul_5117 = (cos_5122 * cos_5122);
			double add_5153 = ((m_history_3 + phasor_5154) + 0);
			double mod_5152 = safemod(add_5153, 1);
			double delta_5095 = m_delta_39(mod_5152);
			double sah_5094 = m_sah_40(mul_5115, delta_5095, 0);
			double sah_5116 = m_sah_41(mstosamps_5148, delta_5095, 0);
			double mul_5101 = (sah_5116 * mod_5152);
			double sub_5151 = (mod_5152 - 0.5);
			double mul_5150 = (sub_5151 * 3.1415926535898);
			double cos_5149 = cos(mul_5150);
			double mul_5120 = (cos_5149 * cos_5149);
			double add_5132 = ((m_history_2 + phasor_5154) + 0.5);
			double mod_5131 = safemod(add_5132, 1);
			double delta_5111 = m_delta_42(mod_5131);
			double sah_5090 = m_sah_43(mul_5109, delta_5111, 0);
			double sah_5110 = m_sah_44(mstosamps_5148, delta_5111, 0);
			double mul_5099 = (sah_5110 * mod_5131);
			double tap_5139 = m_delay_10.read_linear(mul_5101);
			double tap_5140 = m_delay_10.read_linear(mul_5100);
			double tap_5141 = m_delay_10.read_linear(mul_5099);
			double tap_5142 = m_delay_10.read_linear(mul_5098);
			double mul_5137 = (tap_5139 * mul_5120);
			double mul_5136 = (tap_5140 * mul_5119);
			double mul_5121 = (tap_5142 * mul_5117);
			double sub_5130 = (mod_5131 - 0.5);
			double mul_5129 = (sub_5130 * 3.1415926535898);
			double cos_5128 = cos(mul_5129);
			double mul_5118 = (cos_5128 * cos_5128);
			double mul_5127 = (tap_5141 * mul_5118);
			double add_5174 = (((((((mul_5066 + mul_5065) + mul_5056) + mul_5050) + mul_5121) + mul_5127) + mul_5136) + mul_5137);
			double mix_5173 = (m_y_1 + (clamp_5013 * (add_5174 - m_y_1)));
			double mix_5010 = mix_5173;
			double mix_5175 = (in1 + (m_mix_13 * (mix_5010 - in1)));
			double out1 = mix_5175;
			double history_5019_next_5156 = sah_5018;
			double history_5021_next_5157 = sah_5020;
			double history_5026_next_5158 = sah_5024;
			double history_5023_next_5159 = sah_5022;
			double history_5093_next_5160 = sah_5092;
			double history_5089_next_5161 = sah_5088;
			double history_5096_next_5162 = sah_5094;
			double history_5091_next_5163 = sah_5090;
			double y0_next_5164 = mix_5010;
			m_delay_11.write(in1);
			m_delay_10.write(in1);
			m_history_9 = history_5019_next_5156;
			m_history_8 = history_5021_next_5157;
			m_history_7 = history_5026_next_5158;
			m_history_6 = history_5023_next_5159;
			m_history_5 = history_5093_next_5160;
			m_history_4 = history_5089_next_5161;
			m_history_3 = history_5096_next_5162;
			m_history_2 = history_5091_next_5163;
			m_y_1 = y0_next_5164;
			m_delay_10.step();
			m_delay_11.step();
			// assign results to output buffer;
			(*(__out1++)) = out1;
			
		};
		return __exception;
		
	};
	inline void set_ratio2(double _value) {
		m_ratio_12 = (_value < 0.5 ? 0.5 : (_value > 2 ? 2 : _value));
	};
	inline void set_mix(double _value) {
		m_mix_13 = (_value < 0 ? 0 : (_value > 1 ? 1 : _value));
	};
	inline void set_delay1(double _value) {
		m_delay_14 = (_value < 0.1 ? 0.1 : (_value > 1000 ? 1000 : _value));
	};
	inline void set_ratio1(double _value) {
		m_ratio_15 = (_value < 0.5 ? 0.5 : (_value > 2 ? 2 : _value));
	};
	inline void set_cutoff(double _value) {
		m_cutoff_16 = (_value < 0 ? 0 : (_value > 3000 ? 3000 : _value));
	};
	inline void set_blur(double _value) {
		m_blur_17 = (_value < 0.01 ? 0.01 : (_value > 0.25 ? 0.25 : _value));
	};
	inline void set_delay2(double _value) {
		m_delay_18 = (_value < 0.1 ? 0.1 : (_value > 1000 ? 1000 : _value));
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
		case 0: self->set_ratio2(value); break;
		case 1: self->set_mix(value); break;
		case 2: self->set_delay1(value); break;
		case 3: self->set_ratio1(value); break;
		case 4: self->set_cutoff(value); break;
		case 5: self->set_blur(value); break;
		case 6: self->set_delay2(value); break;
		
		default: break;
	}
}

/// Get the value of a parameter of a State object 

void getparameter(CommonState *cself, long index, double *value) {
	State *self = (State *)cself;
	switch (index) {
		case 0: *value = self->m_ratio_12; break;
		case 1: *value = self->m_mix_13; break;
		case 2: *value = self->m_delay_14; break;
		case 3: *value = self->m_ratio_15; break;
		case 4: *value = self->m_cutoff_16; break;
		case 5: *value = self->m_blur_17; break;
		case 6: *value = self->m_delay_18; break;
		
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
	// initialize parameter 0 ("m_ratio_12")
	pi = self->__commonstate.params + 0;
	pi->name = "ratio2";
	pi->paramtype = GENLIB_PARAMTYPE_FLOAT;
	pi->defaultvalue = self->m_ratio_12;
	pi->defaultref = 0;
	pi->hasinputminmax = false;
	pi->inputmin = 0; 
	pi->inputmax = 1;
	pi->hasminmax = true;
	pi->outputmin = 0.5;
	pi->outputmax = 2;
	pi->exp = 0;
	pi->units = "";		// no units defined
	// initialize parameter 1 ("m_mix_13")
	pi = self->__commonstate.params + 1;
	pi->name = "mix";
	pi->paramtype = GENLIB_PARAMTYPE_FLOAT;
	pi->defaultvalue = self->m_mix_13;
	pi->defaultref = 0;
	pi->hasinputminmax = false;
	pi->inputmin = 0; 
	pi->inputmax = 1;
	pi->hasminmax = true;
	pi->outputmin = 0;
	pi->outputmax = 1;
	pi->exp = 0;
	pi->units = "";		// no units defined
	// initialize parameter 2 ("m_delay_14")
	pi = self->__commonstate.params + 2;
	pi->name = "delay1";
	pi->paramtype = GENLIB_PARAMTYPE_FLOAT;
	pi->defaultvalue = self->m_delay_14;
	pi->defaultref = 0;
	pi->hasinputminmax = false;
	pi->inputmin = 0; 
	pi->inputmax = 1;
	pi->hasminmax = true;
	pi->outputmin = 0.1;
	pi->outputmax = 1000;
	pi->exp = 0;
	pi->units = "";		// no units defined
	// initialize parameter 3 ("m_ratio_15")
	pi = self->__commonstate.params + 3;
	pi->name = "ratio1";
	pi->paramtype = GENLIB_PARAMTYPE_FLOAT;
	pi->defaultvalue = self->m_ratio_15;
	pi->defaultref = 0;
	pi->hasinputminmax = false;
	pi->inputmin = 0; 
	pi->inputmax = 1;
	pi->hasminmax = true;
	pi->outputmin = 0.5;
	pi->outputmax = 2;
	pi->exp = 0;
	pi->units = "";		// no units defined
	// initialize parameter 4 ("m_cutoff_16")
	pi = self->__commonstate.params + 4;
	pi->name = "cutoff";
	pi->paramtype = GENLIB_PARAMTYPE_FLOAT;
	pi->defaultvalue = self->m_cutoff_16;
	pi->defaultref = 0;
	pi->hasinputminmax = false;
	pi->inputmin = 0; 
	pi->inputmax = 1;
	pi->hasminmax = true;
	pi->outputmin = 0;
	pi->outputmax = 3000;
	pi->exp = 0;
	pi->units = "";		// no units defined
	// initialize parameter 5 ("m_blur_17")
	pi = self->__commonstate.params + 5;
	pi->name = "blur";
	pi->paramtype = GENLIB_PARAMTYPE_FLOAT;
	pi->defaultvalue = self->m_blur_17;
	pi->defaultref = 0;
	pi->hasinputminmax = false;
	pi->inputmin = 0; 
	pi->inputmax = 1;
	pi->hasminmax = true;
	pi->outputmin = 0.01;
	pi->outputmax = 0.25;
	pi->exp = 0;
	pi->units = "";		// no units defined
	// initialize parameter 6 ("m_delay_18")
	pi = self->__commonstate.params + 6;
	pi->name = "delay2";
	pi->paramtype = GENLIB_PARAMTYPE_FLOAT;
	pi->defaultvalue = self->m_delay_18;
	pi->defaultref = 0;
	pi->hasinputminmax = false;
	pi->inputmin = 0; 
	pi->inputmax = 1;
	pi->hasminmax = true;
	pi->outputmin = 0.1;
	pi->outputmax = 1000;
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
