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
	Delta m_delta_38;
	Delta m_delta_35;
	Delta m_delta_41;
	Delta m_delta_32;
	Phasor m_phasor_31;
	Sah m_sah_36;
	Sah m_sah_34;
	Sah m_sah_37;
	Sah m_sah_40;
	Sah m_sah_39;
	Sah m_sah_33;
	Sah m_sah_43;
	Sah m_sah_42;
	double m_history_4;
	double m_history_5;
	double m_history_6;
	double m_history_7;
	double m_history_3;
	double m_history_1;
	double m_y_2;
	double samplerate;
	double m_history_10;
	double m_history_8;
	double samples_to_seconds;
	double m_history_9;
	double m_shimmer_25;
	double m_damping_24;
	double m_ratio_27;
	double m_roomsize_26;
	double m_tone_29;
	double m_mix_30;
	double m_decay_28;
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
		m_delay_11.reset("m_delay_11", 7000);
		m_delay_12.reset("m_delay_12", 5000);
		m_delay_13.reset("m_delay_13", 6000);
		m_delay_14.reset("m_delay_14", 48000);
		m_delay_15.reset("m_delay_15", 16000);
		m_delay_16.reset("m_delay_16", 96000);
		m_delay_17.reset("m_delay_17", 15000);
		m_delay_18.reset("m_delay_18", 12000);
		m_delay_19.reset("m_delay_19", 10000);
		m_delay_20.reset("m_delay_20", 48000);
		m_delay_21.reset("m_delay_21", 48000);
		m_delay_22.reset("m_delay_22", 48000);
		m_delay_23.reset("m_delay_23", 48000);
		m_damping_24 = 75;
		m_shimmer_25 = 50;
		m_roomsize_26 = 100;
		m_ratio_27 = 2;
		m_decay_28 = 50;
		m_tone_29 = 3000;
		m_mix_30 = 75;
		samples_to_seconds = (1 / samplerate);
		m_phasor_31.reset(0);
		m_delta_32.reset(0);
		m_sah_33.reset(0);
		m_sah_34.reset(0);
		m_delta_35.reset(0);
		m_sah_36.reset(0);
		m_sah_37.reset(0);
		m_delta_38.reset(0);
		m_sah_39.reset(0);
		m_sah_40.reset(0);
		m_delta_41.reset(0);
		m_sah_42.reset(0);
		m_sah_43.reset(0);
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
		double mul_46984 = (m_mix_30 * 0.01);
		double mul_46983 = (m_damping_24 * 0.01);
		double mul_46982 = (m_shimmer_25 * 0.01);
		double expr_46980 = safediv(((m_tone_29 * 2) * 3.1415926535898), 48000);
		double sin_46767 = sin(expr_46980);
		double clamp_46768 = ((sin_46767 <= 1e-05) ? 1e-05 : ((sin_46767 >= 0.99999) ? 0.99999 : sin_46767));
		double expr_46979 = safediv((m_roomsize_26 * 48000), 340);
		double mul_46949 = (expr_46979 * 0.81649);
		double mul_46950 = (expr_46979 * 1);
		double mul_46948 = (expr_46979 * 0.7071);
		double mul_46947 = (expr_46979 * 0.63245);
		double mul_46959 = (m_decay_28 * 3.6);
		double expr_46978 = safepow(0.001, safediv(1, (mul_46959 * 48000)));
		double expr_46875 = (-safepow(expr_46978, mul_46949));
		double expr_46945 = (-safepow(expr_46978, mul_46950));
		double expr_46870 = (-safepow(expr_46978, mul_46948));
		double expr_46865 = (-safepow(expr_46978, mul_46947));
		double mul_46944 = (expr_46979 * 0.000527);
		int int_46943 = int(mul_46944);
		double mul_46968 = (int_46943 * 400.600006);
		double mul_46971 = (int_46943 * 419.5);
		double mul_46967 = (int_46943 * 568.299988);
		double add_46893 = (expr_46979 + 5);
		double expr_46886 = safepow(expr_46978, add_46893);
		double rsub_46809 = (1 - m_ratio_27);
		double mul_46981 = (rsub_46809 * 10);
		samples_to_seconds = (1 / samplerate);
		double mul_46970 = (int_46943 * 566.700012);
		double mul_46899 = (expr_46979 * 0.41);
		double add_46896 = (mul_46899 + 5);
		double expr_46892 = safepow(expr_46978, add_46896);
		double mul_46898 = (expr_46979 * 0.3);
		double add_46895 = (mul_46898 + 5);
		double expr_46890 = safepow(expr_46978, add_46895);
		double mul_46897 = (expr_46979 * 0.155);
		double add_46894 = (mul_46897 + 5);
		double expr_46888 = safepow(expr_46978, add_46894);
		double mul_46942 = (expr_46979 * 0.110732);
		double mul_46966 = (int_46943 * 162.100006);
		double mul_46969 = (int_46943 * 144.800003);
		double mul_46960 = (m_decay_28 * 0.01);
		// the main sample loop;
		while ((__n--)) { 
			const double in1 = (*(__in1++));
			double noise_46770 = noise();
			double abs_46787 = fabs(noise_46770);
			double mul_46957 = (abs_46787 * 0.25);
			double noise_46772 = noise();
			double abs_46789 = fabs(noise_46772);
			double mul_46955 = (abs_46789 * 0.25);
			double noise_46771 = noise();
			double abs_46788 = fabs(noise_46771);
			double mul_46956 = (abs_46788 * 0.25);
			double noise_46769 = noise();
			double abs_46786 = fabs(noise_46769);
			double mul_46958 = (abs_46786 * 0.25);
			double tap_46879 = m_delay_23.read_linear(mul_46949);
			double tap_46953 = m_delay_22.read_linear(mul_46950);
			double tap_46874 = m_delay_21.read_linear(mul_46948);
			double tap_46869 = m_delay_20.read_linear(mul_46947);
			double mul_46876 = (tap_46879 * expr_46875);
			double mix_47035 = (mul_46876 + (mul_46983 * (m_history_10 - mul_46876)));
			double mix_46976 = mix_47035;
			double mul_46946 = (tap_46953 * expr_46945);
			double mix_47036 = (mul_46946 + (mul_46983 * (m_history_9 - mul_46946)));
			double mix_46977 = mix_47036;
			double mul_46871 = (tap_46874 * expr_46870);
			double mix_47037 = (mul_46871 + (mul_46983 * (m_history_8 - mul_46871)));
			double mix_46975 = mix_47037;
			double mul_46866 = (tap_46869 * expr_46865);
			double mix_47038 = (mul_46866 + (mul_46983 * (m_history_7 - mul_46866)));
			double mix_46974 = mix_47038;
			double tap_46923 = m_delay_19.read_linear(mul_46968);
			double mul_46921 = (tap_46923 * 0.625);
			double tap_46905 = m_delay_18.read_linear(mul_46971);
			double mul_46903 = (tap_46905 * 0.625);
			double tap_46929 = m_delay_17.read_linear(mul_46967);
			double sub_46859 = (mix_46977 - mix_46976);
			double sub_46856 = (mix_46975 - mix_46974);
			double sub_46855 = (sub_46859 - sub_46856);
			double mul_46838 = (sub_46855 * 0.5);
			double phasor_46827 = m_phasor_31(mul_46981, samples_to_seconds);
			double add_46808 = ((m_history_6 + phasor_46827) + 0.5);
			double mod_46807 = safemod(add_46808, 1);
			double delta_46791 = m_delta_32(mod_46807);
			double sah_46775 = m_sah_33(mul_46957, delta_46791, 0);
			int sah_46963 = m_sah_34(4800, delta_46791, 0);
			double mul_46783 = (sah_46963 * mod_46807);
			double sub_46806 = (mod_46807 - 0.5);
			double mul_46805 = (sub_46806 * 3.1415926535898);
			double cos_46804 = cos(mul_46805);
			double mul_46794 = (cos_46804 * cos_46804);
			double add_46826 = ((m_history_5 + phasor_46827) + 0);
			double mod_46825 = safemod(add_46826, 1);
			double delta_46780 = m_delta_35(mod_46825);
			double sah_46779 = m_sah_36(mul_46955, delta_46780, 0);
			int sah_46961 = m_sah_37(4800, delta_46780, 0);
			double mul_46785 = (sah_46961 * mod_46825);
			double sub_46824 = (mod_46825 - 0.5);
			double mul_46823 = (sub_46824 * 3.1415926535898);
			double cos_46822 = cos(mul_46823);
			double mul_46796 = (cos_46822 * cos_46822);
			double add_46821 = ((m_history_4 + phasor_46827) + 0.25);
			double mod_46820 = safemod(add_46821, 1);
			double delta_46792 = m_delta_38(mod_46820);
			double sah_46777 = m_sah_39(mul_46956, delta_46792, 0);
			int sah_46962 = m_sah_40(4800, delta_46792, 0);
			double mul_46784 = (sah_46962 * mod_46820);
			double sub_46819 = (mod_46820 - 0.5);
			double mul_46818 = (sub_46819 * 3.1415926535898);
			double cos_46817 = cos(mul_46818);
			double mul_46795 = (cos_46817 * cos_46817);
			double add_46802 = ((m_history_3 + phasor_46827) + 0.75);
			double mod_46801 = safemod(add_46802, 1);
			double delta_46790 = m_delta_41(mod_46801);
			double sah_46773 = m_sah_42(mul_46958, delta_46790, 0);
			int sah_46964 = m_sah_43(4800, delta_46790, 0);
			double mul_46782 = (sah_46964 * mod_46801);
			double tap_46813 = m_delay_16.read_linear(mul_46785);
			double tap_46814 = m_delay_16.read_linear(mul_46784);
			double tap_46815 = m_delay_16.read_linear(mul_46783);
			double tap_46816 = m_delay_16.read_linear(mul_46782);
			double mul_46803 = (tap_46815 * mul_46794);
			double mul_46810 = (tap_46814 * mul_46795);
			double mul_46811 = (tap_46813 * mul_46796);
			double sub_46800 = (mod_46801 - 0.5);
			double mul_46799 = (sub_46800 * 3.1415926535898);
			double cos_46798 = cos(mul_46799);
			double mul_46793 = (cos_46798 * cos_46798);
			double mul_46797 = (tap_46816 * mul_46793);
			double add_47040 = (((mul_46811 + mul_46810) + mul_46803) + mul_46797);
			double mix_47039 = (m_y_2 + (clamp_46768 * (add_47040 - m_y_2)));
			double mix_46766 = mix_47039;
			double mix_47041 = (in1 + (mul_46982 * (mix_46766 - in1)));
			double mix_46973 = mix_47041;
			double mul_46830 = (mix_46973 * 0.707);
			double mix_47042 = (mul_46830 + (0.5 * (m_history_1 - mul_46830)));
			double mix_46965 = mix_47042;
			double tap_46911 = m_delay_15.read_linear(mul_46970);
			double add_46860 = (mix_46977 + mix_46976);
			double add_46858 = (mix_46975 + mix_46974);
			double sub_46857 = (add_46860 - add_46858);
			double mul_46839 = (sub_46857 * 0.5);
			double add_46853 = (add_46860 + add_46858);
			double mul_46836 = (add_46853 * 0.5);
			double add_46854 = (sub_46859 + sub_46856);
			double rsub_46852 = (0 - add_46854);
			double mul_46837 = (rsub_46852 * 0.5);
			double mul_46927 = (tap_46929 * 0.625);
			double tap_46881 = m_delay_14.read_linear(add_46896);
			double tap_46882 = m_delay_14.read_linear(add_46895);
			double tap_46883 = m_delay_14.read_linear(add_46894);
			double tap_46884 = m_delay_14.read_linear(add_46893);
			double mul_46889 = (tap_46882 * expr_46890);
			double add_46863 = (mul_46838 + mul_46889);
			double mul_46891 = (tap_46881 * expr_46892);
			double add_46864 = (mul_46839 + mul_46891);
			double mul_46885 = (tap_46884 * expr_46886);
			double add_46861 = (mul_46836 + mul_46885);
			double mul_46887 = (tap_46883 * expr_46888);
			double add_46862 = (mul_46837 + mul_46887);
			double mul_46909 = (tap_46911 * 0.625);
			double tap_46941 = m_delay_13.read_linear(mul_46942);
			double tap_46935 = m_delay_12.read_linear(mul_46966);
			double mul_46939 = (tap_46941 * 0.75);
			double sub_46938 = (mix_46965 - mul_46939);
			double mul_46937 = (sub_46938 * 0.75);
			double add_46936 = (mul_46937 + tap_46941);
			double tap_46917 = m_delay_11.read_linear(mul_46969);
			double mul_46933 = (tap_46935 * 0.75);
			double mul_46915 = (tap_46917 * 0.75);
			double mul_46851 = (mul_46839 * mul_46960);
			double mul_46849 = (mul_46837 * mul_46960);
			double add_46835 = (mul_46851 + mul_46849);
			double mul_46850 = (mul_46838 * mul_46960);
			double mul_46848 = (mul_46836 * mul_46960);
			double add_46834 = (mul_46850 + mul_46848);
			double sub_46843 = (add_46835 - add_46834);
			double mul_46847 = (mul_46891 * mul_46960);
			double mul_46845 = (mul_46887 * mul_46960);
			double add_46833 = (mul_46847 + mul_46845);
			double mul_46846 = (mul_46889 * mul_46960);
			double mul_46844 = (mul_46885 * mul_46960);
			double add_46832 = (mul_46846 + mul_46844);
			double sub_46842 = (add_46833 - add_46832);
			double add_46829 = (sub_46843 + sub_46842);
			double add_46841 = (add_46829 + mix_46973);
			double sub_46932 = (add_46841 - mul_46933);
			double mul_46931 = (sub_46932 * 0.75);
			double add_46930 = (mul_46931 + tap_46935);
			double sub_46926 = (add_46930 - mul_46927);
			double mul_46925 = (sub_46926 * 0.625);
			double add_46924 = (mul_46925 + tap_46929);
			double sub_46920 = (add_46924 - mul_46921);
			double mul_46919 = (sub_46920 * 0.625);
			double add_46918 = (mul_46919 + tap_46923);
			double mul_46840 = (add_46918 * mul_46984);
			double out1 = (in1 + mul_46840);
			double add_46831 = (add_46829 + mix_46973);
			double sub_46914 = (add_46831 - mul_46915);
			double mul_46913 = (sub_46914 * 0.75);
			double add_46912 = (mul_46913 + tap_46917);
			double sub_46908 = (add_46912 - mul_46909);
			double mul_46907 = (sub_46908 * 0.625);
			double add_46906 = (mul_46907 + tap_46911);
			double sub_46902 = (add_46906 - mul_46903);
			double mul_46901 = (sub_46902 * 0.625);
			double add_46900 = (mul_46901 + tap_46905);
			double mul_46828 = (add_46900 * mul_46984);
			double out2 = (in1 + mul_46828);
			double history_46877_next_46985 = mix_46976;
			double history_46951_next_46986 = mix_46977;
			double history_46872_next_46987 = mix_46975;
			double history_46867_next_46988 = mix_46974;
			double history_46776_next_46989 = sah_46775;
			double history_46781_next_46990 = sah_46779;
			double history_46778_next_46991 = sah_46777;
			double history_46774_next_46992 = sah_46773;
			double y0_next_46993 = mix_46766;
			double history_46954_next_46994 = mix_46965;
			m_delay_23.write(add_46863);
			m_delay_22.write(add_46864);
			m_delay_21.write(add_46862);
			m_delay_20.write(add_46861);
			m_delay_19.write(sub_46920);
			m_delay_18.write(sub_46902);
			m_delay_17.write(sub_46926);
			m_delay_16.write(in1);
			m_delay_15.write(sub_46908);
			m_delay_14.write(add_46936);
			m_delay_13.write(sub_46938);
			m_delay_12.write(sub_46932);
			m_delay_11.write(sub_46914);
			m_history_10 = history_46877_next_46985;
			m_history_9 = history_46951_next_46986;
			m_history_8 = history_46872_next_46987;
			m_history_7 = history_46867_next_46988;
			m_history_6 = history_46776_next_46989;
			m_history_5 = history_46781_next_46990;
			m_history_4 = history_46778_next_46991;
			m_history_3 = history_46774_next_46992;
			m_y_2 = y0_next_46993;
			m_history_1 = history_46954_next_46994;
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
	inline void set_damping(double _value) {
		m_damping_24 = (_value < 0 ? 0 : (_value > 100 ? 100 : _value));
	};
	inline void set_shimmer(double _value) {
		m_shimmer_25 = (_value < 0 ? 0 : (_value > 100 ? 100 : _value));
	};
	inline void set_roomsize(double _value) {
		m_roomsize_26 = (_value < 0.1 ? 0.1 : (_value > 300 ? 300 : _value));
	};
	inline void set_ratio(double _value) {
		m_ratio_27 = (_value < 0.5 ? 0.5 : (_value > 2 ? 2 : _value));
	};
	inline void set_decay(double _value) {
		m_decay_28 = (_value < 0.1 ? 0.1 : (_value > 100 ? 100 : _value));
	};
	inline void set_tone(double _value) {
		m_tone_29 = (_value < 500 ? 500 : (_value > 6000 ? 6000 : _value));
	};
	inline void set_mix(double _value) {
		m_mix_30 = (_value < 0 ? 0 : (_value > 100 ? 100 : _value));
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
int num_params() { return 7; }

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
		case 0: self->set_damping(value); break;
		case 1: self->set_shimmer(value); break;
		case 2: self->set_roomsize(value); break;
		case 3: self->set_ratio(value); break;
		case 4: self->set_decay(value); break;
		case 5: self->set_tone(value); break;
		case 6: self->set_mix(value); break;
		
		default: break;
	}
}

/// Get the value of a parameter of a State object 

void getparameter(CommonState *cself, long index, double *value) {
	State *self = (State *)cself;
	switch (index) {
		case 0: *value = self->m_damping_24; break;
		case 1: *value = self->m_shimmer_25; break;
		case 2: *value = self->m_roomsize_26; break;
		case 3: *value = self->m_ratio_27; break;
		case 4: *value = self->m_decay_28; break;
		case 5: *value = self->m_tone_29; break;
		case 6: *value = self->m_mix_30; break;
		
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
	// initialize parameter 0 ("m_damping_24")
	pi = self->__commonstate.params + 0;
	pi->name = "damping";
	pi->paramtype = GENLIB_PARAMTYPE_FLOAT;
	pi->defaultvalue = self->m_damping_24;
	pi->defaultref = 0;
	pi->hasinputminmax = false;
	pi->inputmin = 0; 
	pi->inputmax = 1;
	pi->hasminmax = true;
	pi->outputmin = 0;
	pi->outputmax = 100;
	pi->exp = 0;
	pi->units = "";		// no units defined
	// initialize parameter 1 ("m_shimmer_25")
	pi = self->__commonstate.params + 1;
	pi->name = "shimmer";
	pi->paramtype = GENLIB_PARAMTYPE_FLOAT;
	pi->defaultvalue = self->m_shimmer_25;
	pi->defaultref = 0;
	pi->hasinputminmax = false;
	pi->inputmin = 0; 
	pi->inputmax = 1;
	pi->hasminmax = true;
	pi->outputmin = 0;
	pi->outputmax = 100;
	pi->exp = 0;
	pi->units = "";		// no units defined
	// initialize parameter 2 ("m_roomsize_26")
	pi = self->__commonstate.params + 2;
	pi->name = "roomsize";
	pi->paramtype = GENLIB_PARAMTYPE_FLOAT;
	pi->defaultvalue = self->m_roomsize_26;
	pi->defaultref = 0;
	pi->hasinputminmax = false;
	pi->inputmin = 0; 
	pi->inputmax = 1;
	pi->hasminmax = true;
	pi->outputmin = 0.1;
	pi->outputmax = 300;
	pi->exp = 0;
	pi->units = "";		// no units defined
	// initialize parameter 3 ("m_ratio_27")
	pi = self->__commonstate.params + 3;
	pi->name = "ratio";
	pi->paramtype = GENLIB_PARAMTYPE_FLOAT;
	pi->defaultvalue = self->m_ratio_27;
	pi->defaultref = 0;
	pi->hasinputminmax = false;
	pi->inputmin = 0; 
	pi->inputmax = 1;
	pi->hasminmax = true;
	pi->outputmin = 0.5;
	pi->outputmax = 2;
	pi->exp = 0;
	pi->units = "";		// no units defined
	// initialize parameter 4 ("m_decay_28")
	pi = self->__commonstate.params + 4;
	pi->name = "decay";
	pi->paramtype = GENLIB_PARAMTYPE_FLOAT;
	pi->defaultvalue = self->m_decay_28;
	pi->defaultref = 0;
	pi->hasinputminmax = false;
	pi->inputmin = 0; 
	pi->inputmax = 1;
	pi->hasminmax = true;
	pi->outputmin = 0.1;
	pi->outputmax = 100;
	pi->exp = 0;
	pi->units = "";		// no units defined
	// initialize parameter 5 ("m_tone_29")
	pi = self->__commonstate.params + 5;
	pi->name = "tone";
	pi->paramtype = GENLIB_PARAMTYPE_FLOAT;
	pi->defaultvalue = self->m_tone_29;
	pi->defaultref = 0;
	pi->hasinputminmax = false;
	pi->inputmin = 0; 
	pi->inputmax = 1;
	pi->hasminmax = true;
	pi->outputmin = 500;
	pi->outputmax = 6000;
	pi->exp = 0;
	pi->units = "";		// no units defined
	// initialize parameter 6 ("m_mix_30")
	pi = self->__commonstate.params + 6;
	pi->name = "mix";
	pi->paramtype = GENLIB_PARAMTYPE_FLOAT;
	pi->defaultvalue = self->m_mix_30;
	pi->defaultref = 0;
	pi->hasinputminmax = false;
	pi->inputmin = 0; 
	pi->inputmax = 1;
	pi->hasminmax = true;
	pi->outputmin = 0;
	pi->outputmax = 100;
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
