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
	Delay m_delay_18;
	Delay m_delay_19;
	Delay m_delay_15;
	Delay m_delay_12;
	Delay m_delay_17;
	Delay m_delay_13;
	Delay m_delay_21;
	Delay m_delay_20;
	Delay m_delay_22;
	Delay m_delay_24;
	Delay m_delay_23;
	Delay m_delay_16;
	Delta m_delta_43;
	Delta m_delta_49;
	Delta m_delta_46;
	Delta m_delta_40;
	Phasor m_phasor_39;
	Sah m_sah_50;
	Sah m_sah_51;
	Sah m_sah_47;
	Sah m_sah_48;
	Sah m_sah_45;
	Sah m_sah_42;
	Sah m_sah_44;
	Sah m_sah_41;
	double m_roomsize_34;
	double m_tail_35;
	double m_blur_36;
	double m_damping_38;
	double m_shimmer_33;
	double m_decay_37;
	double samples_to_seconds;
	double m_early_32;
	double m_ratio_25;
	double m_spread_30;
	double m_history_4;
	double m_history_5;
	double m_history_6;
	double m_history_3;
	double m_history_1;
	double m_y_2;
	double samplerate;
	double m_cutoff_31;
	double m_history_7;
	double m_history_9;
	double m_tone_28;
	double m_mix_29;
	double m_history_8;
	double m_resonance_27;
	double m_y_11;
	double m_history_10;
	double m_window_26;
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
		m_y_11 = 0;
		m_delay_12.reset("m_delay_12", 5000);
		m_delay_13.reset("m_delay_13", 7000);
		m_delay_14.reset("m_delay_14", 15000);
		m_delay_15.reset("m_delay_15", 16000);
		m_delay_16.reset("m_delay_16", 6000);
		m_delay_17.reset("m_delay_17", 48000);
		m_delay_18.reset("m_delay_18", 96000);
		m_delay_19.reset("m_delay_19", 10000);
		m_delay_20.reset("m_delay_20", 12000);
		m_delay_21.reset("m_delay_21", 48000);
		m_delay_22.reset("m_delay_22", 48000);
		m_delay_23.reset("m_delay_23", 48000);
		m_delay_24.reset("m_delay_24", 48000);
		m_ratio_25 = 2;
		m_window_26 = 100;
		m_resonance_27 = 0;
		m_tone_28 = 0.5;
		m_mix_29 = 0.5;
		m_spread_30 = 25;
		m_cutoff_31 = 3000;
		m_early_32 = 0.25;
		m_shimmer_33 = 0.5;
		m_roomsize_34 = 150;
		m_tail_35 = 0.25;
		m_blur_36 = 0.25;
		m_decay_37 = 180;
		m_damping_38 = 0.75;
		samples_to_seconds = (1 / samplerate);
		m_phasor_39.reset(0);
		m_delta_40.reset(0);
		m_sah_41.reset(0);
		m_sah_42.reset(0);
		m_delta_43.reset(0);
		m_sah_44.reset(0);
		m_sah_45.reset(0);
		m_delta_46.reset(0);
		m_sah_47.reset(0);
		m_sah_48.reset(0);
		m_delta_49.reset(0);
		m_sah_50.reset(0);
		m_sah_51.reset(0);
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
		double rsub_552 = (1 - m_tone_28);
		double expr_1476 = safepow(0.001, safediv(1, (m_decay_37 * 44100)));
		double expr_1477 = safediv((m_roomsize_34 * 44100), 340);
		double mul_542 = (expr_1477 * 0.7071);
		double expr_1469 = (-safepow(expr_1476, mul_542));
		double mul_541 = (expr_1477 * 0.63245);
		double expr_1468 = (-safepow(expr_1476, mul_541));
		double mul_543 = (expr_1477 * 0.81649);
		double expr_1470 = (-safepow(expr_1476, mul_543));
		double mul_544 = (expr_1477 * 1);
		double expr_1475 = (-safepow(expr_1476, mul_544));
		double expr_1478 = safediv(((m_cutoff_31 * 2) * 3.1415926535898), 44100);
		double cos_1010 = cos(expr_1478);
		double mul_1012 = (m_resonance_27 * 0.125);
		double exp_1013 = exp(mul_1012);
		double mul_1011 = (exp_1013 * 0.882497);
		double mul_1007 = (mul_1011 * mul_1011);
		double mul_1009 = (cos_1010 * mul_1011);
		double mul_1008 = (mul_1009 * -2);
		double add_1006 = ((mul_1007 + mul_1008) + 1);
		double mul_538 = (expr_1477 * 0.000527);
		int int_537 = int(mul_538);
		double mstosamps_963 = (m_window_26 * (samplerate * 0.001));
		double add_467 = (expr_1477 + 5);
		double expr_1471 = safepow(expr_1476, add_467);
		double mul_478 = (m_spread_30 * -0.380445);
		double add_477 = (mul_478 + 931);
		double rsub_474 = (1341 - add_477);
		double mul_487 = (int_537 * rsub_474);
		double mul_506 = (m_spread_30 * 0.376623);
		double add_505 = (mul_506 + 931);
		double rsub_502 = (1341 - add_505);
		double mul_513 = (int_537 * rsub_502);
		double rsub_950 = (1 - m_ratio_25);
		double mul_949 = (rsub_950 * 1000);
		double div_948 = safediv(mul_949, m_window_26);
		samples_to_seconds = (1 / samplerate);
		double mul_473 = (expr_1477 * 0.41);
		double add_470 = (mul_473 + 5);
		double expr_1474 = safepow(expr_1476, add_470);
		double mul_472 = (expr_1477 * 0.3);
		double add_469 = (mul_472 + 5);
		double expr_1473 = safepow(expr_1476, add_469);
		double mul_471 = (expr_1477 * 0.155);
		double add_468 = (mul_471 + 5);
		double expr_1472 = safepow(expr_1476, add_468);
		double mul_536 = (expr_1477 * 0.110732);
		double mul_480 = (m_spread_30 * -0.568366);
		double add_476 = (mul_480 + 369);
		double rsub_475 = (add_477 - add_476);
		double mul_494 = (int_537 * rsub_475);
		double mul_522 = (m_spread_30 * 0.125541);
		double add_504 = (mul_522 + 369);
		double rsub_503 = (add_505 - add_504);
		double mul_520 = (int_537 * rsub_503);
		double add_479 = (mul_480 + 159);
		double mul_501 = (int_537 * add_479);
		double add_521 = (mul_522 + 159);
		double mul_529 = (int_537 * add_521);
		// the main sample loop;
		while ((__n--)) { 
			const double in1 = (*(__in1++));
			double noise_900 = noise();
			double abs_917 = fabs(noise_900);
			double mul_921 = (abs_917 * m_blur_36);
			double noise_902 = noise();
			double abs_919 = fabs(noise_902);
			double mul_927 = (abs_919 * m_blur_36);
			double noise_903 = noise();
			double abs_920 = fabs(noise_903);
			double mul_930 = (abs_920 * m_blur_36);
			double noise_901 = noise();
			double abs_918 = fabs(noise_901);
			double mul_924 = (abs_918 * m_blur_36);
			double tap_447 = m_delay_24.read_linear(mul_542);
			double mul_443 = (tap_447 * expr_1469);
			double mix_1521 = (mul_443 + (m_damping_38 * (m_history_10 - mul_443)));
			double mix_445 = mix_1521;
			double tap_441 = m_delay_23.read_linear(mul_541);
			double mul_437 = (tap_441 * expr_1468);
			double mix_1522 = (mul_437 + (m_damping_38 * (m_history_9 - mul_437)));
			double mix_439 = mix_1522;
			double tap_453 = m_delay_22.read_linear(mul_543);
			double mul_449 = (tap_453 * expr_1470);
			double mix_1523 = (mul_449 + (m_damping_38 * (m_history_8 - mul_449)));
			double mix_451 = mix_1523;
			double tap_549 = m_delay_21.read_linear(mul_544);
			double mul_540 = (tap_549 * expr_1475);
			double mix_1524 = (mul_540 + (m_damping_38 * (m_history_7 - mul_540)));
			double mix_547 = mix_1524;
			double mul_1003 = (mul_1007 * m_y_11);
			double mul_1004 = (mul_1008 * m_y_2);
			double add_431 = (mix_547 + mix_451);
			double add_429 = (mix_445 + mix_439);
			double add_424 = (add_431 + add_429);
			double mul_407 = (add_424 * 0.5);
			double sub_428 = (add_431 - add_429);
			double mul_410 = (sub_428 * 0.5);
			double sub_430 = (mix_547 - mix_451);
			double sub_427 = (mix_445 - mix_439);
			double sub_426 = (sub_430 - sub_427);
			double mul_409 = (sub_426 * 0.5);
			double add_425 = (sub_430 + sub_427);
			double rsub_423 = (0 - add_425);
			double mul_408 = (rsub_423 * 0.5);
			double tap_486 = m_delay_20.read_linear(mul_487);
			double mul_484 = (tap_486 * 0.625);
			double tap_512 = m_delay_19.read_linear(mul_513);
			double mul_510 = (tap_512 * 0.625);
			double phasor_969 = m_phasor_39(div_948, samples_to_seconds);
			double add_941 = ((phasor_969 + m_history_6) + 0.75);
			double mod_940 = safemod(add_941, 1);
			double delta_923 = m_delta_40(mod_940);
			double sah_904 = m_sah_41(mul_921, delta_923, 0);
			double sah_922 = m_sah_42(mstosamps_963, delta_923, 0);
			double mul_913 = (sah_922 * mod_940);
			double sub_939 = (mod_940 - 0.5);
			double mul_938 = (sub_939 * 3.1415926535898);
			double cos_937 = cos(mul_938);
			double mul_932 = (cos_937 * cos_937);
			double add_962 = ((phasor_969 + m_history_5) + 0.25);
			double mod_961 = safemod(add_962, 1);
			double delta_929 = m_delta_43(mod_961);
			double sah_908 = m_sah_44(mul_927, delta_929, 0);
			double sah_928 = m_sah_45(mstosamps_963, delta_929, 0);
			double mul_915 = (sah_928 * mod_961);
			double sub_960 = (mod_961 - 0.5);
			double mul_959 = (sub_960 * 3.1415926535898);
			double cos_958 = cos(mul_959);
			double mul_934 = (cos_958 * cos_958);
			double add_968 = ((phasor_969 + m_history_4) + 0);
			double mod_967 = safemod(add_968, 1);
			double delta_911 = m_delta_46(mod_967);
			double sah_910 = m_sah_47(mul_930, delta_911, 0);
			double sah_931 = m_sah_48(mstosamps_963, delta_911, 0);
			double mul_916 = (sah_931 * mod_967);
			double sub_966 = (mod_967 - 0.5);
			double mul_965 = (sub_966 * 3.1415926535898);
			double cos_964 = cos(mul_965);
			double mul_935 = (cos_964 * cos_964);
			double add_947 = ((phasor_969 + m_history_3) + 0.5);
			double mod_946 = safemod(add_947, 1);
			double delta_926 = m_delta_49(mod_946);
			double sah_906 = m_sah_50(mul_924, delta_926, 0);
			double sah_925 = m_sah_51(mstosamps_963, delta_926, 0);
			double mul_914 = (sah_925 * mod_946);
			double tap_954 = m_delay_18.read_linear(mul_916);
			double tap_955 = m_delay_18.read_linear(mul_915);
			double tap_956 = m_delay_18.read_linear(mul_914);
			double tap_957 = m_delay_18.read_linear(mul_913);
			double mul_952 = (tap_954 * mul_935);
			double mul_951 = (tap_955 * mul_934);
			double mul_936 = (tap_957 * mul_932);
			double sub_945 = (mod_946 - 0.5);
			double mul_944 = (sub_945 * 3.1415926535898);
			double cos_943 = cos(mul_944);
			double mul_933 = (cos_943 * cos_943);
			double mul_942 = (tap_956 * mul_933);
			double mul_1005 = ((((mul_936 + mul_942) + mul_951) + mul_952) * add_1006);
			double sub_1002 = (mul_1005 - (mul_1003 + mul_1004));
			double mix_1525 = (in1 + (m_shimmer_33 * (sub_1002 - in1)));
			double mix_341 = mix_1525;
			double mul_401 = (mix_341 * 0.707);
			double mix_1526 = (mul_401 + (rsub_552 * (m_history_1 - mul_401)));
			double mix_551 = mix_1526;
			double tap_455 = m_delay_17.read_linear(add_470);
			double tap_456 = m_delay_17.read_linear(add_469);
			double tap_457 = m_delay_17.read_linear(add_468);
			double tap_458 = m_delay_17.read_linear(add_467);
			double mul_459 = (tap_458 * expr_1471);
			double add_432 = (mul_407 + mul_459);
			double mul_463 = (tap_456 * expr_1473);
			double add_434 = (mul_409 + mul_463);
			double mul_465 = (tap_455 * expr_1474);
			double add_435 = (mul_410 + mul_465);
			double mul_461 = (tap_457 * expr_1472);
			double add_433 = (mul_408 + mul_461);
			double tap_535 = m_delay_16.read_linear(mul_536);
			double mul_533 = (tap_535 * 0.75);
			double sub_532 = (mix_551 - mul_533);
			double mul_531 = (sub_532 * 0.75);
			double add_530 = (mul_531 + tap_535);
			double tap_493 = m_delay_15.read_linear(mul_494);
			double mul_491 = (tap_493 * 0.625);
			double tap_519 = m_delay_14.read_linear(mul_520);
			double mul_517 = (tap_519 * 0.625);
			double tap_500 = m_delay_13.read_linear(mul_501);
			double mul_498 = (tap_500 * 0.75);
			double tap_528 = m_delay_12.read_linear(mul_529);
			double mul_526 = (tap_528 * 0.75);
			double mul_422 = (mul_410 * m_tail_35);
			double mul_420 = (mul_408 * m_tail_35);
			double add_406 = (mul_422 + mul_420);
			double mul_421 = (mul_409 * m_tail_35);
			double mul_419 = (mul_407 * m_tail_35);
			double add_405 = (mul_421 + mul_419);
			double sub_414 = (add_406 - add_405);
			double mul_418 = (mul_465 * m_early_32);
			double mul_416 = (mul_461 * m_early_32);
			double add_404 = (mul_418 + mul_416);
			double mul_417 = (mul_463 * m_early_32);
			double mul_415 = (mul_459 * m_early_32);
			double add_403 = (mul_417 + mul_415);
			double sub_413 = (add_404 - add_403);
			double add_400 = (sub_414 + sub_413);
			double add_402 = (add_400 + mix_341);
			double sub_497 = (add_402 - mul_498);
			double mul_496 = (sub_497 * 0.75);
			double add_495 = (mul_496 + tap_500);
			double sub_490 = (add_495 - mul_491);
			double mul_489 = (sub_490 * 0.625);
			double add_488 = (mul_489 + tap_493);
			double sub_483 = (add_488 - mul_484);
			double mul_482 = (sub_483 * 0.625);
			double add_481 = (mul_482 + tap_486);
			double mul_1327 = (add_481 * m_mix_29);
			double out2 = (mul_1327 + in1);
			double add_412 = (add_400 + mix_341);
			double sub_525 = (add_412 - mul_526);
			double mul_524 = (sub_525 * 0.75);
			double add_523 = (mul_524 + tap_528);
			double sub_516 = (add_523 - mul_517);
			double mul_515 = (sub_516 * 0.625);
			double add_514 = (mul_515 + tap_519);
			double sub_509 = (add_514 - mul_510);
			double mul_508 = (sub_509 * 0.625);
			double add_507 = (mul_508 + tap_512);
			double mul_411 = (add_507 * m_mix_29);
			double out1 = (in1 + mul_411);
			double y2_next_1492 = m_y_2;
			double history_444_next_1493 = mix_445;
			double history_438_next_1494 = mix_439;
			double history_450_next_1495 = mix_451;
			double history_546_next_1496 = mix_547;
			double history_905_next_1497 = sah_904;
			double history_909_next_1498 = sah_908;
			double history_912_next_1499 = sah_910;
			double history_907_next_1500 = sah_906;
			double y1_next_1501 = sub_1002;
			double history_550_next_1502 = mix_551;
			m_delay_24.write(add_433);
			m_delay_23.write(add_432);
			m_delay_22.write(add_434);
			m_delay_21.write(add_435);
			m_delay_20.write(sub_483);
			m_delay_19.write(sub_509);
			m_delay_18.write(in1);
			m_delay_17.write(add_530);
			m_delay_16.write(sub_532);
			m_delay_15.write(sub_490);
			m_delay_14.write(sub_516);
			m_delay_13.write(sub_497);
			m_delay_12.write(sub_525);
			m_y_11 = y2_next_1492;
			m_history_10 = history_444_next_1493;
			m_history_9 = history_438_next_1494;
			m_history_8 = history_450_next_1495;
			m_history_7 = history_546_next_1496;
			m_history_6 = history_905_next_1497;
			m_history_5 = history_909_next_1498;
			m_history_4 = history_912_next_1499;
			m_history_3 = history_907_next_1500;
			m_y_2 = y1_next_1501;
			m_history_1 = history_550_next_1502;
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
	inline void set_ratio(double _value) {
		m_ratio_25 = (_value < 0.5 ? 0.5 : (_value > 2 ? 2 : _value));
	};
	inline void set_window(double _value) {
		m_window_26 = (_value < 0.1 ? 0.1 : (_value > 1000 ? 1000 : _value));
	};
	inline void set_resonance(double _value) {
		m_resonance_27 = (_value < 0 ? 0 : (_value > 0.25 ? 0.25 : _value));
	};
	inline void set_tone(double _value) {
		m_tone_28 = (_value < 0 ? 0 : (_value > 1 ? 1 : _value));
	};
	inline void set_mix(double _value) {
		m_mix_29 = (_value < 0 ? 0 : (_value > 1 ? 1 : _value));
	};
	inline void set_spread(double _value) {
		m_spread_30 = (_value < 0 ? 0 : (_value > 100 ? 100 : _value));
	};
	inline void set_cutoff(double _value) {
		m_cutoff_31 = (_value < 0 ? 0 : (_value > 6000 ? 6000 : _value));
	};
	inline void set_early(double _value) {
		m_early_32 = (_value < 0 ? 0 : (_value > 1 ? 1 : _value));
	};
	inline void set_shimmer(double _value) {
		m_shimmer_33 = (_value < 0 ? 0 : (_value > 1 ? 1 : _value));
	};
	inline void set_roomsize(double _value) {
		m_roomsize_34 = (_value < 0.1 ? 0.1 : (_value > 300 ? 300 : _value));
	};
	inline void set_tail(double _value) {
		m_tail_35 = (_value < 0 ? 0 : (_value > 1 ? 1 : _value));
	};
	inline void set_blur(double _value) {
		m_blur_36 = (_value < 0.01 ? 0.01 : (_value > 0.25 ? 0.25 : _value));
	};
	inline void set_decay(double _value) {
		m_decay_37 = (_value < 0.1 ? 0.1 : (_value > 360 ? 360 : _value));
	};
	inline void set_damping(double _value) {
		m_damping_38 = (_value < 0 ? 0 : (_value > 1 ? 1 : _value));
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
int num_params() { return 14; }

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
		case 2: self->set_resonance(value); break;
		case 3: self->set_tone(value); break;
		case 4: self->set_mix(value); break;
		case 5: self->set_spread(value); break;
		case 6: self->set_cutoff(value); break;
		case 7: self->set_early(value); break;
		case 8: self->set_shimmer(value); break;
		case 9: self->set_roomsize(value); break;
		case 10: self->set_tail(value); break;
		case 11: self->set_blur(value); break;
		case 12: self->set_decay(value); break;
		case 13: self->set_damping(value); break;
		
		default: break;
	}
}

/// Get the value of a parameter of a State object 

void getparameter(CommonState *cself, long index, double *value) {
	State *self = (State *)cself;
	switch (index) {
		case 0: *value = self->m_ratio_25; break;
		case 1: *value = self->m_window_26; break;
		case 2: *value = self->m_resonance_27; break;
		case 3: *value = self->m_tone_28; break;
		case 4: *value = self->m_mix_29; break;
		case 5: *value = self->m_spread_30; break;
		case 6: *value = self->m_cutoff_31; break;
		case 7: *value = self->m_early_32; break;
		case 8: *value = self->m_shimmer_33; break;
		case 9: *value = self->m_roomsize_34; break;
		case 10: *value = self->m_tail_35; break;
		case 11: *value = self->m_blur_36; break;
		case 12: *value = self->m_decay_37; break;
		case 13: *value = self->m_damping_38; break;
		
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
	self->__commonstate.params = (ParamInfo *)genlib_sysmem_newptr(14 * sizeof(ParamInfo));
	self->__commonstate.numparams = 14;
	// initialize parameter 0 ("m_ratio_25")
	pi = self->__commonstate.params + 0;
	pi->name = "ratio";
	pi->paramtype = GENLIB_PARAMTYPE_FLOAT;
	pi->defaultvalue = self->m_ratio_25;
	pi->defaultref = 0;
	pi->hasinputminmax = false;
	pi->inputmin = 0; 
	pi->inputmax = 1;
	pi->hasminmax = true;
	pi->outputmin = 0.5;
	pi->outputmax = 2;
	pi->exp = 0;
	pi->units = "";		// no units defined
	// initialize parameter 1 ("m_window_26")
	pi = self->__commonstate.params + 1;
	pi->name = "window";
	pi->paramtype = GENLIB_PARAMTYPE_FLOAT;
	pi->defaultvalue = self->m_window_26;
	pi->defaultref = 0;
	pi->hasinputminmax = false;
	pi->inputmin = 0; 
	pi->inputmax = 1;
	pi->hasminmax = true;
	pi->outputmin = 0.1;
	pi->outputmax = 1000;
	pi->exp = 0;
	pi->units = "";		// no units defined
	// initialize parameter 2 ("m_resonance_27")
	pi = self->__commonstate.params + 2;
	pi->name = "resonance";
	pi->paramtype = GENLIB_PARAMTYPE_FLOAT;
	pi->defaultvalue = self->m_resonance_27;
	pi->defaultref = 0;
	pi->hasinputminmax = false;
	pi->inputmin = 0; 
	pi->inputmax = 1;
	pi->hasminmax = true;
	pi->outputmin = 0;
	pi->outputmax = 0.25;
	pi->exp = 0;
	pi->units = "";		// no units defined
	// initialize parameter 3 ("m_tone_28")
	pi = self->__commonstate.params + 3;
	pi->name = "tone";
	pi->paramtype = GENLIB_PARAMTYPE_FLOAT;
	pi->defaultvalue = self->m_tone_28;
	pi->defaultref = 0;
	pi->hasinputminmax = false;
	pi->inputmin = 0; 
	pi->inputmax = 1;
	pi->hasminmax = true;
	pi->outputmin = 0;
	pi->outputmax = 1;
	pi->exp = 0;
	pi->units = "";		// no units defined
	// initialize parameter 4 ("m_mix_29")
	pi = self->__commonstate.params + 4;
	pi->name = "mix";
	pi->paramtype = GENLIB_PARAMTYPE_FLOAT;
	pi->defaultvalue = self->m_mix_29;
	pi->defaultref = 0;
	pi->hasinputminmax = false;
	pi->inputmin = 0; 
	pi->inputmax = 1;
	pi->hasminmax = true;
	pi->outputmin = 0;
	pi->outputmax = 1;
	pi->exp = 0;
	pi->units = "";		// no units defined
	// initialize parameter 5 ("m_spread_30")
	pi = self->__commonstate.params + 5;
	pi->name = "spread";
	pi->paramtype = GENLIB_PARAMTYPE_FLOAT;
	pi->defaultvalue = self->m_spread_30;
	pi->defaultref = 0;
	pi->hasinputminmax = false;
	pi->inputmin = 0; 
	pi->inputmax = 1;
	pi->hasminmax = true;
	pi->outputmin = 0;
	pi->outputmax = 100;
	pi->exp = 0;
	pi->units = "";		// no units defined
	// initialize parameter 6 ("m_cutoff_31")
	pi = self->__commonstate.params + 6;
	pi->name = "cutoff";
	pi->paramtype = GENLIB_PARAMTYPE_FLOAT;
	pi->defaultvalue = self->m_cutoff_31;
	pi->defaultref = 0;
	pi->hasinputminmax = false;
	pi->inputmin = 0; 
	pi->inputmax = 1;
	pi->hasminmax = true;
	pi->outputmin = 0;
	pi->outputmax = 6000;
	pi->exp = 0;
	pi->units = "";		// no units defined
	// initialize parameter 7 ("m_early_32")
	pi = self->__commonstate.params + 7;
	pi->name = "early";
	pi->paramtype = GENLIB_PARAMTYPE_FLOAT;
	pi->defaultvalue = self->m_early_32;
	pi->defaultref = 0;
	pi->hasinputminmax = false;
	pi->inputmin = 0; 
	pi->inputmax = 1;
	pi->hasminmax = true;
	pi->outputmin = 0;
	pi->outputmax = 1;
	pi->exp = 0;
	pi->units = "";		// no units defined
	// initialize parameter 8 ("m_shimmer_33")
	pi = self->__commonstate.params + 8;
	pi->name = "shimmer";
	pi->paramtype = GENLIB_PARAMTYPE_FLOAT;
	pi->defaultvalue = self->m_shimmer_33;
	pi->defaultref = 0;
	pi->hasinputminmax = false;
	pi->inputmin = 0; 
	pi->inputmax = 1;
	pi->hasminmax = true;
	pi->outputmin = 0;
	pi->outputmax = 1;
	pi->exp = 0;
	pi->units = "";		// no units defined
	// initialize parameter 9 ("m_roomsize_34")
	pi = self->__commonstate.params + 9;
	pi->name = "roomsize";
	pi->paramtype = GENLIB_PARAMTYPE_FLOAT;
	pi->defaultvalue = self->m_roomsize_34;
	pi->defaultref = 0;
	pi->hasinputminmax = false;
	pi->inputmin = 0; 
	pi->inputmax = 1;
	pi->hasminmax = true;
	pi->outputmin = 0.1;
	pi->outputmax = 300;
	pi->exp = 0;
	pi->units = "";		// no units defined
	// initialize parameter 10 ("m_tail_35")
	pi = self->__commonstate.params + 10;
	pi->name = "tail";
	pi->paramtype = GENLIB_PARAMTYPE_FLOAT;
	pi->defaultvalue = self->m_tail_35;
	pi->defaultref = 0;
	pi->hasinputminmax = false;
	pi->inputmin = 0; 
	pi->inputmax = 1;
	pi->hasminmax = true;
	pi->outputmin = 0;
	pi->outputmax = 1;
	pi->exp = 0;
	pi->units = "";		// no units defined
	// initialize parameter 11 ("m_blur_36")
	pi = self->__commonstate.params + 11;
	pi->name = "blur";
	pi->paramtype = GENLIB_PARAMTYPE_FLOAT;
	pi->defaultvalue = self->m_blur_36;
	pi->defaultref = 0;
	pi->hasinputminmax = false;
	pi->inputmin = 0; 
	pi->inputmax = 1;
	pi->hasminmax = true;
	pi->outputmin = 0.01;
	pi->outputmax = 0.25;
	pi->exp = 0;
	pi->units = "";		// no units defined
	// initialize parameter 12 ("m_decay_37")
	pi = self->__commonstate.params + 12;
	pi->name = "decay";
	pi->paramtype = GENLIB_PARAMTYPE_FLOAT;
	pi->defaultvalue = self->m_decay_37;
	pi->defaultref = 0;
	pi->hasinputminmax = false;
	pi->inputmin = 0; 
	pi->inputmax = 1;
	pi->hasminmax = true;
	pi->outputmin = 0.1;
	pi->outputmax = 360;
	pi->exp = 0;
	pi->units = "";		// no units defined
	// initialize parameter 13 ("m_damping_38")
	pi = self->__commonstate.params + 13;
	pi->name = "damping";
	pi->paramtype = GENLIB_PARAMTYPE_FLOAT;
	pi->defaultvalue = self->m_damping_38;
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
