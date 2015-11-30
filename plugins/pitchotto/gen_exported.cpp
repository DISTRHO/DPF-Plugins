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
	Delay m_delay_12;
	Delay m_delay_11;
	Delta m_delta_22;
	Delta m_delta_38;
	Delta m_delta_41;
	Delta m_delta_31;
	Delta m_delta_28;
	Delta m_delta_25;
	Delta m_delta_44;
	Delta m_delta_35;
	Phasor m_phasor_34;
	Phasor m_phasor_21;
	Sah m_sah_23;
	Sah m_sah_27;
	Sah m_sah_42;
	Sah m_sah_26;
	Sah m_sah_24;
	Sah m_sah_43;
	Sah m_sah_36;
	Sah m_sah_29;
	Sah m_sah_40;
	Sah m_sah_37;
	Sah m_sah_30;
	Sah m_sah_39;
	Sah m_sah_32;
	Sah m_sah_33;
	Sah m_sah_46;
	Sah m_sah_45;
	double m_history_5;
	double m_history_6;
	double m_history_7;
	double samples_to_seconds;
	double m_history_4;
	double m_history_2;
	double m_history_3;
	double samplerate;
	double m_y_1;
	double m_history_9;
	double m_history_8;
	double m_resonance_13;
	double m_y_10;
	double m_window_19;
	double m_ratio_20;
	double m_blur_17;
	double m_ratio_18;
	double m_window_15;
	double m_cutoff_14;
	double m_mix_16;
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
		m_y_10 = 0;
		m_delay_11.reset("m_delay_11", 96000);
		m_delay_12.reset("m_delay_12", 96000);
		m_resonance_13 = 0;
		m_cutoff_14 = 2250;
		m_window_15 = 100;
		m_mix_16 = 0.5;
		m_blur_17 = 0.25;
		m_ratio_18 = 0.5;
		m_window_19 = 100;
		m_ratio_20 = 2;
		samples_to_seconds = (1 / samplerate);
		m_phasor_21.reset(0);
		m_delta_22.reset(0);
		m_sah_23.reset(0);
		m_sah_24.reset(0);
		m_delta_25.reset(0);
		m_sah_26.reset(0);
		m_sah_27.reset(0);
		m_delta_28.reset(0);
		m_sah_29.reset(0);
		m_sah_30.reset(0);
		m_delta_31.reset(0);
		m_sah_32.reset(0);
		m_sah_33.reset(0);
		m_phasor_34.reset(0);
		m_delta_35.reset(0);
		m_sah_36.reset(0);
		m_sah_37.reset(0);
		m_delta_38.reset(0);
		m_sah_39.reset(0);
		m_sah_40.reset(0);
		m_delta_41.reset(0);
		m_sah_42.reset(0);
		m_sah_43.reset(0);
		m_delta_44.reset(0);
		m_sah_45.reset(0);
		m_sah_46.reset(0);
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
		double mul_2327 = (m_resonance_13 * 0.125);
		double exp_2328 = exp(mul_2327);
		double mul_2326 = (exp_2328 * 0.882497);
		double mul_2322 = (mul_2326 * mul_2326);
		double expr_2437 = safediv(((m_cutoff_14 * 2) * 3.1415926535898), 44100);
		double cos_2325 = cos(expr_2437);
		double mul_2324 = (cos_2325 * mul_2326);
		double mul_2323 = (mul_2324 * -2);
		double add_2321 = ((mul_2323 + mul_2322) + 1);
		double mstosamps_2393 = (m_window_15 * (samplerate * 0.001));
		double mstosamps_2309 = (m_window_19 * (samplerate * 0.001));
		double rsub_2296 = (1 - m_ratio_20);
		double mul_2295 = (rsub_2296 * 1000);
		double div_2294 = safediv(mul_2295, m_window_19);
		samples_to_seconds = (1 / samplerate);
		double rsub_2380 = (1 - m_ratio_18);
		double mul_2379 = (rsub_2380 * 1000);
		double div_2378 = safediv(mul_2379, m_window_15);
		// the main sample loop;
		while ((__n--)) { 
			const double in1 = (*(__in1++));
			double noise_2247 = noise();
			double abs_2264 = fabs(noise_2247);
			double mul_2270 = (abs_2264 * m_blur_17);
			double noise_2246 = noise();
			double abs_2263 = fabs(noise_2246);
			double mul_2267 = (abs_2263 * m_blur_17);
			double noise_2330 = noise();
			double abs_2348 = fabs(noise_2330);
			double mul_2354 = (abs_2348 * m_blur_17);
			double noise_2332 = noise();
			double abs_2350 = fabs(noise_2332);
			double mul_2360 = (abs_2350 * m_blur_17);
			double noise_2249 = noise();
			double abs_2266 = fabs(noise_2249);
			double mul_2276 = (abs_2266 * m_blur_17);
			double noise_2248 = noise();
			double abs_2265 = fabs(noise_2248);
			double mul_2273 = (abs_2265 * m_blur_17);
			double noise_2331 = noise();
			double abs_2349 = fabs(noise_2331);
			double mul_2357 = (abs_2349 * m_blur_17);
			double noise_2329 = noise();
			double abs_2347 = fabs(noise_2329);
			double mul_2351 = (abs_2347 * m_blur_17);
			double mul_2318 = (mul_2322 * m_y_10);
			double mul_2319 = (mul_2323 * m_y_1);
			double phasor_2315 = m_phasor_21(div_2294, samples_to_seconds);
			double add_2293 = ((phasor_2315 + m_history_9) + 0.5);
			double mod_2292 = safemod(add_2293, 1);
			double delta_2272 = m_delta_22(mod_2292);
			double sah_2252 = m_sah_23(mul_2270, delta_2272, 0);
			double sah_2271 = m_sah_24(mstosamps_2309, delta_2272, 0);
			double mul_2260 = (sah_2271 * mod_2292);
			double sub_2291 = (mod_2292 - 0.5);
			double mul_2290 = (sub_2291 * 3.1415926535898);
			double cos_2289 = cos(mul_2290);
			double mul_2279 = (cos_2289 * cos_2289);
			double add_2287 = ((phasor_2315 + m_history_8) + 0.75);
			double mod_2286 = safemod(add_2287, 1);
			double delta_2269 = m_delta_25(mod_2286);
			double sah_2250 = m_sah_26(mul_2267, delta_2269, 0);
			double sah_2268 = m_sah_27(mstosamps_2309, delta_2269, 0);
			double mul_2259 = (sah_2268 * mod_2286);
			double sub_2285 = (mod_2286 - 0.5);
			double mul_2284 = (sub_2285 * 3.1415926535898);
			double cos_2283 = cos(mul_2284);
			double mul_2278 = (cos_2283 * cos_2283);
			double add_2314 = ((phasor_2315 + m_history_7) + 0);
			double mod_2313 = safemod(add_2314, 1);
			double delta_2257 = m_delta_28(mod_2313);
			double sah_2256 = m_sah_29(mul_2276, delta_2257, 0);
			double sah_2277 = m_sah_30(mstosamps_2309, delta_2257, 0);
			double mul_2262 = (sah_2277 * mod_2313);
			double sub_2312 = (mod_2313 - 0.5);
			double mul_2311 = (sub_2312 * 3.1415926535898);
			double cos_2310 = cos(mul_2311);
			double mul_2281 = (cos_2310 * cos_2310);
			double add_2308 = ((phasor_2315 + m_history_6) + 0.25);
			double mod_2307 = safemod(add_2308, 1);
			double delta_2275 = m_delta_31(mod_2307);
			double sah_2254 = m_sah_32(mul_2273, delta_2275, 0);
			double sah_2274 = m_sah_33(mstosamps_2309, delta_2275, 0);
			double mul_2261 = (sah_2274 * mod_2307);
			double tap_2300 = m_delay_12.read_linear(mul_2262);
			double tap_2301 = m_delay_12.read_linear(mul_2261);
			double tap_2302 = m_delay_12.read_linear(mul_2260);
			double tap_2303 = m_delay_12.read_linear(mul_2259);
			double mul_2282 = (tap_2303 * mul_2278);
			double mul_2288 = (tap_2302 * mul_2279);
			double mul_2298 = (tap_2300 * mul_2281);
			double sub_2306 = (mod_2307 - 0.5);
			double mul_2305 = (sub_2306 * 3.1415926535898);
			double cos_2304 = cos(mul_2305);
			double mul_2280 = (cos_2304 * cos_2304);
			double mul_2297 = (tap_2301 * mul_2280);
			double phasor_2399 = m_phasor_34(div_2378, samples_to_seconds);
			double add_2377 = ((m_history_5 + phasor_2399) + 0.5);
			double mod_2376 = safemod(add_2377, 1);
			double delta_2356 = m_delta_35(mod_2376);
			double sah_2335 = m_sah_36(mul_2354, delta_2356, 0);
			double sah_2355 = m_sah_37(mstosamps_2393, delta_2356, 0);
			double mul_2344 = (sah_2355 * mod_2376);
			double sub_2375 = (mod_2376 - 0.5);
			double mul_2374 = (sub_2375 * 3.1415926535898);
			double cos_2373 = cos(mul_2374);
			double mul_2363 = (cos_2373 * cos_2373);
			double add_2398 = ((m_history_4 + phasor_2399) + 0);
			double mod_2397 = safemod(add_2398, 1);
			double delta_2340 = m_delta_38(mod_2397);
			double sah_2339 = m_sah_39(mul_2360, delta_2340, 0);
			double sah_2361 = m_sah_40(mstosamps_2393, delta_2340, 0);
			double mul_2346 = (sah_2361 * mod_2397);
			double sub_2396 = (mod_2397 - 0.5);
			double mul_2395 = (sub_2396 * 3.1415926535898);
			double cos_2394 = cos(mul_2395);
			double mul_2365 = (cos_2394 * cos_2394);
			double add_2392 = ((m_history_3 + phasor_2399) + 0.25);
			double mod_2391 = safemod(add_2392, 1);
			double delta_2359 = m_delta_41(mod_2391);
			double sah_2337 = m_sah_42(mul_2357, delta_2359, 0);
			double sah_2358 = m_sah_43(mstosamps_2393, delta_2359, 0);
			double mul_2345 = (sah_2358 * mod_2391);
			double sub_2390 = (mod_2391 - 0.5);
			double mul_2389 = (sub_2390 * 3.1415926535898);
			double cos_2388 = cos(mul_2389);
			double mul_2364 = (cos_2388 * cos_2388);
			double add_2371 = ((m_history_2 + phasor_2399) + 0.75);
			double mod_2370 = safemod(add_2371, 1);
			double delta_2353 = m_delta_44(mod_2370);
			double sah_2333 = m_sah_45(mul_2351, delta_2353, 0);
			double sah_2352 = m_sah_46(mstosamps_2393, delta_2353, 0);
			double mul_2343 = (sah_2352 * mod_2370);
			double tap_2384 = m_delay_11.read_linear(mul_2346);
			double tap_2385 = m_delay_11.read_linear(mul_2345);
			double tap_2386 = m_delay_11.read_linear(mul_2344);
			double tap_2387 = m_delay_11.read_linear(mul_2343);
			double mul_2382 = (tap_2384 * mul_2365);
			double mul_2381 = (tap_2385 * mul_2364);
			double mul_2372 = (tap_2386 * mul_2363);
			double sub_2369 = (mod_2370 - 0.5);
			double mul_2368 = (sub_2369 * 3.1415926535898);
			double cos_2367 = cos(mul_2368);
			double mul_2362 = (cos_2367 * cos_2367);
			double mul_2366 = (tap_2387 * mul_2362);
			double mul_2320 = ((((((((mul_2298 + mul_2297) + mul_2288) + mul_2282) + mul_2366) + mul_2372) + mul_2381) + mul_2382) * add_2321);
			double sub_2317 = (mul_2320 - (mul_2319 + mul_2318));
			double mix_2450 = (in1 + (m_mix_16 * (sub_2317 - in1)));
			double out1 = mix_2450;
			double y2_next_2440 = m_y_1;
			double history_2253_next_2441 = sah_2252;
			double history_2251_next_2442 = sah_2250;
			double history_2258_next_2443 = sah_2256;
			double history_2255_next_2444 = sah_2254;
			double history_2336_next_2445 = sah_2335;
			double history_2341_next_2446 = sah_2339;
			double history_2338_next_2447 = sah_2337;
			double history_2334_next_2448 = sah_2333;
			double y1_next_2449 = sub_2317;
			m_delay_12.write(in1);
			m_delay_11.write(in1);
			m_y_10 = y2_next_2440;
			m_history_9 = history_2253_next_2441;
			m_history_8 = history_2251_next_2442;
			m_history_7 = history_2258_next_2443;
			m_history_6 = history_2255_next_2444;
			m_history_5 = history_2336_next_2445;
			m_history_4 = history_2341_next_2446;
			m_history_3 = history_2338_next_2447;
			m_history_2 = history_2334_next_2448;
			m_y_1 = y1_next_2449;
			m_delay_11.step();
			m_delay_12.step();
			// assign results to output buffer;
			(*(__out1++)) = out1;
			
		};
		return __exception;
		
	};
	inline void set_resonance(double _value) {
		m_resonance_13 = (_value < 0 ? 0 : (_value > 0.25 ? 0.25 : _value));
	};
	inline void set_cutoff(double _value) {
		m_cutoff_14 = (_value < 0 ? 0 : (_value > 3000 ? 3000 : _value));
	};
	inline void set_window1(double _value) {
		m_window_15 = (_value < 0.1 ? 0.1 : (_value > 1000 ? 1000 : _value));
	};
	inline void set_mix(double _value) {
		m_mix_16 = (_value < 0 ? 0 : (_value > 1 ? 1 : _value));
	};
	inline void set_blur(double _value) {
		m_blur_17 = (_value < 0.01 ? 0.01 : (_value > 0.25 ? 0.25 : _value));
	};
	inline void set_ratio1(double _value) {
		m_ratio_18 = (_value < 0.5 ? 0.5 : (_value > 2 ? 2 : _value));
	};
	inline void set_window2(double _value) {
		m_window_19 = (_value < 0.1 ? 0.1 : (_value > 1000 ? 1000 : _value));
	};
	inline void set_ratio2(double _value) {
		m_ratio_20 = (_value < 0.5 ? 0.5 : (_value > 2 ? 2 : _value));
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
int num_params() { return 8; }

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
		case 0: self->set_resonance(value); break;
		case 1: self->set_cutoff(value); break;
		case 2: self->set_window1(value); break;
		case 3: self->set_mix(value); break;
		case 4: self->set_blur(value); break;
		case 5: self->set_ratio1(value); break;
		case 6: self->set_window2(value); break;
		case 7: self->set_ratio2(value); break;
		
		default: break;
	}
}

/// Get the value of a parameter of a State object 

void getparameter(CommonState *cself, long index, double *value) {
	State *self = (State *)cself;
	switch (index) {
		case 0: *value = self->m_resonance_13; break;
		case 1: *value = self->m_cutoff_14; break;
		case 2: *value = self->m_window_15; break;
		case 3: *value = self->m_mix_16; break;
		case 4: *value = self->m_blur_17; break;
		case 5: *value = self->m_ratio_18; break;
		case 6: *value = self->m_window_19; break;
		case 7: *value = self->m_ratio_20; break;
		
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
	// initialize parameter 0 ("m_resonance_13")
	pi = self->__commonstate.params + 0;
	pi->name = "resonance";
	pi->paramtype = GENLIB_PARAMTYPE_FLOAT;
	pi->defaultvalue = self->m_resonance_13;
	pi->defaultref = 0;
	pi->hasinputminmax = false;
	pi->inputmin = 0; 
	pi->inputmax = 1;
	pi->hasminmax = true;
	pi->outputmin = 0;
	pi->outputmax = 0.25;
	pi->exp = 0;
	pi->units = "";		// no units defined
	// initialize parameter 1 ("m_cutoff_14")
	pi = self->__commonstate.params + 1;
	pi->name = "cutoff";
	pi->paramtype = GENLIB_PARAMTYPE_FLOAT;
	pi->defaultvalue = self->m_cutoff_14;
	pi->defaultref = 0;
	pi->hasinputminmax = false;
	pi->inputmin = 0; 
	pi->inputmax = 1;
	pi->hasminmax = true;
	pi->outputmin = 0;
	pi->outputmax = 3000;
	pi->exp = 0;
	pi->units = "";		// no units defined
	// initialize parameter 2 ("m_window_15")
	pi = self->__commonstate.params + 2;
	pi->name = "window1";
	pi->paramtype = GENLIB_PARAMTYPE_FLOAT;
	pi->defaultvalue = self->m_window_15;
	pi->defaultref = 0;
	pi->hasinputminmax = false;
	pi->inputmin = 0; 
	pi->inputmax = 1;
	pi->hasminmax = true;
	pi->outputmin = 0.1;
	pi->outputmax = 1000;
	pi->exp = 0;
	pi->units = "";		// no units defined
	// initialize parameter 3 ("m_mix_16")
	pi = self->__commonstate.params + 3;
	pi->name = "mix";
	pi->paramtype = GENLIB_PARAMTYPE_FLOAT;
	pi->defaultvalue = self->m_mix_16;
	pi->defaultref = 0;
	pi->hasinputminmax = false;
	pi->inputmin = 0; 
	pi->inputmax = 1;
	pi->hasminmax = true;
	pi->outputmin = 0;
	pi->outputmax = 1;
	pi->exp = 0;
	pi->units = "";		// no units defined
	// initialize parameter 4 ("m_blur_17")
	pi = self->__commonstate.params + 4;
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
	// initialize parameter 5 ("m_ratio_18")
	pi = self->__commonstate.params + 5;
	pi->name = "ratio1";
	pi->paramtype = GENLIB_PARAMTYPE_FLOAT;
	pi->defaultvalue = self->m_ratio_18;
	pi->defaultref = 0;
	pi->hasinputminmax = false;
	pi->inputmin = 0; 
	pi->inputmax = 1;
	pi->hasminmax = true;
	pi->outputmin = 0.5;
	pi->outputmax = 2;
	pi->exp = 0;
	pi->units = "";		// no units defined
	// initialize parameter 6 ("m_window_19")
	pi = self->__commonstate.params + 6;
	pi->name = "window2";
	pi->paramtype = GENLIB_PARAMTYPE_FLOAT;
	pi->defaultvalue = self->m_window_19;
	pi->defaultref = 0;
	pi->hasinputminmax = false;
	pi->inputmin = 0; 
	pi->inputmax = 1;
	pi->hasminmax = true;
	pi->outputmin = 0.1;
	pi->outputmax = 1000;
	pi->exp = 0;
	pi->units = "";		// no units defined
	// initialize parameter 7 ("m_ratio_20")
	pi = self->__commonstate.params + 7;
	pi->name = "ratio2";
	pi->paramtype = GENLIB_PARAMTYPE_FLOAT;
	pi->defaultvalue = self->m_ratio_20;
	pi->defaultref = 0;
	pi->hasinputminmax = false;
	pi->inputmin = 0; 
	pi->inputmax = 1;
	pi->hasminmax = true;
	pi->outputmin = 0.5;
	pi->outputmax = 2;
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
