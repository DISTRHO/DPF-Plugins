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
	Delay m_delay_11;
	Delay m_delay_9;
	Delay m_delay_8;
	Delay m_delay_7;
	Delay m_delay_10;
	Delay m_delay_12;
	Delay m_delay_16;
	Delay m_delay_13;
	Delay m_delay_14;
	Delay m_delay_6;
	Delay m_delay_15;
	Delay m_delay_17;
	double m_bandwidth_22;
	double m_tail_23;
	double m_spread_21;
	double m_revtime_19;
	double m_roomsize_20;
	double m_damping_18;
	double m_early_25;
	double m_history_5;
	double m_dry_24;
	double samplerate;
	double m_history_2;
	double m_history_1;
	double m_history_4;
	double m_history_3;
	int vectorsize;
	int __exception;
	// re-initialize all member variables;
	inline void reset(double __sr, int __vs) { 
		__exception = 0;
		vectorsize = __vs;
		samplerate = __sr;
		m_history_1 = 0;
		m_history_2 = 0;
		m_history_3 = 0;
		m_history_4 = 0;
		m_history_5 = 0;
		m_delay_6.reset("m_delay_6", 5000);
		m_delay_7.reset("m_delay_7", 7000);
		m_delay_8.reset("m_delay_8", 15000);
		m_delay_9.reset("m_delay_9", 6000);
		m_delay_10.reset("m_delay_10", 16000);
		m_delay_11.reset("m_delay_11", 48000);
		m_delay_12.reset("m_delay_12", 10000);
		m_delay_13.reset("m_delay_13", 12000);
		m_delay_14.reset("m_delay_14", 48000);
		m_delay_15.reset("m_delay_15", 48000);
		m_delay_16.reset("m_delay_16", 48000);
		m_delay_17.reset("m_delay_17", 48000);
		m_damping_18 = 0.7;
		m_revtime_19 = 11;
		m_roomsize_20 = 75;
		m_spread_21 = 23;
		m_bandwidth_22 = 0.5;
		m_tail_23 = 0.25;
		m_dry_24 = 1;
		m_early_25 = 0.25;
		genlib_reset_complete(this);
		
	};
	// the signal processing routine;
	inline int perform(t_sample ** __ins, t_sample ** __outs, int __n) { 
		vectorsize = __n;
		const t_sample * __in1 = __ins[0];
		const t_sample * __in2 = __ins[1];
		t_sample * __out1 = __outs[0];
		t_sample * __out2 = __outs[1];
		if (__exception) { 
			return __exception;
			
		} else if (( (__in1 == 0) || (__in2 == 0) || (__out1 == 0) || (__out2 == 0) )) { 
			__exception = GENLIB_ERR_NULL_BUFFER;
			return __exception;
			
		};
		double rsub_999 = (1 - m_bandwidth_22);
		double expr_1051 = safepow(0.001, safediv(1, (m_revtime_19 * 44100)));
		double expr_1052 = safediv((m_roomsize_20 * 44100), 340);
		double mul_988 = (expr_1052 * 0.63245);
		double expr_1043 = (-safepow(expr_1051, mul_988));
		double mul_990 = (expr_1052 * 0.81649);
		double expr_1045 = (-safepow(expr_1051, mul_990));
		double mul_989 = (expr_1052 * 0.7071);
		double expr_1044 = (-safepow(expr_1051, mul_989));
		double mul_991 = (expr_1052 * 1);
		double expr_1050 = (-safepow(expr_1051, mul_991));
		double mul_985 = (expr_1052 * 0.000527);
		int int_984 = int(mul_985);
		double mul_925 = (m_spread_21 * -0.380445);
		double add_924 = (mul_925 + 931);
		double rsub_921 = (1341 - add_924);
		double mul_934 = (int_984 * rsub_921);
		double mul_953 = (m_spread_21 * 0.376623);
		double add_952 = (mul_953 + 931);
		double rsub_949 = (1341 - add_952);
		double mul_960 = (int_984 * rsub_949);
		double add_914 = (expr_1052 + 5);
		double expr_1046 = safepow(expr_1051, add_914);
		double mul_920 = (expr_1052 * 0.41);
		double add_917 = (mul_920 + 5);
		double expr_1049 = safepow(expr_1051, add_917);
		double mul_919 = (expr_1052 * 0.3);
		double add_916 = (mul_919 + 5);
		double expr_1048 = safepow(expr_1051, add_916);
		double mul_918 = (expr_1052 * 0.155);
		double add_915 = (mul_918 + 5);
		double expr_1047 = safepow(expr_1051, add_915);
		double mul_927 = (m_spread_21 * -0.568366);
		double add_923 = (mul_927 + 369);
		double rsub_922 = (add_924 - add_923);
		double mul_941 = (int_984 * rsub_922);
		double mul_983 = (expr_1052 * 0.110732);
		double mul_969 = (m_spread_21 * 0.125541);
		double add_951 = (mul_969 + 369);
		double rsub_950 = (add_952 - add_951);
		double mul_967 = (int_984 * rsub_950);
		double add_926 = (mul_927 + 159);
		double mul_948 = (int_984 * add_926);
		double add_968 = (mul_969 + 159);
		double mul_976 = (int_984 * add_968);
		// the main sample loop;
		while ((__n--)) { 
			const double in1 = (*(__in1++));
			const double in2 = (*(__in2++));
			double mul_846 = (in2 * m_dry_24);
			double mul_858 = (in1 * m_dry_24);
			double mul_848 = ((in1 + in2) * 0.707);
			double mix_1070 = (mul_848 + (rsub_999 * (m_history_5 - mul_848)));
			double mix_998 = mix_1070;
			double tap_888 = m_delay_17.read_linear(mul_988);
			double mul_884 = (tap_888 * expr_1043);
			double mix_1071 = (mul_884 + (m_damping_18 * (m_history_4 - mul_884)));
			double mix_886 = mix_1071;
			double tap_900 = m_delay_16.read_linear(mul_990);
			double mul_896 = (tap_900 * expr_1045);
			double mix_1072 = (mul_896 + (m_damping_18 * (m_history_3 - mul_896)));
			double mix_898 = mix_1072;
			double tap_894 = m_delay_15.read_linear(mul_989);
			double mul_890 = (tap_894 * expr_1044);
			double mix_1073 = (mul_890 + (m_damping_18 * (m_history_2 - mul_890)));
			double mix_892 = mix_1073;
			double tap_996 = m_delay_14.read_linear(mul_991);
			double mul_987 = (tap_996 * expr_1050);
			double mix_1074 = (mul_987 + (m_damping_18 * (m_history_1 - mul_987)));
			double mix_994 = mix_1074;
			double tap_933 = m_delay_13.read_linear(mul_934);
			double mul_931 = (tap_933 * 0.625);
			double tap_959 = m_delay_12.read_linear(mul_960);
			double mul_957 = (tap_959 * 0.625);
			double add_878 = (mix_994 + mix_898);
			double add_876 = (mix_892 + mix_886);
			double add_871 = (add_878 + add_876);
			double mul_854 = (add_871 * 0.5);
			double sub_875 = (add_878 - add_876);
			double mul_857 = (sub_875 * 0.5);
			double sub_877 = (mix_994 - mix_898);
			double sub_874 = (mix_892 - mix_886);
			double sub_873 = (sub_877 - sub_874);
			double mul_856 = (sub_873 * 0.5);
			double add_872 = (sub_877 + sub_874);
			double rsub_870 = (0 - add_872);
			double mul_855 = (rsub_870 * 0.5);
			double tap_902 = m_delay_11.read_linear(add_917);
			double tap_903 = m_delay_11.read_linear(add_916);
			double tap_904 = m_delay_11.read_linear(add_915);
			double tap_905 = m_delay_11.read_linear(add_914);
			double mul_906 = (tap_905 * expr_1046);
			double add_879 = (mul_854 + mul_906);
			double mul_910 = (tap_903 * expr_1048);
			double add_881 = (mul_856 + mul_910);
			double mul_912 = (tap_902 * expr_1049);
			double add_882 = (mul_857 + mul_912);
			double mul_908 = (tap_904 * expr_1047);
			double add_880 = (mul_855 + mul_908);
			double tap_940 = m_delay_10.read_linear(mul_941);
			double mul_938 = (tap_940 * 0.625);
			double tap_982 = m_delay_9.read_linear(mul_983);
			double tap_966 = m_delay_8.read_linear(mul_967);
			double mul_964 = (tap_966 * 0.625);
			double tap_947 = m_delay_7.read_linear(mul_948);
			double mul_945 = (tap_947 * 0.75);
			double mul_980 = (tap_982 * 0.75);
			double sub_979 = (mix_998 - mul_980);
			double mul_978 = (sub_979 * 0.75);
			double add_977 = (mul_978 + tap_982);
			double tap_975 = m_delay_6.read_linear(mul_976);
			double mul_973 = (tap_975 * 0.75);
			double mul_869 = (mul_857 * m_tail_23);
			double mul_867 = (mul_855 * m_tail_23);
			double add_853 = (mul_869 + mul_867);
			double mul_868 = (mul_856 * m_tail_23);
			double mul_866 = (mul_854 * m_tail_23);
			double add_852 = (mul_868 + mul_866);
			double sub_861 = (add_853 - add_852);
			double mul_865 = (mul_912 * m_early_25);
			double mul_863 = (mul_908 * m_early_25);
			double add_851 = (mul_865 + mul_863);
			double mul_864 = (mul_910 * m_early_25);
			double mul_862 = (mul_906 * m_early_25);
			double add_850 = (mul_864 + mul_862);
			double sub_860 = (add_851 - add_850);
			double add_847 = (sub_861 + sub_860);
			double add_849 = (add_847 + in2);
			double sub_944 = (add_849 - mul_945);
			double mul_943 = (sub_944 * 0.75);
			double add_942 = (mul_943 + tap_947);
			double sub_937 = (add_942 - mul_938);
			double mul_936 = (sub_937 * 0.625);
			double add_935 = (mul_936 + tap_940);
			double sub_930 = (add_935 - mul_931);
			double mul_929 = (sub_930 * 0.625);
			double add_928 = (mul_929 + tap_933);
			double out2 = (mul_846 + add_928);
			double add_859 = (add_847 + in1);
			double sub_972 = (add_859 - mul_973);
			double mul_971 = (sub_972 * 0.75);
			double add_970 = (mul_971 + tap_975);
			double sub_963 = (add_970 - mul_964);
			double mul_962 = (sub_963 * 0.625);
			double add_961 = (mul_962 + tap_966);
			double sub_956 = (add_961 - mul_957);
			double mul_955 = (sub_956 * 0.625);
			double add_954 = (mul_955 + tap_959);
			double out1 = (mul_858 + add_954);
			double history_997_next_1065 = mix_998;
			double history_885_next_1066 = mix_886;
			double history_897_next_1067 = mix_898;
			double history_891_next_1068 = mix_892;
			double history_993_next_1069 = mix_994;
			m_delay_17.write(add_879);
			m_delay_16.write(add_881);
			m_delay_15.write(add_880);
			m_delay_14.write(add_882);
			m_delay_13.write(sub_930);
			m_delay_12.write(sub_956);
			m_delay_11.write(add_977);
			m_delay_10.write(sub_937);
			m_delay_9.write(sub_979);
			m_delay_8.write(sub_963);
			m_delay_7.write(sub_944);
			m_delay_6.write(sub_972);
			m_history_5 = history_997_next_1065;
			m_history_4 = history_885_next_1066;
			m_history_3 = history_897_next_1067;
			m_history_2 = history_891_next_1068;
			m_history_1 = history_993_next_1069;
			m_delay_6.step();
			m_delay_7.step();
			m_delay_8.step();
			m_delay_9.step();
			m_delay_10.step();
			m_delay_11.step();
			m_delay_12.step();
			m_delay_13.step();
			m_delay_14.step();
			m_delay_15.step();
			m_delay_16.step();
			m_delay_17.step();
			// assign results to output buffer;
			(*(__out1++)) = out1;
			(*(__out2++)) = out2;
			
		};
		return __exception;
		
	};
	inline void set_damping(double _value) {
		m_damping_18 = (_value < 0 ? 0 : (_value > 1 ? 1 : _value));
	};
	inline void set_revtime(double _value) {
		m_revtime_19 = (_value < 0.1 ? 0.1 : (_value > 360 ? 360 : _value));
	};
	inline void set_roomsize(double _value) {
		m_roomsize_20 = (_value < 0.1 ? 0.1 : (_value > 300 ? 300 : _value));
	};
	inline void set_spread(double _value) {
		m_spread_21 = (_value < 0 ? 0 : (_value > 100 ? 100 : _value));
	};
	inline void set_bandwidth(double _value) {
		m_bandwidth_22 = (_value < 0 ? 0 : (_value > 1 ? 1 : _value));
	};
	inline void set_tail(double _value) {
		m_tail_23 = (_value < 0 ? 0 : (_value > 1 ? 1 : _value));
	};
	inline void set_dry(double _value) {
		m_dry_24 = (_value < 0 ? 0 : (_value > 1 ? 1 : _value));
	};
	inline void set_early(double _value) {
		m_early_25 = (_value < 0 ? 0 : (_value > 1 ? 1 : _value));
	};
	
} State;


/// 
///	Configuration for the genlib API
///

/// Number of signal inputs and outputs 

int gen_kernel_numins = 2;
int gen_kernel_numouts = 2;

int num_inputs() { return gen_kernel_numins; }
int num_outputs() { return gen_kernel_numouts; }
int num_params() { return 8; }

/// Assistive lables for the signal inputs and outputs 

const char * gen_kernel_innames[] = { "in1", "in2" };
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
		case 0: self->set_damping(value); break;
		case 1: self->set_revtime(value); break;
		case 2: self->set_roomsize(value); break;
		case 3: self->set_spread(value); break;
		case 4: self->set_bandwidth(value); break;
		case 5: self->set_tail(value); break;
		case 6: self->set_dry(value); break;
		case 7: self->set_early(value); break;
		
		default: break;
	}
}

/// Get the value of a parameter of a State object 

void getparameter(CommonState *cself, long index, double *value) {
	State *self = (State *)cself;
	switch (index) {
		case 0: *value = self->m_damping_18; break;
		case 1: *value = self->m_revtime_19; break;
		case 2: *value = self->m_roomsize_20; break;
		case 3: *value = self->m_spread_21; break;
		case 4: *value = self->m_bandwidth_22; break;
		case 5: *value = self->m_tail_23; break;
		case 6: *value = self->m_dry_24; break;
		case 7: *value = self->m_early_25; break;
		
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
	// initialize parameter 0 ("m_damping_18")
	pi = self->__commonstate.params + 0;
	pi->name = "damping";
	pi->paramtype = GENLIB_PARAMTYPE_FLOAT;
	pi->defaultvalue = self->m_damping_18;
	pi->defaultref = 0;
	pi->hasinputminmax = false;
	pi->inputmin = 0; 
	pi->inputmax = 1;
	pi->hasminmax = true;
	pi->outputmin = 0;
	pi->outputmax = 1;
	pi->exp = 0;
	pi->units = "";		// no units defined
	// initialize parameter 1 ("m_revtime_19")
	pi = self->__commonstate.params + 1;
	pi->name = "revtime";
	pi->paramtype = GENLIB_PARAMTYPE_FLOAT;
	pi->defaultvalue = self->m_revtime_19;
	pi->defaultref = 0;
	pi->hasinputminmax = false;
	pi->inputmin = 0; 
	pi->inputmax = 1;
	pi->hasminmax = true;
	pi->outputmin = 0.1;
	pi->outputmax = 360;
	pi->exp = 0;
	pi->units = "";		// no units defined
	// initialize parameter 2 ("m_roomsize_20")
	pi = self->__commonstate.params + 2;
	pi->name = "roomsize";
	pi->paramtype = GENLIB_PARAMTYPE_FLOAT;
	pi->defaultvalue = self->m_roomsize_20;
	pi->defaultref = 0;
	pi->hasinputminmax = false;
	pi->inputmin = 0; 
	pi->inputmax = 1;
	pi->hasminmax = true;
	pi->outputmin = 0.1;
	pi->outputmax = 300;
	pi->exp = 0;
	pi->units = "";		// no units defined
	// initialize parameter 3 ("m_spread_21")
	pi = self->__commonstate.params + 3;
	pi->name = "spread";
	pi->paramtype = GENLIB_PARAMTYPE_FLOAT;
	pi->defaultvalue = self->m_spread_21;
	pi->defaultref = 0;
	pi->hasinputminmax = false;
	pi->inputmin = 0; 
	pi->inputmax = 1;
	pi->hasminmax = true;
	pi->outputmin = 0;
	pi->outputmax = 100;
	pi->exp = 0;
	pi->units = "";		// no units defined
	// initialize parameter 4 ("m_bandwidth_22")
	pi = self->__commonstate.params + 4;
	pi->name = "bandwidth";
	pi->paramtype = GENLIB_PARAMTYPE_FLOAT;
	pi->defaultvalue = self->m_bandwidth_22;
	pi->defaultref = 0;
	pi->hasinputminmax = false;
	pi->inputmin = 0; 
	pi->inputmax = 1;
	pi->hasminmax = true;
	pi->outputmin = 0;
	pi->outputmax = 1;
	pi->exp = 0;
	pi->units = "";		// no units defined
	// initialize parameter 5 ("m_tail_23")
	pi = self->__commonstate.params + 5;
	pi->name = "tail";
	pi->paramtype = GENLIB_PARAMTYPE_FLOAT;
	pi->defaultvalue = self->m_tail_23;
	pi->defaultref = 0;
	pi->hasinputminmax = false;
	pi->inputmin = 0; 
	pi->inputmax = 1;
	pi->hasminmax = true;
	pi->outputmin = 0;
	pi->outputmax = 1;
	pi->exp = 0;
	pi->units = "";		// no units defined
	// initialize parameter 6 ("m_dry_24")
	pi = self->__commonstate.params + 6;
	pi->name = "dry";
	pi->paramtype = GENLIB_PARAMTYPE_FLOAT;
	pi->defaultvalue = self->m_dry_24;
	pi->defaultref = 0;
	pi->hasinputminmax = false;
	pi->inputmin = 0; 
	pi->inputmax = 1;
	pi->hasminmax = true;
	pi->outputmin = 0;
	pi->outputmax = 1;
	pi->exp = 0;
	pi->units = "";		// no units defined
	// initialize parameter 7 ("m_early_25")
	pi = self->__commonstate.params + 7;
	pi->name = "early";
	pi->paramtype = GENLIB_PARAMTYPE_FLOAT;
	pi->defaultvalue = self->m_early_25;
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
