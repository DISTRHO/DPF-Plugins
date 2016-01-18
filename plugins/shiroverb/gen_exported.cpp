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
	Delay m_delay_22;
	Delay m_delay_14;
	Delay m_delay_15;
	Delay m_delay_13;
	Delay m_delay_12;
	Delay m_delay_23;
	Delay m_delay_16;
	Delay m_delay_18;
	Delay m_delay_17;
	Delay m_delay_20;
	Delay m_delay_19;
	Delay m_delay_21;
	Delay m_delay_24;
	Delta m_delta_34;
	Delta m_delta_43;
	Delta m_delta_37;
	Delta m_delta_40;
	Phasor m_phasor_33;
	Sah m_sah_42;
	Sah m_sah_41;
	Sah m_sah_39;
	Sah m_sah_36;
	Sah m_sah_38;
	Sah m_sah_35;
	Sah m_sah_44;
	Sah m_sah_45;
	double m_history_10;
	double m_history_5;
	double m_history_6;
	double m_history_7;
	double m_smth_11;
	double m_history_4;
	double m_y_2;
	double m_history_3;
	double samplerate;
	double m_history_1;
	double m_history_9;
	double m_history_8;
	double m_tone_32;
	double samples_to_seconds;
	double m_shimmer_26;
	double m_roomsize_25;
	double m_damping_28;
	double m_decay_27;
	double m_bandwidth_30;
	double m_mix_31;
	double m_ratio_29;
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
		m_smth_11 = 0;
		m_delay_12.reset("m_delay_12", 5000);
		m_delay_13.reset("m_delay_13", 7000);
		m_delay_14.reset("m_delay_14", 6000);
		m_delay_15.reset("m_delay_15", 48000);
		m_delay_16.reset("m_delay_16", 15000);
		m_delay_17.reset("m_delay_17", 16000);
		m_delay_18.reset("m_delay_18", 96000);
		m_delay_19.reset("m_delay_19", 10000);
		m_delay_20.reset("m_delay_20", 12000);
		m_delay_21.reset("m_delay_21", 48000);
		m_delay_22.reset("m_delay_22", 48000);
		m_delay_23.reset("m_delay_23", 48000);
		m_delay_24.reset("m_delay_24", 48000);
		m_roomsize_25 = 150;
		m_shimmer_26 = 50;
		m_decay_27 = 50;
		m_damping_28 = 50;
		m_ratio_29 = 2;
		m_bandwidth_30 = 75;
		m_mix_31 = 50;
		m_tone_32 = 1500;
		samples_to_seconds = (1 / samplerate);
		m_phasor_33.reset(0);
		m_delta_34.reset(0);
		m_sah_35.reset(0);
		m_sah_36.reset(0);
		m_delta_37.reset(0);
		m_sah_38.reset(0);
		m_sah_39.reset(0);
		m_delta_40.reset(0);
		m_sah_41.reset(0);
		m_sah_42.reset(0);
		m_delta_43.reset(0);
		m_sah_44.reset(0);
		m_sah_45.reset(0);
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
		double mul_11 = (m_damping_28 * 0.01);
		double expr_239 = safediv((m_roomsize_25 * 48000), 340);
		double mul_12 = (m_mix_31 * 0.01);
		double mul_10 = (m_bandwidth_30 * 0.01);
		double rsub_9 = (1 - mul_10);
		double expr_229 = safediv(((m_tone_32 * 2) * 3.1415926535898), 48000);
		double sin_17 = sin(expr_229);
		double clamp_18 = ((sin_17 <= 1e-05) ? 1e-05 : ((sin_17 >= 0.99999) ? 0.99999 : sin_17));
		double mul_13 = (m_shimmer_26 * 0.01);
		double expr_228 = safepow(1.0418, m_decay_27);
		double expr_238 = safepow(0.001, safediv(1, (expr_228 * 48000)));
		double rsub_68 = (1 - m_ratio_29);
		double mul_67 = (rsub_68 * 10);
		samples_to_seconds = (1 / samplerate);
		double mul_7 = (m_decay_27 * 2);
		double clamp_2 = ((mul_7 <= 0) ? 0 : ((mul_7 >= 100) ? 100 : mul_7));
		double mul_6 = (clamp_2 * 0.01);
		double atodb_5 = atodb(mul_6);
		double sub_4 = (atodb_5 - 9);
		double dbtoa_3 = dbtoa(sub_4);
		// the main sample loop;
		while ((__n--)) { 
			const double in1 = (*(__in1++));
			double mix_251 = (expr_239 + (0.999 * (m_smth_11 - expr_239)));
			double mix_1 = mix_251;
			double mul_216 = (mix_1 * 0.63245);
			double tap_130 = m_delay_24.read_linear(mul_216);
			double noise_20 = noise();
			double abs_37 = fabs(noise_20);
			double mul_43 = (abs_37 * 0.25);
			double noise_21 = noise();
			double abs_38 = fabs(noise_21);
			double mul_46 = (abs_38 * 0.25);
			double mul_219 = (mix_1 * 1);
			double tap_224 = m_delay_23.read_linear(mul_219);
			double noise_22 = noise();
			double abs_39 = fabs(noise_22);
			double mul_49 = (abs_39 * 0.25);
			double mul_217 = (mix_1 * 0.7071);
			double tap_136 = m_delay_22.read_linear(mul_217);
			double noise_19 = noise();
			double abs_36 = fabs(noise_19);
			double mul_40 = (abs_36 * 0.25);
			double mul_218 = (mix_1 * 0.81649);
			double tap_142 = m_delay_21.read_linear(mul_218);
			double expr_230 = (-safepow(expr_238, mul_216));
			double mul_126 = (tap_130 * expr_230);
			double mix_252 = (mul_126 + (mul_11 * (m_history_10 - mul_126)));
			double mix_128 = mix_252;
			double expr_237 = (-safepow(expr_238, mul_219));
			double mul_215 = (tap_224 * expr_237);
			double mix_253 = (mul_215 + (mul_11 * (m_history_9 - mul_215)));
			double mix_222 = mix_253;
			double expr_231 = (-safepow(expr_238, mul_217));
			double mul_132 = (tap_136 * expr_231);
			double mix_254 = (mul_132 + (mul_11 * (m_history_8 - mul_132)));
			double mix_134 = mix_254;
			double expr_232 = (-safepow(expr_238, mul_218));
			double mul_138 = (tap_142 * expr_232);
			double mix_255 = (mul_138 + (mul_11 * (m_history_7 - mul_138)));
			double mix_140 = mix_255;
			double mul_213 = (mix_1 * 0.000527);
			int int_212 = int(mul_213);
			double mul_169 = (int_212 * 419.5);
			double tap_168 = m_delay_20.read_linear(mul_169);
			double mul_166 = (tap_168 * 0.625);
			double mul_190 = (int_212 * 400.600006);
			double tap_189 = m_delay_19.read_linear(mul_190);
			double mul_187 = (tap_189 * 0.625);
			double add_120 = (mix_222 + mix_140);
			double add_118 = (mix_134 + mix_128);
			double add_113 = (add_120 + add_118);
			double mul_96 = (add_113 * 0.5);
			double sub_117 = (add_120 - add_118);
			double mul_99 = (sub_117 * 0.5);
			double add_156 = (mix_1 + 5);
			double expr_233 = safepow(expr_238, add_156);
			double phasor_86 = m_phasor_33(mul_67, samples_to_seconds);
			double add_66 = ((m_history_6 + phasor_86) + 0.5);
			double mod_65 = safemod(add_66, 1);
			double delta_45 = m_delta_34(mod_65);
			double sah_25 = m_sah_35(mul_43, delta_45, 0);
			int sah_44 = m_sah_36(4800, delta_45, 0);
			double mul_33 = (sah_44 * mod_65);
			double sub_64 = (mod_65 - 0.5);
			double mul_63 = (sub_64 * 3.1415926535898);
			double cos_62 = cos(mul_63);
			double mul_52 = (cos_62 * cos_62);
			double add_80 = ((m_history_5 + phasor_86) + 0.25);
			double mod_79 = safemod(add_80, 1);
			double delta_48 = m_delta_37(mod_79);
			double sah_27 = m_sah_38(mul_46, delta_48, 0);
			int sah_47 = m_sah_39(4800, delta_48, 0);
			double mul_34 = (sah_47 * mod_79);
			double sub_78 = (mod_79 - 0.5);
			double mul_77 = (sub_78 * 3.1415926535898);
			double cos_76 = cos(mul_77);
			double mul_53 = (cos_76 * cos_76);
			double add_85 = ((m_history_4 + phasor_86) + 0);
			double mod_84 = safemod(add_85, 1);
			double delta_30 = m_delta_40(mod_84);
			double sah_29 = m_sah_41(mul_49, delta_30, 0);
			int sah_50 = m_sah_42(4800, delta_30, 0);
			double mul_35 = (sah_50 * mod_84);
			double sub_83 = (mod_84 - 0.5);
			double mul_82 = (sub_83 * 3.1415926535898);
			double cos_81 = cos(mul_82);
			double mul_54 = (cos_81 * cos_81);
			double add_60 = ((m_history_3 + phasor_86) + 0.75);
			double mod_59 = safemod(add_60, 1);
			double delta_42 = m_delta_43(mod_59);
			double sah_23 = m_sah_44(mul_40, delta_42, 0);
			int sah_41 = m_sah_45(4800, delta_42, 0);
			double mul_32 = (sah_41 * mod_59);
			double tap_72 = m_delay_18.read_linear(mul_35);
			double tap_73 = m_delay_18.read_linear(mul_34);
			double tap_74 = m_delay_18.read_linear(mul_33);
			double tap_75 = m_delay_18.read_linear(mul_32);
			double mul_61 = (tap_74 * mul_52);
			double mul_69 = (tap_73 * mul_53);
			double mul_70 = (tap_72 * mul_54);
			double sub_58 = (mod_59 - 0.5);
			double mul_57 = (sub_58 * 3.1415926535898);
			double cos_56 = cos(mul_57);
			double mul_51 = (cos_56 * cos_56);
			double mul_55 = (tap_75 * mul_51);
			double add_257 = (((mul_70 + mul_69) + mul_61) + mul_55);
			double mix_256 = (m_y_2 + (clamp_18 * (add_257 - m_y_2)));
			double mix_15 = mix_256;
			double mix_258 = (in1 + (mul_13 * (mix_15 - in1)));
			double mix_87 = mix_258;
			double mul_90 = (mix_87 * 0.707);
			double mix_259 = (mul_90 + (rsub_9 * (m_history_1 - mul_90)));
			double mix_226 = mix_259;
			double mul_176 = (int_212 * 566.700012);
			double tap_175 = m_delay_17.read_linear(mul_176);
			double mul_197 = (int_212 * 568.299988);
			double tap_196 = m_delay_16.read_linear(mul_197);
			double sub_119 = (mix_222 - mix_140);
			double sub_116 = (mix_134 - mix_128);
			double sub_115 = (sub_119 - sub_116);
			double mul_98 = (sub_115 * 0.5);
			double add_114 = (sub_119 + sub_116);
			double rsub_112 = (0 - add_114);
			double mul_97 = (rsub_112 * 0.5);
			double mul_162 = (mix_1 * 0.41);
			double add_159 = (mul_162 + 5);
			double expr_236 = safepow(expr_238, add_159);
			double mul_161 = (mix_1 * 0.3);
			double add_158 = (mul_161 + 5);
			double expr_235 = safepow(expr_238, add_158);
			double mul_160 = (mix_1 * 0.155);
			double add_157 = (mul_160 + 5);
			double tap_144 = m_delay_15.read_linear(add_159);
			double tap_145 = m_delay_15.read_linear(add_158);
			double tap_146 = m_delay_15.read_linear(add_157);
			double tap_147 = m_delay_15.read_linear(add_156);
			double mul_148 = (tap_147 * expr_233);
			double add_121 = (mul_96 + mul_148);
			double mul_154 = (tap_144 * expr_236);
			double add_124 = (mul_99 + mul_154);
			double mul_152 = (tap_145 * expr_235);
			double add_123 = (mul_98 + mul_152);
			double expr_234 = safepow(expr_238, add_157);
			double mul_150 = (tap_146 * expr_234);
			double add_122 = (mul_97 + mul_150);
			double mul_173 = (tap_175 * 0.625);
			double mul_194 = (tap_196 * 0.625);
			double mul_211 = (mix_1 * 0.110732);
			double tap_210 = m_delay_14.read_cubic(mul_211);
			double mul_208 = (tap_210 * 0.75);
			double sub_207 = (mix_226 - mul_208);
			double mul_206 = (sub_207 * 0.75);
			double add_205 = (mul_206 + tap_210);
			double mul_183 = (int_212 * 144.800003);
			double tap_182 = m_delay_13.read_linear(mul_183);
			double mul_204 = (int_212 * 162.100006);
			double tap_203 = m_delay_12.read_linear(mul_204);
			double mul_180 = (tap_182 * 0.75);
			double mul_201 = (tap_203 * 0.75);
			double mul_107 = (mul_154 * mul_6);
			double mul_105 = (mul_150 * mul_6);
			double add_93 = (mul_107 + mul_105);
			double mul_106 = (mul_152 * mul_6);
			double mul_104 = (mul_148 * mul_6);
			double add_92 = (mul_106 + mul_104);
			double sub_102 = (add_93 - add_92);
			double mul_111 = (mul_99 * dbtoa_3);
			double mul_109 = (mul_97 * dbtoa_3);
			double add_95 = (mul_111 + mul_109);
			double mul_110 = (mul_98 * dbtoa_3);
			double mul_108 = (mul_96 * dbtoa_3);
			double add_94 = (mul_110 + mul_108);
			double sub_103 = (add_95 - add_94);
			double add_89 = (sub_103 + sub_102);
			double add_91 = (add_89 + mix_87);
			double sub_179 = (add_91 - mul_180);
			double mul_178 = (sub_179 * 0.75);
			double add_177 = (mul_178 + tap_182);
			double sub_172 = (add_177 - mul_173);
			double mul_171 = (sub_172 * 0.625);
			double add_170 = (mul_171 + tap_175);
			double sub_165 = (add_170 - mul_166);
			double mul_164 = (sub_165 * 0.625);
			double add_163 = (mul_164 + tap_168);
			double mul_88 = (add_163 * mul_12);
			double out2 = (in1 + mul_88);
			double add_101 = (add_89 + mix_87);
			double sub_200 = (add_101 - mul_201);
			double mul_199 = (sub_200 * 0.75);
			double add_198 = (mul_199 + tap_203);
			double sub_193 = (add_198 - mul_194);
			double mul_192 = (sub_193 * 0.625);
			double add_191 = (mul_192 + tap_196);
			double sub_186 = (add_191 - mul_187);
			double mul_185 = (sub_186 * 0.625);
			double add_184 = (mul_185 + tap_189);
			double mul_100 = (add_184 * mul_12);
			double out1 = (in1 + mul_100);
			double smth_next_240 = mix_1;
			double history_127_next_241 = mix_128;
			double history_221_next_242 = mix_222;
			double history_133_next_243 = mix_134;
			double history_139_next_244 = mix_140;
			double history_26_next_245 = sah_25;
			double history_28_next_246 = sah_27;
			double history_31_next_247 = sah_29;
			double history_24_next_248 = sah_23;
			double y0_next_249 = mix_15;
			double history_225_next_250 = mix_226;
			m_delay_24.write(add_121);
			m_delay_23.write(add_124);
			m_delay_22.write(add_122);
			m_delay_21.write(add_123);
			m_delay_20.write(sub_165);
			m_delay_19.write(sub_186);
			m_delay_18.write(in1);
			m_delay_17.write(sub_172);
			m_delay_16.write(sub_193);
			m_delay_15.write(add_205);
			m_delay_14.write(sub_207);
			m_delay_13.write(sub_179);
			m_delay_12.write(sub_200);
			m_smth_11 = smth_next_240;
			m_history_10 = history_127_next_241;
			m_history_9 = history_221_next_242;
			m_history_8 = history_133_next_243;
			m_history_7 = history_139_next_244;
			m_history_6 = history_26_next_245;
			m_history_5 = history_28_next_246;
			m_history_4 = history_31_next_247;
			m_history_3 = history_24_next_248;
			m_y_2 = y0_next_249;
			m_history_1 = history_225_next_250;
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
			m_delay_24.step();
			// assign results to output buffer;
			(*(__out1++)) = out1;
			(*(__out2++)) = out2;
			
		};
		return __exception;
		
	};
	inline void set_roomsize(double _value) {
		m_roomsize_25 = (_value < 1 ? 1 : (_value > 300 ? 300 : _value));
	};
	inline void set_shimmer(double _value) {
		m_shimmer_26 = (_value < 0 ? 0 : (_value > 100 ? 100 : _value));
	};
	inline void set_decay(double _value) {
		m_decay_27 = (_value < 1 ? 1 : (_value > 100 ? 100 : _value));
	};
	inline void set_damping(double _value) {
		m_damping_28 = (_value < 0 ? 0 : (_value > 100 ? 100 : _value));
	};
	inline void set_ratio(double _value) {
		m_ratio_29 = (_value < 0.5 ? 0.5 : (_value > 2 ? 2 : _value));
	};
	inline void set_bandwidth(double _value) {
		m_bandwidth_30 = (_value < 0 ? 0 : (_value > 100 ? 100 : _value));
	};
	inline void set_mix(double _value) {
		m_mix_31 = (_value < 0 ? 0 : (_value > 100 ? 100 : _value));
	};
	inline void set_tone(double _value) {
		m_tone_32 = (_value < 500 ? 500 : (_value > 6000 ? 6000 : _value));
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
		case 0: self->set_roomsize(value); break;
		case 1: self->set_shimmer(value); break;
		case 2: self->set_decay(value); break;
		case 3: self->set_damping(value); break;
		case 4: self->set_ratio(value); break;
		case 5: self->set_bandwidth(value); break;
		case 6: self->set_mix(value); break;
		case 7: self->set_tone(value); break;
		
		default: break;
	}
}

/// Get the value of a parameter of a State object 

void getparameter(CommonState *cself, long index, double *value) {
	State *self = (State *)cself;
	switch (index) {
		case 0: *value = self->m_roomsize_25; break;
		case 1: *value = self->m_shimmer_26; break;
		case 2: *value = self->m_decay_27; break;
		case 3: *value = self->m_damping_28; break;
		case 4: *value = self->m_ratio_29; break;
		case 5: *value = self->m_bandwidth_30; break;
		case 6: *value = self->m_mix_31; break;
		case 7: *value = self->m_tone_32; break;
		
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
	// initialize parameter 0 ("m_roomsize_25")
	pi = self->__commonstate.params + 0;
	pi->name = "roomsize";
	pi->paramtype = GENLIB_PARAMTYPE_FLOAT;
	pi->defaultvalue = self->m_roomsize_25;
	pi->defaultref = 0;
	pi->hasinputminmax = false;
	pi->inputmin = 0; 
	pi->inputmax = 1;
	pi->hasminmax = true;
	pi->outputmin = 1;
	pi->outputmax = 300;
	pi->exp = 0;
	pi->units = "";		// no units defined
	// initialize parameter 1 ("m_shimmer_26")
	pi = self->__commonstate.params + 1;
	pi->name = "shimmer";
	pi->paramtype = GENLIB_PARAMTYPE_FLOAT;
	pi->defaultvalue = self->m_shimmer_26;
	pi->defaultref = 0;
	pi->hasinputminmax = false;
	pi->inputmin = 0; 
	pi->inputmax = 1;
	pi->hasminmax = true;
	pi->outputmin = 0;
	pi->outputmax = 100;
	pi->exp = 0;
	pi->units = "";		// no units defined
	// initialize parameter 2 ("m_decay_27")
	pi = self->__commonstate.params + 2;
	pi->name = "decay";
	pi->paramtype = GENLIB_PARAMTYPE_FLOAT;
	pi->defaultvalue = self->m_decay_27;
	pi->defaultref = 0;
	pi->hasinputminmax = false;
	pi->inputmin = 0; 
	pi->inputmax = 1;
	pi->hasminmax = true;
	pi->outputmin = 1;
	pi->outputmax = 100;
	pi->exp = 0;
	pi->units = "";		// no units defined
	// initialize parameter 3 ("m_damping_28")
	pi = self->__commonstate.params + 3;
	pi->name = "damping";
	pi->paramtype = GENLIB_PARAMTYPE_FLOAT;
	pi->defaultvalue = self->m_damping_28;
	pi->defaultref = 0;
	pi->hasinputminmax = false;
	pi->inputmin = 0; 
	pi->inputmax = 1;
	pi->hasminmax = true;
	pi->outputmin = 0;
	pi->outputmax = 100;
	pi->exp = 0;
	pi->units = "";		// no units defined
	// initialize parameter 4 ("m_ratio_29")
	pi = self->__commonstate.params + 4;
	pi->name = "ratio";
	pi->paramtype = GENLIB_PARAMTYPE_FLOAT;
	pi->defaultvalue = self->m_ratio_29;
	pi->defaultref = 0;
	pi->hasinputminmax = false;
	pi->inputmin = 0; 
	pi->inputmax = 1;
	pi->hasminmax = true;
	pi->outputmin = 0.5;
	pi->outputmax = 2;
	pi->exp = 0;
	pi->units = "";		// no units defined
	// initialize parameter 5 ("m_bandwidth_30")
	pi = self->__commonstate.params + 5;
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
	// initialize parameter 6 ("m_mix_31")
	pi = self->__commonstate.params + 6;
	pi->name = "mix";
	pi->paramtype = GENLIB_PARAMTYPE_FLOAT;
	pi->defaultvalue = self->m_mix_31;
	pi->defaultref = 0;
	pi->hasinputminmax = false;
	pi->inputmin = 0; 
	pi->inputmax = 1;
	pi->hasminmax = true;
	pi->outputmin = 0;
	pi->outputmax = 100;
	pi->exp = 0;
	pi->units = "";		// no units defined
	// initialize parameter 7 ("m_tone_32")
	pi = self->__commonstate.params + 7;
	pi->name = "tone";
	pi->paramtype = GENLIB_PARAMTYPE_FLOAT;
	pi->defaultvalue = self->m_tone_32;
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
