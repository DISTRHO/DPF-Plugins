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
	Delay m_delay_21;
	Delay m_delay_13;
	Delay m_delay_14;
	Delay m_delay_12;
	Delay m_delay_11;
	Delay m_delay_23;
	Delay m_delay_15;
	Delay m_delay_17;
	Delay m_delay_16;
	Delay m_delay_19;
	Delay m_delay_18;
	Delay m_delay_20;
	Delay m_delay_22;
	Delta m_delta_39;
	Delta m_delta_36;
	Delta m_delta_42;
	Delta m_delta_33;
	Phasor m_phasor_32;
	Sah m_sah_37;
	Sah m_sah_35;
	Sah m_sah_38;
	Sah m_sah_41;
	Sah m_sah_40;
	Sah m_sah_34;
	Sah m_sah_44;
	Sah m_sah_43;
	double m_history_5;
	double m_history_6;
	double m_history_7;
	double m_history_10;
	double m_history_4;
	double m_y_2;
	double m_history_3;
	double samplerate;
	double m_history_1;
	double m_history_9;
	double m_history_8;
	double m_tone_31;
	double samples_to_seconds;
	double m_decay_25;
	double m_shimmer_24;
	double m_mix_27;
	double m_damping_26;
	double m_roomsize_29;
	double m_bandwidth_30;
	double m_ratio_28;
	int vectorsize;
	int __exception;
	// re-initialize all member variables;
	inline void reset(double __sr, int __vs) { 
		__exception = 0;
		vectorsize = __vs;
		samplerate = __sr;
		m_history_1 = 0;
		m_y_2 = 0;
		m_history_3 = 0;
		m_history_4 = 0;
		m_history_5 = 0;
		m_history_6 = 0;
		m_history_7 = 0;
		m_history_8 = 0;
		m_history_9 = 0;
		m_history_10 = 0;
		m_delay_11.reset("m_delay_11", 5000);
		m_delay_12.reset("m_delay_12", 7000);
		m_delay_13.reset("m_delay_13", 6000);
		m_delay_14.reset("m_delay_14", 48000);
		m_delay_15.reset("m_delay_15", 15000);
		m_delay_16.reset("m_delay_16", 16000);
		m_delay_17.reset("m_delay_17", 96000);
		m_delay_18.reset("m_delay_18", 10000);
		m_delay_19.reset("m_delay_19", 12000);
		m_delay_20.reset("m_delay_20", 48000);
		m_delay_21.reset("m_delay_21", 48000);
		m_delay_22.reset("m_delay_22", 48000);
		m_delay_23.reset("m_delay_23", 48000);
		m_shimmer_24 = 50;
		m_decay_25 = 50;
		m_damping_26 = 50;
		m_mix_27 = 50;
		m_ratio_28 = 2;
		m_roomsize_29 = 150;
		m_bandwidth_30 = 75;
		m_tone_31 = 1500;
		samples_to_seconds = (1 / samplerate);
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
		t_sample * __out2 = __outs[1];
		if (__exception) { 
			return __exception;
			
		} else if (( (__in1 == 0) || (__out1 == 0) || (__out2 == 0) )) { 
			__exception = GENLIB_ERR_NULL_BUFFER;
			return __exception;
			
		};
		double mul_10 = (m_damping_26 * 0.01);
		double mul_11 = (m_mix_27 * 0.01);
		double mul_9 = (m_bandwidth_30 * 0.01);
		double rsub_8 = (1 - mul_9);
		double expr_228 = safediv(((m_tone_31 * 2) * 3.1415926535898), 48000);
		double sin_16 = sin(expr_228);
		double clamp_17 = ((sin_16 <= 1e-05) ? 1e-05 : ((sin_16 >= 0.99999) ? 0.99999 : sin_16));
		double mul_12 = (m_shimmer_24 * 0.01);
		double expr_238 = safediv((m_roomsize_29 * 48000), 340);
		double mul_215 = (expr_238 * 0.63245);
		double mul_216 = (expr_238 * 0.7071);
		double mul_218 = (expr_238 * 1);
		double mul_217 = (expr_238 * 0.81649);
		double expr_227 = safepow(1.0418, m_decay_25);
		double expr_237 = safepow(0.001, safediv(1, (expr_227 * 48000)));
		double expr_229 = (-safepow(expr_237, mul_215));
		double expr_230 = (-safepow(expr_237, mul_216));
		double expr_236 = (-safepow(expr_237, mul_218));
		double expr_231 = (-safepow(expr_237, mul_217));
		double mul_212 = (expr_238 * 0.000527);
		int int_211 = int(mul_212);
		double mul_168 = (int_211 * 419.5);
		double mul_189 = (int_211 * 400.600006);
		double rsub_67 = (1 - m_ratio_28);
		double mul_66 = (rsub_67 * 10);
		samples_to_seconds = (1 / samplerate);
		double add_155 = (expr_238 + 5);
		double expr_232 = safepow(expr_237, add_155);
		double mul_175 = (int_211 * 566.700012);
		double mul_196 = (int_211 * 568.299988);
		double mul_161 = (expr_238 * 0.41);
		double add_158 = (mul_161 + 5);
		double expr_235 = safepow(expr_237, add_158);
		double mul_160 = (expr_238 * 0.3);
		double add_157 = (mul_160 + 5);
		double expr_234 = safepow(expr_237, add_157);
		double mul_159 = (expr_238 * 0.155);
		double add_156 = (mul_159 + 5);
		double expr_233 = safepow(expr_237, add_156);
		double mul_210 = (expr_238 * 0.110732);
		double mul_182 = (int_211 * 144.800003);
		double mul_203 = (int_211 * 162.100006);
		double mul_6 = (m_decay_25 * 2);
		double clamp_1 = ((mul_6 <= 0) ? 0 : ((mul_6 >= 100) ? 100 : mul_6));
		double mul_5 = (clamp_1 * 0.01);
		double atodb_4 = atodb(mul_5);
		double sub_3 = (atodb_4 - 9);
		double dbtoa_2 = dbtoa(sub_3);
		// the main sample loop;
		while ((__n--)) { 
			const double in1 = (*(__in1++));
			double noise_20 = noise();
			double abs_37 = fabs(noise_20);
			double mul_45 = (abs_37 * 0.25);
			double noise_19 = noise();
			double abs_36 = fabs(noise_19);
			double mul_42 = (abs_36 * 0.25);
			double noise_21 = noise();
			double abs_38 = fabs(noise_21);
			double mul_48 = (abs_38 * 0.25);
			double noise_18 = noise();
			double abs_35 = fabs(noise_18);
			double mul_39 = (abs_35 * 0.25);
			double tap_129 = m_delay_23.read_linear(mul_215);
			double tap_135 = m_delay_22.read_linear(mul_216);
			double tap_223 = m_delay_21.read_linear(mul_218);
			double tap_141 = m_delay_20.read_linear(mul_217);
			double mul_125 = (tap_129 * expr_229);
			double mix_249 = (mul_125 + (mul_10 * (m_history_10 - mul_125)));
			double mix_127 = mix_249;
			double mul_131 = (tap_135 * expr_230);
			double mix_250 = (mul_131 + (mul_10 * (m_history_9 - mul_131)));
			double mix_133 = mix_250;
			double mul_214 = (tap_223 * expr_236);
			double mix_251 = (mul_214 + (mul_10 * (m_history_8 - mul_214)));
			double mix_221 = mix_251;
			double mul_137 = (tap_141 * expr_231);
			double mix_252 = (mul_137 + (mul_10 * (m_history_7 - mul_137)));
			double mix_139 = mix_252;
			double tap_167 = m_delay_19.read_linear(mul_168);
			double mul_165 = (tap_167 * 0.625);
			double tap_188 = m_delay_18.read_linear(mul_189);
			double mul_186 = (tap_188 * 0.625);
			double phasor_85 = m_phasor_32(mul_66, samples_to_seconds);
			double add_79 = ((m_history_6 + phasor_85) + 0.25);
			double mod_78 = safemod(add_79, 1);
			double delta_47 = m_delta_33(mod_78);
			double sah_26 = m_sah_34(mul_45, delta_47, 0);
			int sah_46 = m_sah_35(4800, delta_47, 0);
			double mul_33 = (sah_46 * mod_78);
			double sub_77 = (mod_78 - 0.5);
			double mul_76 = (sub_77 * 3.1415926535898);
			double cos_75 = cos(mul_76);
			double mul_52 = (cos_75 * cos_75);
			double add_65 = ((m_history_5 + phasor_85) + 0.5);
			double mod_64 = safemod(add_65, 1);
			double delta_44 = m_delta_36(mod_64);
			double sah_24 = m_sah_37(mul_42, delta_44, 0);
			int sah_43 = m_sah_38(4800, delta_44, 0);
			double mul_32 = (sah_43 * mod_64);
			double sub_63 = (mod_64 - 0.5);
			double mul_62 = (sub_63 * 3.1415926535898);
			double cos_61 = cos(mul_62);
			double mul_51 = (cos_61 * cos_61);
			double add_84 = ((m_history_4 + phasor_85) + 0);
			double mod_83 = safemod(add_84, 1);
			double delta_29 = m_delta_39(mod_83);
			double sah_28 = m_sah_40(mul_48, delta_29, 0);
			int sah_49 = m_sah_41(4800, delta_29, 0);
			double mul_34 = (sah_49 * mod_83);
			double sub_82 = (mod_83 - 0.5);
			double mul_81 = (sub_82 * 3.1415926535898);
			double cos_80 = cos(mul_81);
			double mul_53 = (cos_80 * cos_80);
			double add_59 = ((m_history_3 + phasor_85) + 0.75);
			double mod_58 = safemod(add_59, 1);
			double delta_41 = m_delta_42(mod_58);
			double sah_22 = m_sah_43(mul_39, delta_41, 0);
			int sah_40 = m_sah_44(4800, delta_41, 0);
			double mul_31 = (sah_40 * mod_58);
			double tap_71 = m_delay_17.read_linear(mul_34);
			double tap_72 = m_delay_17.read_linear(mul_33);
			double tap_73 = m_delay_17.read_linear(mul_32);
			double tap_74 = m_delay_17.read_linear(mul_31);
			double mul_60 = (tap_73 * mul_51);
			double mul_68 = (tap_72 * mul_52);
			double mul_69 = (tap_71 * mul_53);
			double sub_57 = (mod_58 - 0.5);
			double mul_56 = (sub_57 * 3.1415926535898);
			double cos_55 = cos(mul_56);
			double mul_50 = (cos_55 * cos_55);
			double mul_54 = (tap_74 * mul_50);
			double add_254 = (((mul_69 + mul_68) + mul_60) + mul_54);
			double mix_253 = (m_y_2 + (clamp_17 * (add_254 - m_y_2)));
			double mix_14 = mix_253;
			double mix_255 = (in1 + (mul_12 * (mix_14 - in1)));
			double mix_86 = mix_255;
			double mul_89 = (mix_86 * 0.707);
			double mix_256 = (mul_89 + (rsub_8 * (m_history_1 - mul_89)));
			double mix_225 = mix_256;
			double add_119 = (mix_221 + mix_139);
			double add_117 = (mix_133 + mix_127);
			double add_112 = (add_119 + add_117);
			double mul_95 = (add_112 * 0.5);
			double sub_116 = (add_119 - add_117);
			double mul_98 = (sub_116 * 0.5);
			double tap_174 = m_delay_16.read_linear(mul_175);
			double tap_195 = m_delay_15.read_linear(mul_196);
			double sub_118 = (mix_221 - mix_139);
			double sub_115 = (mix_133 - mix_127);
			double sub_114 = (sub_118 - sub_115);
			double mul_97 = (sub_114 * 0.5);
			double add_113 = (sub_118 + sub_115);
			double rsub_111 = (0 - add_113);
			double mul_96 = (rsub_111 * 0.5);
			double tap_143 = m_delay_14.read_linear(add_158);
			double tap_144 = m_delay_14.read_linear(add_157);
			double tap_145 = m_delay_14.read_linear(add_156);
			double tap_146 = m_delay_14.read_linear(add_155);
			double mul_147 = (tap_146 * expr_232);
			double add_120 = (mul_95 + mul_147);
			double mul_153 = (tap_143 * expr_235);
			double add_123 = (mul_98 + mul_153);
			double mul_151 = (tap_144 * expr_234);
			double add_122 = (mul_97 + mul_151);
			double mul_149 = (tap_145 * expr_233);
			double add_121 = (mul_96 + mul_149);
			double mul_172 = (tap_174 * 0.625);
			double mul_193 = (tap_195 * 0.625);
			double tap_209 = m_delay_13.read_cubic(mul_210);
			double mul_207 = (tap_209 * 0.75);
			double sub_206 = (mix_225 - mul_207);
			double mul_205 = (sub_206 * 0.75);
			double add_204 = (mul_205 + tap_209);
			double tap_181 = m_delay_12.read_linear(mul_182);
			double tap_202 = m_delay_11.read_linear(mul_203);
			double mul_179 = (tap_181 * 0.75);
			double mul_200 = (tap_202 * 0.75);
			double mul_106 = (mul_153 * mul_5);
			double mul_104 = (mul_149 * mul_5);
			double add_92 = (mul_106 + mul_104);
			double mul_105 = (mul_151 * mul_5);
			double mul_103 = (mul_147 * mul_5);
			double add_91 = (mul_105 + mul_103);
			double sub_101 = (add_92 - add_91);
			double mul_110 = (mul_98 * dbtoa_2);
			double mul_108 = (mul_96 * dbtoa_2);
			double add_94 = (mul_110 + mul_108);
			double mul_109 = (mul_97 * dbtoa_2);
			double mul_107 = (mul_95 * dbtoa_2);
			double add_93 = (mul_109 + mul_107);
			double sub_102 = (add_94 - add_93);
			double add_88 = (sub_102 + sub_101);
			double add_90 = (add_88 + mix_86);
			double sub_178 = (add_90 - mul_179);
			double mul_177 = (sub_178 * 0.75);
			double add_176 = (mul_177 + tap_181);
			double sub_171 = (add_176 - mul_172);
			double mul_170 = (sub_171 * 0.625);
			double add_169 = (mul_170 + tap_174);
			double sub_164 = (add_169 - mul_165);
			double mul_163 = (sub_164 * 0.625);
			double add_162 = (mul_163 + tap_167);
			double mul_87 = (add_162 * mul_11);
			double out2 = (in1 + mul_87);
			double add_100 = (add_88 + mix_86);
			double sub_199 = (add_100 - mul_200);
			double mul_198 = (sub_199 * 0.75);
			double add_197 = (mul_198 + tap_202);
			double sub_192 = (add_197 - mul_193);
			double mul_191 = (sub_192 * 0.625);
			double add_190 = (mul_191 + tap_195);
			double sub_185 = (add_190 - mul_186);
			double mul_184 = (sub_185 * 0.625);
			double add_183 = (mul_184 + tap_188);
			double mul_99 = (add_183 * mul_11);
			double out1 = (in1 + mul_99);
			double history_126_next_239 = mix_127;
			double history_132_next_240 = mix_133;
			double history_220_next_241 = mix_221;
			double history_138_next_242 = mix_139;
			double history_27_next_243 = sah_26;
			double history_25_next_244 = sah_24;
			double history_30_next_245 = sah_28;
			double history_23_next_246 = sah_22;
			double y0_next_247 = mix_14;
			double history_224_next_248 = mix_225;
			m_delay_23.write(add_120);
			m_delay_22.write(add_121);
			m_delay_21.write(add_123);
			m_delay_20.write(add_122);
			m_delay_19.write(sub_164);
			m_delay_18.write(sub_185);
			m_delay_17.write(in1);
			m_delay_16.write(sub_171);
			m_delay_15.write(sub_192);
			m_delay_14.write(add_204);
			m_delay_13.write(sub_206);
			m_delay_12.write(sub_178);
			m_delay_11.write(sub_199);
			m_history_10 = history_126_next_239;
			m_history_9 = history_132_next_240;
			m_history_8 = history_220_next_241;
			m_history_7 = history_138_next_242;
			m_history_6 = history_27_next_243;
			m_history_5 = history_25_next_244;
			m_history_4 = history_30_next_245;
			m_history_3 = history_23_next_246;
			m_y_2 = y0_next_247;
			m_history_1 = history_224_next_248;
			m_delay_11.step();
			m_delay_12.step();
			m_delay_13.step();
			m_delay_14.step();
			m_delay_15.step();
			m_delay_16.step();
			m_delay_17.step();
			m_delay_18.step();
			m_delay_19.step();
			m_delay_20.step();
			m_delay_21.step();
			m_delay_22.step();
			m_delay_23.step();
			// assign results to output buffer;
			(*(__out1++)) = out1;
			(*(__out2++)) = out2;
			
		};
		return __exception;
		
	};
	inline void set_shimmer(double _value) {
		m_shimmer_24 = (_value < 0 ? 0 : (_value > 100 ? 100 : _value));
	};
	inline void set_decay(double _value) {
		m_decay_25 = (_value < 1 ? 1 : (_value > 100 ? 100 : _value));
	};
	inline void set_damping(double _value) {
		m_damping_26 = (_value < 0 ? 0 : (_value > 100 ? 100 : _value));
	};
	inline void set_mix(double _value) {
		m_mix_27 = (_value < 0 ? 0 : (_value > 100 ? 100 : _value));
	};
	inline void set_ratio(double _value) {
		m_ratio_28 = (_value < 0.5 ? 0.5 : (_value > 2 ? 2 : _value));
	};
	inline void set_roomsize(double _value) {
		m_roomsize_29 = (_value < 1 ? 1 : (_value > 300 ? 300 : _value));
	};
	inline void set_bandwidth(double _value) {
		m_bandwidth_30 = (_value < 0 ? 0 : (_value > 100 ? 100 : _value));
	};
	inline void set_tone(double _value) {
		m_tone_31 = (_value < 500 ? 500 : (_value > 6000 ? 6000 : _value));
	};
	
} State;


/// 
///	Configuration for the genlib API
///

/// Number of signal inputs and outputs 

int gen_kernel_numins = 1;
int gen_kernel_numouts = 2;

int num_inputs() { return gen_kernel_numins; }
int num_outputs() { return gen_kernel_numouts; }
int num_params() { return 8; }

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

void setparameter(CommonState *cself, long index, double value, void *ref) {
	State * self = (State *)cself;
	switch (index) {
		case 0: self->set_shimmer(value); break;
		case 1: self->set_decay(value); break;
		case 2: self->set_damping(value); break;
		case 3: self->set_mix(value); break;
		case 4: self->set_ratio(value); break;
		case 5: self->set_roomsize(value); break;
		case 6: self->set_bandwidth(value); break;
		case 7: self->set_tone(value); break;
		
		default: break;
	}
}

/// Get the value of a parameter of a State object 

void getparameter(CommonState *cself, long index, double *value) {
	State *self = (State *)cself;
	switch (index) {
		case 0: *value = self->m_shimmer_24; break;
		case 1: *value = self->m_decay_25; break;
		case 2: *value = self->m_damping_26; break;
		case 3: *value = self->m_mix_27; break;
		case 4: *value = self->m_ratio_28; break;
		case 5: *value = self->m_roomsize_29; break;
		case 6: *value = self->m_bandwidth_30; break;
		case 7: *value = self->m_tone_31; break;
		
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
	self->__commonstate.params = (ParamInfo *)genlib_sysmem_newptr(8 * sizeof(ParamInfo));
	self->__commonstate.numparams = 8;
	// initialize parameter 0 ("m_shimmer_24")
	pi = self->__commonstate.params + 0;
	pi->name = "shimmer";
	pi->paramtype = GENLIB_PARAMTYPE_FLOAT;
	pi->defaultvalue = self->m_shimmer_24;
	pi->defaultref = 0;
	pi->hasinputminmax = false;
	pi->inputmin = 0; 
	pi->inputmax = 1;
	pi->hasminmax = true;
	pi->outputmin = 0;
	pi->outputmax = 100;
	pi->exp = 0;
	pi->units = "";		// no units defined
	// initialize parameter 1 ("m_decay_25")
	pi = self->__commonstate.params + 1;
	pi->name = "decay";
	pi->paramtype = GENLIB_PARAMTYPE_FLOAT;
	pi->defaultvalue = self->m_decay_25;
	pi->defaultref = 0;
	pi->hasinputminmax = false;
	pi->inputmin = 0; 
	pi->inputmax = 1;
	pi->hasminmax = true;
	pi->outputmin = 1;
	pi->outputmax = 100;
	pi->exp = 0;
	pi->units = "";		// no units defined
	// initialize parameter 2 ("m_damping_26")
	pi = self->__commonstate.params + 2;
	pi->name = "damping";
	pi->paramtype = GENLIB_PARAMTYPE_FLOAT;
	pi->defaultvalue = self->m_damping_26;
	pi->defaultref = 0;
	pi->hasinputminmax = false;
	pi->inputmin = 0; 
	pi->inputmax = 1;
	pi->hasminmax = true;
	pi->outputmin = 0;
	pi->outputmax = 100;
	pi->exp = 0;
	pi->units = "";		// no units defined
	// initialize parameter 3 ("m_mix_27")
	pi = self->__commonstate.params + 3;
	pi->name = "mix";
	pi->paramtype = GENLIB_PARAMTYPE_FLOAT;
	pi->defaultvalue = self->m_mix_27;
	pi->defaultref = 0;
	pi->hasinputminmax = false;
	pi->inputmin = 0; 
	pi->inputmax = 1;
	pi->hasminmax = true;
	pi->outputmin = 0;
	pi->outputmax = 100;
	pi->exp = 0;
	pi->units = "";		// no units defined
	// initialize parameter 4 ("m_ratio_28")
	pi = self->__commonstate.params + 4;
	pi->name = "ratio";
	pi->paramtype = GENLIB_PARAMTYPE_FLOAT;
	pi->defaultvalue = self->m_ratio_28;
	pi->defaultref = 0;
	pi->hasinputminmax = false;
	pi->inputmin = 0; 
	pi->inputmax = 1;
	pi->hasminmax = true;
	pi->outputmin = 0.5;
	pi->outputmax = 2;
	pi->exp = 0;
	pi->units = "";		// no units defined
	// initialize parameter 5 ("m_roomsize_29")
	pi = self->__commonstate.params + 5;
	pi->name = "roomsize";
	pi->paramtype = GENLIB_PARAMTYPE_FLOAT;
	pi->defaultvalue = self->m_roomsize_29;
	pi->defaultref = 0;
	pi->hasinputminmax = false;
	pi->inputmin = 0; 
	pi->inputmax = 1;
	pi->hasminmax = true;
	pi->outputmin = 1;
	pi->outputmax = 300;
	pi->exp = 0;
	pi->units = "";		// no units defined
	// initialize parameter 6 ("m_bandwidth_30")
	pi = self->__commonstate.params + 6;
	pi->name = "bandwidth";
	pi->paramtype = GENLIB_PARAMTYPE_FLOAT;
	pi->defaultvalue = self->m_bandwidth_30;
	pi->defaultref = 0;
	pi->hasinputminmax = false;
	pi->inputmin = 0; 
	pi->inputmax = 1;
	pi->hasminmax = true;
	pi->outputmin = 0;
	pi->outputmax = 100;
	pi->exp = 0;
	pi->units = "";		// no units defined
	// initialize parameter 7 ("m_tone_31")
	pi = self->__commonstate.params + 7;
	pi->name = "tone";
	pi->paramtype = GENLIB_PARAMTYPE_FLOAT;
	pi->defaultvalue = self->m_tone_31;
	pi->defaultref = 0;
	pi->hasinputminmax = false;
	pi->inputmin = 0; 
	pi->inputmax = 1;
	pi->hasminmax = true;
	pi->outputmin = 500;
	pi->outputmax = 6000;
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
