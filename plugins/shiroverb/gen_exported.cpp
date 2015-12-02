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
	Delay m_delay_14;
	Delay m_delay_11;
	Delay m_delay_18;
	Delay m_delay_15;
	Delay m_delay_12;
	Delay m_delay_17;
	Delay m_delay_13;
	Delay m_delay_20;
	Delay m_delay_19;
	Delay m_delay_21;
	Delay m_delay_23;
	Delay m_delay_22;
	Delay m_delay_16;
	Delta m_delta_41;
	Delta m_delta_47;
	Delta m_delta_44;
	Delta m_delta_38;
	Phasor m_phasor_37;
	Sah m_sah_48;
	Sah m_sah_49;
	Sah m_sah_46;
	Sah m_sah_45;
	Sah m_sah_42;
	Sah m_sah_43;
	Sah m_sah_40;
	Sah m_sah_39;
	double m_cutoff_34;
	double m_tail_33;
	double m_early_35;
	double samples_to_seconds;
	double m_tone_32;
	double m_mix_36;
	double m_spread_31;
	double m_ratio_24;
	double m_roomsize_29;
	double m_history_4;
	double m_history_5;
	double m_damping_30;
	double m_history_3;
	double m_history_1;
	double m_y_2;
	double samplerate;
	double m_history_7;
	double m_history_6;
	double m_history_9;
	double m_history_8;
	double m_blur_28;
	double m_decay_26;
	double m_shimmer_27;
	double m_history_10;
	double m_window_25;
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
		m_delay_13.reset("m_delay_13", 15000);
		m_delay_14.reset("m_delay_14", 16000);
		m_delay_15.reset("m_delay_15", 6000);
		m_delay_16.reset("m_delay_16", 96000);
		m_delay_17.reset("m_delay_17", 48000);
		m_delay_18.reset("m_delay_18", 10000);
		m_delay_19.reset("m_delay_19", 12000);
		m_delay_20.reset("m_delay_20", 48000);
		m_delay_21.reset("m_delay_21", 48000);
		m_delay_22.reset("m_delay_22", 48000);
		m_delay_23.reset("m_delay_23", 48000);
		m_ratio_24 = 2;
		m_window_25 = 100;
		m_decay_26 = 180;
		m_shimmer_27 = 0.5;
		m_blur_28 = 0.25;
		m_roomsize_29 = 150;
		m_damping_30 = 0.75;
		m_spread_31 = 25;
		m_tone_32 = 0.5;
		m_tail_33 = 0.25;
		m_cutoff_34 = 3000;
		m_early_35 = 0.25;
		m_mix_36 = 0.5;
		samples_to_seconds = (1 / samplerate);
		m_phasor_37.reset(0);
		m_delta_38.reset(0);
		m_sah_39.reset(0);
		m_sah_40.reset(0);
		m_delta_41.reset(0);
		m_sah_42.reset(0);
		m_sah_43.reset(0);
		m_delta_44.reset(0);
		m_sah_45.reset(0);
		m_sah_46.reset(0);
		m_delta_47.reset(0);
		m_sah_48.reset(0);
		m_sah_49.reset(0);
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
		double rsub_5405 = (1 - m_tone_32);
		double expr_5416 = safepow(0.001, safediv(1, (m_decay_26 * 44100)));
		double expr_5407 = safediv(((m_cutoff_34 * 2) * 3.1415926535898), 44100);
		double sin_5179 = sin(expr_5407);
		double clamp_5180 = ((sin_5179 <= 1e-05) ? 1e-05 : ((sin_5179 >= 0.99999) ? 0.99999 : sin_5179));
		double expr_5417 = safediv((m_roomsize_29 * 44100), 340);
		double mul_5394 = (expr_5417 * 0.63245);
		double expr_5408 = (-safepow(expr_5416, mul_5394));
		double mul_5397 = (expr_5417 * 1);
		double expr_5415 = (-safepow(expr_5416, mul_5397));
		double mul_5395 = (expr_5417 * 0.7071);
		double expr_5409 = (-safepow(expr_5416, mul_5395));
		double mul_5396 = (expr_5417 * 0.81649);
		double expr_5410 = (-safepow(expr_5416, mul_5396));
		double mul_5391 = (expr_5417 * 0.000527);
		int int_5390 = int(mul_5391);
		double mstosamps_5244 = (m_window_25 * (samplerate * 0.001));
		double add_5320 = (expr_5417 + 5);
		double expr_5411 = safepow(expr_5416, add_5320);
		double mul_5331 = (m_spread_31 * -0.380445);
		double add_5330 = (mul_5331 + 931);
		double rsub_5327 = (1341 - add_5330);
		double mul_5340 = (int_5390 * rsub_5327);
		double mul_5359 = (m_spread_31 * 0.376623);
		double add_5358 = (mul_5359 + 931);
		double rsub_5355 = (1341 - add_5358);
		double mul_5366 = (int_5390 * rsub_5355);
		double mul_5326 = (expr_5417 * 0.41);
		double add_5323 = (mul_5326 + 5);
		double expr_5414 = safepow(expr_5416, add_5323);
		double mul_5325 = (expr_5417 * 0.3);
		double add_5322 = (mul_5325 + 5);
		double expr_5413 = safepow(expr_5416, add_5322);
		double mul_5324 = (expr_5417 * 0.155);
		double add_5321 = (mul_5324 + 5);
		double expr_5412 = safepow(expr_5416, add_5321);
		double rsub_5231 = (1 - m_ratio_24);
		double mul_5230 = (rsub_5231 * 1000);
		double div_5229 = safediv(mul_5230, m_window_25);
		samples_to_seconds = (1 / samplerate);
		double mul_5389 = (expr_5417 * 0.110732);
		double mul_5333 = (m_spread_31 * -0.568366);
		double add_5329 = (mul_5333 + 369);
		double rsub_5328 = (add_5330 - add_5329);
		double mul_5347 = (int_5390 * rsub_5328);
		double mul_5375 = (m_spread_31 * 0.125541);
		double add_5357 = (mul_5375 + 369);
		double rsub_5356 = (add_5358 - add_5357);
		double mul_5373 = (int_5390 * rsub_5356);
		double add_5332 = (mul_5333 + 159);
		double mul_5354 = (int_5390 * add_5332);
		double add_5374 = (mul_5375 + 159);
		double mul_5382 = (int_5390 * add_5374);
		// the main sample loop;
		while ((__n--)) { 
			const double in1 = (*(__in1++));
			double noise_5183 = noise();
			double abs_5200 = fabs(noise_5183);
			double mul_5208 = (abs_5200 * m_blur_28);
			double noise_5182 = noise();
			double abs_5199 = fabs(noise_5182);
			double mul_5205 = (abs_5199 * m_blur_28);
			double noise_5181 = noise();
			double abs_5198 = fabs(noise_5181);
			double mul_5202 = (abs_5198 * m_blur_28);
			double noise_5184 = noise();
			double abs_5201 = fabs(noise_5184);
			double mul_5211 = (abs_5201 * m_blur_28);
			double tap_5294 = m_delay_23.read_linear(mul_5394);
			double mul_5290 = (tap_5294 * expr_5408);
			double mix_5468 = (mul_5290 + (m_damping_30 * (m_history_10 - mul_5290)));
			double mix_5292 = mix_5468;
			double tap_5402 = m_delay_22.read_linear(mul_5397);
			double mul_5393 = (tap_5402 * expr_5415);
			double mix_5469 = (mul_5393 + (m_damping_30 * (m_history_9 - mul_5393)));
			double mix_5400 = mix_5469;
			double tap_5300 = m_delay_21.read_linear(mul_5395);
			double mul_5296 = (tap_5300 * expr_5409);
			double mix_5470 = (mul_5296 + (m_damping_30 * (m_history_8 - mul_5296)));
			double mix_5298 = mix_5470;
			double tap_5306 = m_delay_20.read_linear(mul_5396);
			double mul_5302 = (tap_5306 * expr_5410);
			double mix_5471 = (mul_5302 + (m_damping_30 * (m_history_7 - mul_5302)));
			double mix_5304 = mix_5471;
			double add_5284 = (mix_5400 + mix_5304);
			double add_5282 = (mix_5298 + mix_5292);
			double add_5277 = (add_5284 + add_5282);
			double mul_5260 = (add_5277 * 0.5);
			double sub_5281 = (add_5284 - add_5282);
			double mul_5263 = (sub_5281 * 0.5);
			double tap_5339 = m_delay_19.read_linear(mul_5340);
			double mul_5337 = (tap_5339 * 0.625);
			double tap_5365 = m_delay_18.read_linear(mul_5366);
			double mul_5363 = (tap_5365 * 0.625);
			double sub_5283 = (mix_5400 - mix_5304);
			double sub_5280 = (mix_5298 - mix_5292);
			double sub_5279 = (sub_5283 - sub_5280);
			double mul_5262 = (sub_5279 * 0.5);
			double add_5278 = (sub_5283 + sub_5280);
			double rsub_5276 = (0 - add_5278);
			double mul_5261 = (rsub_5276 * 0.5);
			double tap_5308 = m_delay_17.read_linear(add_5323);
			double tap_5309 = m_delay_17.read_linear(add_5322);
			double tap_5310 = m_delay_17.read_linear(add_5321);
			double tap_5311 = m_delay_17.read_linear(add_5320);
			double mul_5312 = (tap_5311 * expr_5411);
			double add_5285 = (mul_5260 + mul_5312);
			double mul_5318 = (tap_5308 * expr_5414);
			double add_5288 = (mul_5263 + mul_5318);
			double mul_5316 = (tap_5309 * expr_5413);
			double add_5287 = (mul_5262 + mul_5316);
			double mul_5314 = (tap_5310 * expr_5412);
			double add_5286 = (mul_5261 + mul_5314);
			double phasor_5250 = m_phasor_37(div_5229, samples_to_seconds);
			double add_5243 = ((m_history_6 + phasor_5250) + 0.25);
			double mod_5242 = safemod(add_5243, 1);
			double delta_5210 = m_delta_38(mod_5242);
			double sah_5189 = m_sah_39(mul_5208, delta_5210, 0);
			double sah_5209 = m_sah_40(mstosamps_5244, delta_5210, 0);
			double mul_5196 = (sah_5209 * mod_5242);
			double sub_5241 = (mod_5242 - 0.5);
			double mul_5240 = (sub_5241 * 3.1415926535898);
			double cos_5239 = cos(mul_5240);
			double mul_5215 = (cos_5239 * cos_5239);
			double add_5228 = ((m_history_5 + phasor_5250) + 0.5);
			double mod_5227 = safemod(add_5228, 1);
			double delta_5207 = m_delta_41(mod_5227);
			double sah_5187 = m_sah_42(mul_5205, delta_5207, 0);
			double sah_5206 = m_sah_43(mstosamps_5244, delta_5207, 0);
			double mul_5195 = (sah_5206 * mod_5227);
			double sub_5226 = (mod_5227 - 0.5);
			double mul_5225 = (sub_5226 * 3.1415926535898);
			double cos_5224 = cos(mul_5225);
			double mul_5214 = (cos_5224 * cos_5224);
			double add_5222 = ((m_history_4 + phasor_5250) + 0.75);
			double mod_5221 = safemod(add_5222, 1);
			double delta_5204 = m_delta_44(mod_5221);
			double sah_5185 = m_sah_45(mul_5202, delta_5204, 0);
			double sah_5203 = m_sah_46(mstosamps_5244, delta_5204, 0);
			double mul_5194 = (sah_5203 * mod_5221);
			double sub_5220 = (mod_5221 - 0.5);
			double mul_5219 = (sub_5220 * 3.1415926535898);
			double cos_5218 = cos(mul_5219);
			double mul_5213 = (cos_5218 * cos_5218);
			double add_5249 = ((m_history_3 + phasor_5250) + 0);
			double mod_5248 = safemod(add_5249, 1);
			double delta_5192 = m_delta_47(mod_5248);
			double sah_5191 = m_sah_48(mul_5211, delta_5192, 0);
			double sah_5212 = m_sah_49(mstosamps_5244, delta_5192, 0);
			double mul_5197 = (sah_5212 * mod_5248);
			double tap_5235 = m_delay_16.read_linear(mul_5197);
			double tap_5236 = m_delay_16.read_linear(mul_5196);
			double tap_5237 = m_delay_16.read_linear(mul_5195);
			double tap_5238 = m_delay_16.read_linear(mul_5194);
			double mul_5217 = (tap_5238 * mul_5213);
			double mul_5223 = (tap_5237 * mul_5214);
			double mul_5232 = (tap_5236 * mul_5215);
			double sub_5247 = (mod_5248 - 0.5);
			double mul_5246 = (sub_5247 * 3.1415926535898);
			double cos_5245 = cos(mul_5246);
			double mul_5216 = (cos_5245 * cos_5245);
			double mul_5233 = (tap_5235 * mul_5216);
			double add_5473 = (((mul_5233 + mul_5232) + mul_5223) + mul_5217);
			double mix_5472 = (m_y_2 + (clamp_5180 * (add_5473 - m_y_2)));
			double mix_5177 = mix_5472;
			double mix_5474 = (in1 + (m_shimmer_27 * (mix_5177 - in1)));
			double mix_5251 = mix_5474;
			double mul_5254 = (mix_5251 * 0.707);
			double mix_5475 = (mul_5254 + (rsub_5405 * (m_history_1 - mul_5254)));
			double mix_5404 = mix_5475;
			double tap_5388 = m_delay_15.read_linear(mul_5389);
			double mul_5386 = (tap_5388 * 0.75);
			double sub_5385 = (mix_5404 - mul_5386);
			double mul_5384 = (sub_5385 * 0.75);
			double add_5383 = (mul_5384 + tap_5388);
			double tap_5346 = m_delay_14.read_linear(mul_5347);
			double mul_5344 = (tap_5346 * 0.625);
			double tap_5372 = m_delay_13.read_linear(mul_5373);
			double mul_5370 = (tap_5372 * 0.625);
			double tap_5353 = m_delay_12.read_linear(mul_5354);
			double mul_5351 = (tap_5353 * 0.75);
			double tap_5381 = m_delay_11.read_linear(mul_5382);
			double mul_5379 = (tap_5381 * 0.75);
			double mul_5275 = (mul_5263 * m_tail_33);
			double mul_5273 = (mul_5261 * m_tail_33);
			double add_5259 = (mul_5275 + mul_5273);
			double mul_5274 = (mul_5262 * m_tail_33);
			double mul_5272 = (mul_5260 * m_tail_33);
			double add_5258 = (mul_5274 + mul_5272);
			double sub_5267 = (add_5259 - add_5258);
			double mul_5271 = (mul_5318 * m_early_35);
			double mul_5269 = (mul_5314 * m_early_35);
			double add_5257 = (mul_5271 + mul_5269);
			double mul_5270 = (mul_5316 * m_early_35);
			double mul_5268 = (mul_5312 * m_early_35);
			double add_5256 = (mul_5270 + mul_5268);
			double sub_5266 = (add_5257 - add_5256);
			double add_5253 = (sub_5267 + sub_5266);
			double add_5255 = (add_5253 + mix_5251);
			double sub_5350 = (add_5255 - mul_5351);
			double mul_5349 = (sub_5350 * 0.75);
			double add_5348 = (mul_5349 + tap_5353);
			double sub_5343 = (add_5348 - mul_5344);
			double mul_5342 = (sub_5343 * 0.625);
			double add_5341 = (mul_5342 + tap_5346);
			double sub_5336 = (add_5341 - mul_5337);
			double mul_5335 = (sub_5336 * 0.625);
			double add_5334 = (mul_5335 + tap_5339);
			double mul_5252 = (add_5334 * m_mix_36);
			double out2 = (in1 + mul_5252);
			double add_5265 = (add_5253 + mix_5251);
			double sub_5378 = (add_5265 - mul_5379);
			double mul_5377 = (sub_5378 * 0.75);
			double add_5376 = (mul_5377 + tap_5381);
			double sub_5369 = (add_5376 - mul_5370);
			double mul_5368 = (sub_5369 * 0.625);
			double add_5367 = (mul_5368 + tap_5372);
			double sub_5362 = (add_5367 - mul_5363);
			double mul_5361 = (sub_5362 * 0.625);
			double add_5360 = (mul_5361 + tap_5365);
			double mul_5264 = (add_5360 * m_mix_36);
			double out1 = (in1 + mul_5264);
			double history_5291_next_5418 = mix_5292;
			double history_5399_next_5419 = mix_5400;
			double history_5297_next_5420 = mix_5298;
			double history_5303_next_5421 = mix_5304;
			double history_5190_next_5422 = sah_5189;
			double history_5188_next_5423 = sah_5187;
			double history_5186_next_5424 = sah_5185;
			double history_5193_next_5425 = sah_5191;
			double y0_next_5426 = mix_5177;
			double history_5403_next_5427 = mix_5404;
			m_delay_23.write(add_5285);
			m_delay_22.write(add_5288);
			m_delay_21.write(add_5286);
			m_delay_20.write(add_5287);
			m_delay_19.write(sub_5336);
			m_delay_18.write(sub_5362);
			m_delay_17.write(add_5383);
			m_delay_16.write(in1);
			m_delay_15.write(sub_5385);
			m_delay_14.write(sub_5343);
			m_delay_13.write(sub_5369);
			m_delay_12.write(sub_5350);
			m_delay_11.write(sub_5378);
			m_history_10 = history_5291_next_5418;
			m_history_9 = history_5399_next_5419;
			m_history_8 = history_5297_next_5420;
			m_history_7 = history_5303_next_5421;
			m_history_6 = history_5190_next_5422;
			m_history_5 = history_5188_next_5423;
			m_history_4 = history_5186_next_5424;
			m_history_3 = history_5193_next_5425;
			m_y_2 = y0_next_5426;
			m_history_1 = history_5403_next_5427;
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
	inline void set_ratio(double _value) {
		m_ratio_24 = (_value < 0.5 ? 0.5 : (_value > 2 ? 2 : _value));
	};
	inline void set_window(double _value) {
		m_window_25 = (_value < 0.1 ? 0.1 : (_value > 1000 ? 1000 : _value));
	};
	inline void set_decay(double _value) {
		m_decay_26 = (_value < 0.1 ? 0.1 : (_value > 360 ? 360 : _value));
	};
	inline void set_shimmer(double _value) {
		m_shimmer_27 = (_value < 0 ? 0 : (_value > 1 ? 1 : _value));
	};
	inline void set_blur(double _value) {
		m_blur_28 = (_value < 0.01 ? 0.01 : (_value > 0.25 ? 0.25 : _value));
	};
	inline void set_roomsize(double _value) {
		m_roomsize_29 = (_value < 0.1 ? 0.1 : (_value > 300 ? 300 : _value));
	};
	inline void set_damping(double _value) {
		m_damping_30 = (_value < 0 ? 0 : (_value > 1 ? 1 : _value));
	};
	inline void set_spread(double _value) {
		m_spread_31 = (_value < 0 ? 0 : (_value > 100 ? 100 : _value));
	};
	inline void set_tone(double _value) {
		m_tone_32 = (_value < 0 ? 0 : (_value > 1 ? 1 : _value));
	};
	inline void set_tail(double _value) {
		m_tail_33 = (_value < 0 ? 0 : (_value > 1 ? 1 : _value));
	};
	inline void set_cutoff(double _value) {
		m_cutoff_34 = (_value < 0 ? 0 : (_value > 6000 ? 6000 : _value));
	};
	inline void set_early(double _value) {
		m_early_35 = (_value < 0 ? 0 : (_value > 1 ? 1 : _value));
	};
	inline void set_mix(double _value) {
		m_mix_36 = (_value < 0 ? 0 : (_value > 1 ? 1 : _value));
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
int num_params() { return 13; }

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
		case 0: self->set_ratio(value); break;
		case 1: self->set_window(value); break;
		case 2: self->set_decay(value); break;
		case 3: self->set_shimmer(value); break;
		case 4: self->set_blur(value); break;
		case 5: self->set_roomsize(value); break;
		case 6: self->set_damping(value); break;
		case 7: self->set_spread(value); break;
		case 8: self->set_tone(value); break;
		case 9: self->set_tail(value); break;
		case 10: self->set_cutoff(value); break;
		case 11: self->set_early(value); break;
		case 12: self->set_mix(value); break;
		
		default: break;
	}
}

/// Get the value of a parameter of a State object 

void getparameter(CommonState *cself, long index, double *value) {
	State *self = (State *)cself;
	switch (index) {
		case 0: *value = self->m_ratio_24; break;
		case 1: *value = self->m_window_25; break;
		case 2: *value = self->m_decay_26; break;
		case 3: *value = self->m_shimmer_27; break;
		case 4: *value = self->m_blur_28; break;
		case 5: *value = self->m_roomsize_29; break;
		case 6: *value = self->m_damping_30; break;
		case 7: *value = self->m_spread_31; break;
		case 8: *value = self->m_tone_32; break;
		case 9: *value = self->m_tail_33; break;
		case 10: *value = self->m_cutoff_34; break;
		case 11: *value = self->m_early_35; break;
		case 12: *value = self->m_mix_36; break;
		
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
	self->__commonstate.params = (ParamInfo *)genlib_sysmem_newptr(13 * sizeof(ParamInfo));
	self->__commonstate.numparams = 13;
	// initialize parameter 0 ("m_ratio_24")
	pi = self->__commonstate.params + 0;
	pi->name = "ratio";
	pi->paramtype = GENLIB_PARAMTYPE_FLOAT;
	pi->defaultvalue = self->m_ratio_24;
	pi->defaultref = 0;
	pi->hasinputminmax = false;
	pi->inputmin = 0; 
	pi->inputmax = 1;
	pi->hasminmax = true;
	pi->outputmin = 0.5;
	pi->outputmax = 2;
	pi->exp = 0;
	pi->units = "";		// no units defined
	// initialize parameter 1 ("m_window_25")
	pi = self->__commonstate.params + 1;
	pi->name = "window";
	pi->paramtype = GENLIB_PARAMTYPE_FLOAT;
	pi->defaultvalue = self->m_window_25;
	pi->defaultref = 0;
	pi->hasinputminmax = false;
	pi->inputmin = 0; 
	pi->inputmax = 1;
	pi->hasminmax = true;
	pi->outputmin = 0.1;
	pi->outputmax = 1000;
	pi->exp = 0;
	pi->units = "";		// no units defined
	// initialize parameter 2 ("m_decay_26")
	pi = self->__commonstate.params + 2;
	pi->name = "decay";
	pi->paramtype = GENLIB_PARAMTYPE_FLOAT;
	pi->defaultvalue = self->m_decay_26;
	pi->defaultref = 0;
	pi->hasinputminmax = false;
	pi->inputmin = 0; 
	pi->inputmax = 1;
	pi->hasminmax = true;
	pi->outputmin = 0.1;
	pi->outputmax = 360;
	pi->exp = 0;
	pi->units = "";		// no units defined
	// initialize parameter 3 ("m_shimmer_27")
	pi = self->__commonstate.params + 3;
	pi->name = "shimmer";
	pi->paramtype = GENLIB_PARAMTYPE_FLOAT;
	pi->defaultvalue = self->m_shimmer_27;
	pi->defaultref = 0;
	pi->hasinputminmax = false;
	pi->inputmin = 0; 
	pi->inputmax = 1;
	pi->hasminmax = true;
	pi->outputmin = 0;
	pi->outputmax = 1;
	pi->exp = 0;
	pi->units = "";		// no units defined
	// initialize parameter 4 ("m_blur_28")
	pi = self->__commonstate.params + 4;
	pi->name = "blur";
	pi->paramtype = GENLIB_PARAMTYPE_FLOAT;
	pi->defaultvalue = self->m_blur_28;
	pi->defaultref = 0;
	pi->hasinputminmax = false;
	pi->inputmin = 0; 
	pi->inputmax = 1;
	pi->hasminmax = true;
	pi->outputmin = 0.01;
	pi->outputmax = 0.25;
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
	pi->outputmin = 0.1;
	pi->outputmax = 300;
	pi->exp = 0;
	pi->units = "";		// no units defined
	// initialize parameter 6 ("m_damping_30")
	pi = self->__commonstate.params + 6;
	pi->name = "damping";
	pi->paramtype = GENLIB_PARAMTYPE_FLOAT;
	pi->defaultvalue = self->m_damping_30;
	pi->defaultref = 0;
	pi->hasinputminmax = false;
	pi->inputmin = 0; 
	pi->inputmax = 1;
	pi->hasminmax = true;
	pi->outputmin = 0;
	pi->outputmax = 1;
	pi->exp = 0;
	pi->units = "";		// no units defined
	// initialize parameter 7 ("m_spread_31")
	pi = self->__commonstate.params + 7;
	pi->name = "spread";
	pi->paramtype = GENLIB_PARAMTYPE_FLOAT;
	pi->defaultvalue = self->m_spread_31;
	pi->defaultref = 0;
	pi->hasinputminmax = false;
	pi->inputmin = 0; 
	pi->inputmax = 1;
	pi->hasminmax = true;
	pi->outputmin = 0;
	pi->outputmax = 100;
	pi->exp = 0;
	pi->units = "";		// no units defined
	// initialize parameter 8 ("m_tone_32")
	pi = self->__commonstate.params + 8;
	pi->name = "tone";
	pi->paramtype = GENLIB_PARAMTYPE_FLOAT;
	pi->defaultvalue = self->m_tone_32;
	pi->defaultref = 0;
	pi->hasinputminmax = false;
	pi->inputmin = 0; 
	pi->inputmax = 1;
	pi->hasminmax = true;
	pi->outputmin = 0;
	pi->outputmax = 1;
	pi->exp = 0;
	pi->units = "";		// no units defined
	// initialize parameter 9 ("m_tail_33")
	pi = self->__commonstate.params + 9;
	pi->name = "tail";
	pi->paramtype = GENLIB_PARAMTYPE_FLOAT;
	pi->defaultvalue = self->m_tail_33;
	pi->defaultref = 0;
	pi->hasinputminmax = false;
	pi->inputmin = 0; 
	pi->inputmax = 1;
	pi->hasminmax = true;
	pi->outputmin = 0;
	pi->outputmax = 1;
	pi->exp = 0;
	pi->units = "";		// no units defined
	// initialize parameter 10 ("m_cutoff_34")
	pi = self->__commonstate.params + 10;
	pi->name = "cutoff";
	pi->paramtype = GENLIB_PARAMTYPE_FLOAT;
	pi->defaultvalue = self->m_cutoff_34;
	pi->defaultref = 0;
	pi->hasinputminmax = false;
	pi->inputmin = 0; 
	pi->inputmax = 1;
	pi->hasminmax = true;
	pi->outputmin = 0;
	pi->outputmax = 6000;
	pi->exp = 0;
	pi->units = "";		// no units defined
	// initialize parameter 11 ("m_early_35")
	pi = self->__commonstate.params + 11;
	pi->name = "early";
	pi->paramtype = GENLIB_PARAMTYPE_FLOAT;
	pi->defaultvalue = self->m_early_35;
	pi->defaultref = 0;
	pi->hasinputminmax = false;
	pi->inputmin = 0; 
	pi->inputmax = 1;
	pi->hasminmax = true;
	pi->outputmin = 0;
	pi->outputmax = 1;
	pi->exp = 0;
	pi->units = "";		// no units defined
	// initialize parameter 12 ("m_mix_36")
	pi = self->__commonstate.params + 12;
	pi->name = "mix";
	pi->paramtype = GENLIB_PARAMTYPE_FLOAT;
	pi->defaultvalue = self->m_mix_36;
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
