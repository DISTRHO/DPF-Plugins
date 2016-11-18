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
	Delay m_delay_24;
	Delay m_delay_15;
	Delay m_delay_13;
	Delay m_delay_23;
	Delay m_delay_9;
	Delay m_delay_17;
	Delay m_delay_21;
	Delay m_delay_19;
	Delay m_delay_22;
	Delay m_delay_7;
	Delay m_delay_11;
	Delay m_delay_5;
	t_sample m_history_6;
	t_sample m_history_18;
	t_sample m_fb_1;
	t_sample m_history_20;
	t_sample samplerate;
	t_sample m_damp_2;
	t_sample m_history_8;
	t_sample m_history_16;
	t_sample m_history_14;
	t_sample m_fb_3;
	t_sample m_history_12;
	t_sample m_history_10;
	t_sample m_spread_4;
	int vectorsize;
	int __exception;
	// re-initialize all member variables;
	inline void reset(t_sample __sr, int __vs) {
		__exception = 0;
		vectorsize = __vs;
		samplerate = __sr;
		m_fb_1 = 0.5;
		m_damp_2 = 0.5;
		m_fb_3 = 0.9;
		m_spread_4 = 0;
		m_delay_5.reset("m_delay_5", 2000);
		m_history_6 = 0;
		m_delay_7.reset("m_delay_7", 2000);
		m_history_8 = 0;
		m_delay_9.reset("m_delay_9", 2000);
		m_history_10 = 0;
		m_delay_11.reset("m_delay_11", 2000);
		m_history_12 = 0;
		m_delay_13.reset("m_delay_13", 2000);
		m_history_14 = 0;
		m_delay_15.reset("m_delay_15", 2000);
		m_history_16 = 0;
		m_delay_17.reset("m_delay_17", 2000);
		m_history_18 = 0;
		m_delay_19.reset("m_delay_19", 2000);
		m_history_20 = 0;
		m_delay_21.reset("m_delay_21", 2000);
		m_delay_22.reset("m_delay_22", 2000);
		m_delay_23.reset("m_delay_23", 2000);
		m_delay_24.reset("m_delay_24", 2000);
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
		t_sample mul_448 = (m_fb_1 * 0.5);
		t_sample add_424 = (225 + m_spread_4);
		t_sample add_431 = (341 + m_spread_4);
		t_sample add_446 = (441 + m_spread_4);
		t_sample add_417 = (556 + m_spread_4);
		t_sample damp_327 = m_damp_2;
		t_sample damp_326 = damp_327;
		t_sample damp_328 = damp_327;
		t_sample damp_329 = damp_327;
		t_sample damp_330 = damp_327;
		t_sample damp_331 = damp_327;
		t_sample damp_332 = damp_327;
		t_sample damp_333 = damp_327;
		t_sample add_439 = (1557 + m_spread_4);
		t_sample rsub_295 = (1 - damp_327);
		t_sample add_438 = (1617 + m_spread_4);
		t_sample rsub_466 = (1 - damp_326);
		t_sample add_440 = (1491 + m_spread_4);
		t_sample rsub_479 = (1 - damp_328);
		t_sample add_441 = (1422 + m_spread_4);
		t_sample rsub_484 = (1 - damp_329);
		t_sample add_442 = (1356 + m_spread_4);
		t_sample rsub_496 = (1 - damp_330);
		t_sample add_443 = (1277 + m_spread_4);
		t_sample rsub_508 = (1 - damp_331);
		t_sample add_444 = (1188 + m_spread_4);
		t_sample rsub_520 = (1 - damp_332);
		t_sample add_445 = (1116 + m_spread_4);
		t_sample rsub_532 = (1 - damp_333);
		// the main sample loop;
		while ((__n--)) { 
			const t_sample in1 = (*(__in1++));
			t_sample mul_459 = (in1 * 0.015);
			t_sample tap_302 = m_delay_5.read_linear(add_439);
			t_sample gen_410 = tap_302;
			t_sample mul_300 = (tap_302 * damp_327);
			t_sample mul_298 = (m_history_6 * rsub_295);
			t_sample add_299 = (mul_300 + mul_298);
			t_sample mul_296 = (add_299 * m_fb_3);
			t_sample add_303 = (mul_459 + mul_296);
			t_sample history_297_next_304 = add_299;
			t_sample tap_469 = m_delay_7.read_linear(add_438);
			t_sample gen_458 = tap_469;
			t_sample mul_468 = (tap_469 * damp_326);
			t_sample mul_465 = (m_history_8 * rsub_466);
			t_sample add_464 = (mul_468 + mul_465);
			t_sample mul_462 = (add_464 * m_fb_3);
			t_sample add_463 = (mul_459 + mul_462);
			t_sample history_297_next_461 = add_464;
			t_sample tap_474 = m_delay_9.read_linear(add_440);
			t_sample gen_399 = tap_474;
			t_sample mul_480 = (tap_474 * damp_328);
			t_sample mul_478 = (m_history_10 * rsub_479);
			t_sample add_476 = (mul_480 + mul_478);
			t_sample mul_473 = (add_476 * m_fb_3);
			t_sample add_477 = (mul_459 + mul_473);
			t_sample history_297_next_475 = add_476;
			t_sample tap_489 = m_delay_11.read_linear(add_441);
			t_sample gen_388 = tap_489;
			t_sample mul_493 = (tap_489 * damp_329);
			t_sample mul_487 = (m_history_12 * rsub_484);
			t_sample add_492 = (mul_493 + mul_487);
			t_sample mul_491 = (add_492 * m_fb_3);
			t_sample add_490 = (mul_459 + mul_491);
			t_sample history_297_next_485 = add_492;
			t_sample tap_500 = m_delay_13.read_linear(add_442);
			t_sample gen_377 = tap_500;
			t_sample mul_501 = (tap_500 * damp_330);
			t_sample mul_499 = (m_history_14 * rsub_496);
			t_sample add_505 = (mul_501 + mul_499);
			t_sample mul_504 = (add_505 * m_fb_3);
			t_sample add_503 = (mul_459 + mul_504);
			t_sample history_297_next_497 = add_505;
			t_sample tap_512 = m_delay_15.read_linear(add_443);
			t_sample gen_366 = tap_512;
			t_sample mul_517 = (tap_512 * damp_331);
			t_sample mul_511 = (m_history_16 * rsub_508);
			t_sample add_516 = (mul_517 + mul_511);
			t_sample mul_513 = (add_516 * m_fb_3);
			t_sample add_514 = (mul_459 + mul_513);
			t_sample history_297_next_509 = add_516;
			t_sample tap_525 = m_delay_17.read_linear(add_444);
			t_sample gen_355 = tap_525;
			t_sample mul_521 = (tap_525 * damp_332);
			t_sample mul_524 = (m_history_18 * rsub_520);
			t_sample add_529 = (mul_521 + mul_524);
			t_sample mul_526 = (add_529 * m_fb_3);
			t_sample add_527 = (mul_459 + mul_526);
			t_sample history_297_next_522 = add_529;
			t_sample tap_537 = m_delay_19.read_linear(add_445);
			t_sample gen_344 = tap_537;
			t_sample mul_541 = (tap_537 * damp_333);
			t_sample mul_536 = (m_history_20 * rsub_532);
			t_sample add_538 = (mul_541 + mul_536);
			t_sample mul_540 = (add_538 * m_fb_3);
			t_sample add_534 = (mul_459 + mul_540);
			t_sample history_297_next_533 = add_538;
			t_sample add_447 = ((((((((gen_344 + gen_355) + gen_366) + gen_377) + gen_388) + gen_399) + gen_458) + gen_410) + 0);
			t_sample tap_283 = m_delay_21.read_linear(add_417);
			t_sample sub_279 = (add_447 - tap_283);
			t_sample mul_281 = (tap_283 * mul_448);
			t_sample add_280 = (add_447 + mul_281);
			t_sample tap_548 = m_delay_22.read_linear(add_446);
			t_sample sub_546 = (sub_279 - tap_548);
			t_sample mul_547 = (tap_548 * mul_448);
			t_sample add_544 = (sub_279 + mul_547);
			t_sample tap_554 = m_delay_23.read_linear(add_431);
			t_sample sub_552 = (sub_546 - tap_554);
			t_sample mul_553 = (tap_554 * mul_448);
			t_sample add_550 = (sub_546 + mul_553);
			t_sample tap_560 = m_delay_24.read_linear(add_424);
			t_sample sub_558 = (sub_552 - tap_560);
			t_sample mul_559 = (tap_560 * mul_448);
			t_sample add_556 = (sub_552 + mul_559);
			t_sample out1 = sub_558;
			m_delay_5.write(add_303);
			m_delay_24.write(add_556);
			m_delay_23.write(add_550);
			m_delay_22.write(add_544);
			m_delay_21.write(add_280);
			m_history_20 = history_297_next_533;
			m_delay_19.write(add_534);
			m_history_18 = history_297_next_522;
			m_delay_17.write(add_527);
			m_history_16 = history_297_next_509;
			m_delay_15.write(add_514);
			m_history_14 = history_297_next_497;
			m_delay_13.write(add_503);
			m_history_12 = history_297_next_485;
			m_delay_11.write(add_490);
			m_history_10 = history_297_next_475;
			m_delay_9.write(add_477);
			m_history_8 = history_297_next_461;
			m_delay_7.write(add_463);
			m_history_6 = history_297_next_304;
			m_delay_5.step();
			m_delay_7.step();
			m_delay_9.step();
			m_delay_11.step();
			m_delay_13.step();
			m_delay_15.step();
			m_delay_17.step();
			m_delay_19.step();
			m_delay_21.step();
			m_delay_22.step();
			m_delay_23.step();
			m_delay_24.step();
			// assign results to output buffer;
			(*(__out1++)) = out1;
			
		};
		return __exception;
		
	};
	inline void set_fb2(t_sample _value) {
		m_fb_1 = (_value < 0 ? 0 : (_value > 1 ? 1 : _value));
	};
	inline void set_damp(t_sample _value) {
		m_damp_2 = (_value < 0 ? 0 : (_value > 1 ? 1 : _value));
	};
	inline void set_fb1(t_sample _value) {
		m_fb_3 = (_value < 0 ? 0 : (_value > 1 ? 1 : _value));
	};
	inline void set_spread(t_sample _value) {
		m_spread_4 = (_value < 0 ? 0 : (_value > 400 ? 400 : _value));
	};
	
} State;


/// 
///	Configuration for the genlib API
///

/// Number of signal inputs and outputs 

const int gen_kernel_numins = 1;
const int gen_kernel_numouts = 1;

int num_inputs() { return gen_kernel_numins; }
int num_outputs() { return gen_kernel_numouts; }
int num_params() { return 4; }

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

void setparameter(CommonState *cself, long index, t_param value, void *ref) {
	State * self = (State *)cself;
	switch (index) {
		case 0: self->set_fb2(value); break;
		case 1: self->set_damp(value); break;
		case 2: self->set_fb1(value); break;
		case 3: self->set_spread(value); break;
		
		default: break;
	}
}

/// Get the value of a parameter of a State object 

void getparameter(CommonState *cself, long index, t_param *value) {
	State *self = (State *)cself;
	switch (index) {
		case 0: *value = self->m_fb_1; break;
		case 1: *value = self->m_damp_2; break;
		case 2: *value = self->m_fb_3; break;
		case 3: *value = self->m_spread_4; break;
		
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
	// initialize parameter 0 ("m_fb_1")
	pi = self->__commonstate.params + 0;
	pi->name = "fb2";
	pi->paramtype = GENLIB_PARAMTYPE_FLOAT;
	pi->defaultvalue = self->m_fb_1;
	pi->defaultref = 0;
	pi->hasinputminmax = false;
	pi->inputmin = 0; 
	pi->inputmax = 1;
	pi->hasminmax = true;
	pi->outputmin = 0;
	pi->outputmax = 1;
	pi->exp = 0;
	pi->units = "";		// no units defined
	// initialize parameter 1 ("m_damp_2")
	pi = self->__commonstate.params + 1;
	pi->name = "damp";
	pi->paramtype = GENLIB_PARAMTYPE_FLOAT;
	pi->defaultvalue = self->m_damp_2;
	pi->defaultref = 0;
	pi->hasinputminmax = false;
	pi->inputmin = 0; 
	pi->inputmax = 1;
	pi->hasminmax = true;
	pi->outputmin = 0;
	pi->outputmax = 1;
	pi->exp = 0;
	pi->units = "";		// no units defined
	// initialize parameter 2 ("m_fb_3")
	pi = self->__commonstate.params + 2;
	pi->name = "fb1";
	pi->paramtype = GENLIB_PARAMTYPE_FLOAT;
	pi->defaultvalue = self->m_fb_3;
	pi->defaultref = 0;
	pi->hasinputminmax = false;
	pi->inputmin = 0; 
	pi->inputmax = 1;
	pi->hasminmax = true;
	pi->outputmin = 0;
	pi->outputmax = 1;
	pi->exp = 0;
	pi->units = "";		// no units defined
	// initialize parameter 3 ("m_spread_4")
	pi = self->__commonstate.params + 3;
	pi->name = "spread";
	pi->paramtype = GENLIB_PARAMTYPE_FLOAT;
	pi->defaultvalue = self->m_spread_4;
	pi->defaultref = 0;
	pi->hasinputminmax = false;
	pi->inputmin = 0; 
	pi->inputmax = 1;
	pi->hasminmax = true;
	pi->outputmin = 0;
	pi->outputmax = 400;
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
