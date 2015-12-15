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
	double m_ratio_25;
	double m_roomsize_24;
	double m_decay_27;
	double m_damping_26;
	double m_mix_29;
	double m_shimmer_30;
	double m_tone_28;
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
		m_delay_16.reset("m_delay_16", 15000);
		m_delay_17.reset("m_delay_17", 96000);
		m_delay_18.reset("m_delay_18", 12000);
		m_delay_19.reset("m_delay_19", 10000);
		m_delay_20.reset("m_delay_20", 48000);
		m_delay_21.reset("m_delay_21", 48000);
		m_delay_22.reset("m_delay_22", 48000);
		m_delay_23.reset("m_delay_23", 48000);
		m_roomsize_24 = 100;
		m_ratio_25 = 2;
		m_damping_26 = 75;
		m_decay_27 = 50;
		m_tone_28 = 3000;
		m_mix_29 = 75;
		m_shimmer_30 = 50;
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
		double mul_6640 = (m_damping_26 * 0.01);
		double mul_6639 = (m_mix_29 * 0.01);
		double mul_6641 = (m_shimmer_30 * 0.01);
		double expr_6646 = safediv(((m_tone_28 * 2) * 3.1415926535898), 48000);
		double sin_6647 = sin(expr_6646);
		double clamp_6648 = ((sin_6647 <= 1e-05) ? 1e-05 : ((sin_6647 >= 0.99999) ? 0.99999 : sin_6647));
		double expr_6857 = safediv((m_roomsize_24 * 48000), 340);
		double mul_6849 = (expr_6857 * 1);
		double mul_6846 = (expr_6857 * 0.63245);
		double mul_6847 = (expr_6857 * 0.7071);
		double mul_6848 = (expr_6857 * 0.81649);
		double mul_6644 = (m_decay_27 * 3.6);
		double expr_6850 = safepow(0.001, safediv(1, (mul_6644 * 48000)));
		double expr_6844 = (-safepow(expr_6850, mul_6849));
		double expr_6755 = (-safepow(expr_6850, mul_6846));
		double expr_6761 = (-safepow(expr_6850, mul_6847));
		double expr_6767 = (-safepow(expr_6850, mul_6848));
		double mul_6843 = (expr_6857 * 0.000527);
		int int_6842 = int(mul_6843);
		double mul_6820 = (int_6842 * 400.600006);
		double mul_6799 = (int_6842 * 419.5);
		double rsub_6698 = (1 - m_ratio_25);
		double mul_6697 = (rsub_6698 * 10);
		samples_to_seconds = (1 / samplerate);
		double add_6786 = (expr_6857 + 5);
		double expr_6779 = safepow(expr_6850, add_6786);
		double mul_6827 = (int_6842 * 568.299988);
		double mul_6806 = (int_6842 * 566.700012);
		double mul_6792 = (expr_6857 * 0.41);
		double add_6789 = (mul_6792 + 5);
		double expr_6785 = safepow(expr_6850, add_6789);
		double mul_6791 = (expr_6857 * 0.3);
		double add_6788 = (mul_6791 + 5);
		double expr_6783 = safepow(expr_6850, add_6788);
		double mul_6790 = (expr_6857 * 0.155);
		double add_6787 = (mul_6790 + 5);
		double expr_6781 = safepow(expr_6850, add_6787);
		double mul_6841 = (expr_6857 * 0.110732);
		double mul_6834 = (int_6842 * 162.100006);
		double mul_6813 = (int_6842 * 144.800003);
		double expr_6861 = safepow(1.0472, m_decay_27);
		double sub_6862 = (expr_6861 - 1);
		double mul_6643 = (sub_6862 * 0.01);
		double mul_6859 = (m_decay_27 * 0.75);
		double add_6858 = (mul_6859 + 25);
		double mul_6860 = (add_6858 * 0.01);
		// the main sample loop;
		while ((__n--)) { 
			const double in1 = (*(__in1++));
			double noise_6650 = noise();
			double abs_6667 = fabs(noise_6650);
			double mul_6673 = (abs_6667 * 0.25);
			double noise_6649 = noise();
			double abs_6666 = fabs(noise_6649);
			double mul_6670 = (abs_6666 * 0.25);
			double noise_6651 = noise();
			double abs_6668 = fabs(noise_6651);
			double mul_6676 = (abs_6668 * 0.25);
			double noise_6652 = noise();
			double abs_6669 = fabs(noise_6652);
			double mul_6679 = (abs_6669 * 0.25);
			double tap_6854 = m_delay_23.read_linear(mul_6849);
			double tap_6760 = m_delay_22.read_linear(mul_6846);
			double tap_6766 = m_delay_21.read_linear(mul_6847);
			double tap_6772 = m_delay_20.read_linear(mul_6848);
			double mul_6845 = (tap_6854 * expr_6844);
			double mix_6913 = (mul_6845 + (mul_6640 * (m_history_10 - mul_6845)));
			double mix_6852 = mix_6913;
			double mul_6756 = (tap_6760 * expr_6755);
			double mix_6914 = (mul_6756 + (mul_6640 * (m_history_9 - mul_6756)));
			double mix_6758 = mix_6914;
			double mul_6762 = (tap_6766 * expr_6761);
			double mix_6915 = (mul_6762 + (mul_6640 * (m_history_8 - mul_6762)));
			double mix_6764 = mix_6915;
			double mul_6768 = (tap_6772 * expr_6767);
			double mix_6916 = (mul_6768 + (mul_6640 * (m_history_7 - mul_6768)));
			double mix_6770 = mix_6916;
			double tap_6819 = m_delay_19.read_linear(mul_6820);
			double mul_6817 = (tap_6819 * 0.625);
			double tap_6798 = m_delay_18.read_linear(mul_6799);
			double mul_6796 = (tap_6798 * 0.625);
			double phasor_6716 = m_phasor_31(mul_6697, samples_to_seconds);
			double add_6696 = ((m_history_6 + phasor_6716) + 0.5);
			double mod_6695 = safemod(add_6696, 1);
			double delta_6675 = m_delta_32(mod_6695);
			double sah_6655 = m_sah_33(mul_6673, delta_6675, 0);
			int sah_6674 = m_sah_34(4800, delta_6675, 0);
			double mul_6663 = (sah_6674 * mod_6695);
			double sub_6694 = (mod_6695 - 0.5);
			double mul_6693 = (sub_6694 * 3.1415926535898);
			double cos_6692 = cos(mul_6693);
			double mul_6682 = (cos_6692 * cos_6692);
			double add_6690 = ((m_history_5 + phasor_6716) + 0.75);
			double mod_6689 = safemod(add_6690, 1);
			double delta_6672 = m_delta_35(mod_6689);
			double sah_6653 = m_sah_36(mul_6670, delta_6672, 0);
			int sah_6671 = m_sah_37(4800, delta_6672, 0);
			double mul_6662 = (sah_6671 * mod_6689);
			double sub_6688 = (mod_6689 - 0.5);
			double mul_6687 = (sub_6688 * 3.1415926535898);
			double cos_6686 = cos(mul_6687);
			double mul_6681 = (cos_6686 * cos_6686);
			double add_6710 = ((m_history_4 + phasor_6716) + 0.25);
			double mod_6709 = safemod(add_6710, 1);
			double delta_6678 = m_delta_38(mod_6709);
			double sah_6657 = m_sah_39(mul_6676, delta_6678, 0);
			int sah_6677 = m_sah_40(4800, delta_6678, 0);
			double mul_6664 = (sah_6677 * mod_6709);
			double sub_6708 = (mod_6709 - 0.5);
			double mul_6707 = (sub_6708 * 3.1415926535898);
			double cos_6706 = cos(mul_6707);
			double mul_6683 = (cos_6706 * cos_6706);
			double add_6715 = ((m_history_3 + phasor_6716) + 0);
			double mod_6714 = safemod(add_6715, 1);
			double delta_6660 = m_delta_41(mod_6714);
			double sah_6659 = m_sah_42(mul_6679, delta_6660, 0);
			int sah_6680 = m_sah_43(4800, delta_6660, 0);
			double mul_6665 = (sah_6680 * mod_6714);
			double tap_6702 = m_delay_17.read_linear(mul_6665);
			double tap_6703 = m_delay_17.read_linear(mul_6664);
			double tap_6704 = m_delay_17.read_linear(mul_6663);
			double tap_6705 = m_delay_17.read_linear(mul_6662);
			double mul_6685 = (tap_6705 * mul_6681);
			double mul_6691 = (tap_6704 * mul_6682);
			double mul_6699 = (tap_6703 * mul_6683);
			double sub_6713 = (mod_6714 - 0.5);
			double mul_6712 = (sub_6713 * 3.1415926535898);
			double cos_6711 = cos(mul_6712);
			double mul_6684 = (cos_6711 * cos_6711);
			double mul_6700 = (tap_6702 * mul_6684);
			double add_6918 = (((mul_6700 + mul_6699) + mul_6691) + mul_6685);
			double mix_6917 = (m_y_2 + (clamp_6648 * (add_6918 - m_y_2)));
			double mix_6645 = mix_6917;
			double mix_6919 = (in1 + (mul_6641 * (mix_6645 - in1)));
			double mix_6717 = mix_6919;
			double mul_6720 = (mix_6717 * 0.707);
			double mix_6920 = (mul_6720 + (0.5 * (m_history_1 - mul_6720)));
			double mix_6856 = mix_6920;
			double add_6750 = (mix_6852 + mix_6770);
			double add_6748 = (mix_6764 + mix_6758);
			double sub_6747 = (add_6750 - add_6748);
			double mul_6729 = (sub_6747 * 0.5);
			double add_6743 = (add_6750 + add_6748);
			double mul_6726 = (add_6743 * 0.5);
			double tap_6826 = m_delay_16.read_linear(mul_6827);
			double tap_6805 = m_delay_15.read_linear(mul_6806);
			double sub_6749 = (mix_6852 - mix_6770);
			double sub_6746 = (mix_6764 - mix_6758);
			double sub_6745 = (sub_6749 - sub_6746);
			double mul_6728 = (sub_6745 * 0.5);
			double add_6744 = (sub_6749 + sub_6746);
			double rsub_6742 = (0 - add_6744);
			double mul_6727 = (rsub_6742 * 0.5);
			double tap_6774 = m_delay_14.read_linear(add_6789);
			double tap_6775 = m_delay_14.read_linear(add_6788);
			double tap_6776 = m_delay_14.read_linear(add_6787);
			double tap_6777 = m_delay_14.read_linear(add_6786);
			double mul_6784 = (tap_6774 * expr_6785);
			double add_6754 = (mul_6729 + mul_6784);
			double mul_6778 = (tap_6777 * expr_6779);
			double add_6751 = (mul_6726 + mul_6778);
			double mul_6782 = (tap_6775 * expr_6783);
			double add_6753 = (mul_6728 + mul_6782);
			double mul_6780 = (tap_6776 * expr_6781);
			double add_6752 = (mul_6727 + mul_6780);
			double mul_6824 = (tap_6826 * 0.625);
			double mul_6803 = (tap_6805 * 0.625);
			double tap_6840 = m_delay_13.read_linear(mul_6841);
			double mul_6838 = (tap_6840 * 0.75);
			double sub_6837 = (mix_6856 - mul_6838);
			double mul_6836 = (sub_6837 * 0.75);
			double add_6835 = (mul_6836 + tap_6840);
			double tap_6833 = m_delay_12.read_linear(mul_6834);
			double tap_6812 = m_delay_11.read_linear(mul_6813);
			double mul_6831 = (tap_6833 * 0.75);
			double mul_6810 = (tap_6812 * 0.75);
			double mul_6741 = (mul_6729 * mul_6643);
			double mul_6739 = (mul_6727 * mul_6643);
			double add_6725 = (mul_6741 + mul_6739);
			double mul_6740 = (mul_6728 * mul_6643);
			double mul_6738 = (mul_6726 * mul_6643);
			double add_6724 = (mul_6740 + mul_6738);
			double sub_6733 = (add_6725 - add_6724);
			double mul_6737 = (mul_6784 * mul_6860);
			double mul_6735 = (mul_6780 * mul_6860);
			double add_6723 = (mul_6737 + mul_6735);
			double mul_6736 = (mul_6782 * mul_6860);
			double mul_6734 = (mul_6778 * mul_6860);
			double add_6722 = (mul_6736 + mul_6734);
			double sub_6732 = (add_6723 - add_6722);
			double add_6719 = (sub_6733 + sub_6732);
			double add_6731 = (add_6719 + mix_6717);
			double sub_6830 = (add_6731 - mul_6831);
			double mul_6829 = (sub_6830 * 0.75);
			double add_6828 = (mul_6829 + tap_6833);
			double sub_6823 = (add_6828 - mul_6824);
			double mul_6822 = (sub_6823 * 0.625);
			double add_6821 = (mul_6822 + tap_6826);
			double sub_6816 = (add_6821 - mul_6817);
			double mul_6815 = (sub_6816 * 0.625);
			double add_6814 = (mul_6815 + tap_6819);
			double mul_6730 = (add_6814 * mul_6639);
			double out1 = (in1 + mul_6730);
			double add_6721 = (add_6719 + mix_6717);
			double sub_6809 = (add_6721 - mul_6810);
			double mul_6808 = (sub_6809 * 0.75);
			double add_6807 = (mul_6808 + tap_6812);
			double sub_6802 = (add_6807 - mul_6803);
			double mul_6801 = (sub_6802 * 0.625);
			double add_6800 = (mul_6801 + tap_6805);
			double sub_6795 = (add_6800 - mul_6796);
			double mul_6794 = (sub_6795 * 0.625);
			double add_6793 = (mul_6794 + tap_6798);
			double mul_6718 = (add_6793 * mul_6639);
			double out2 = (in1 + mul_6718);
			double history_6851_next_6863 = mix_6852;
			double history_6757_next_6864 = mix_6758;
			double history_6763_next_6865 = mix_6764;
			double history_6769_next_6866 = mix_6770;
			double history_6656_next_6867 = sah_6655;
			double history_6654_next_6868 = sah_6653;
			double history_6658_next_6869 = sah_6657;
			double history_6661_next_6870 = sah_6659;
			double y0_next_6871 = mix_6645;
			double history_6855_next_6872 = mix_6856;
			m_delay_23.write(add_6754);
			m_delay_22.write(add_6751);
			m_delay_21.write(add_6752);
			m_delay_20.write(add_6753);
			m_delay_19.write(sub_6816);
			m_delay_18.write(sub_6795);
			m_delay_17.write(in1);
			m_delay_16.write(sub_6823);
			m_delay_15.write(sub_6802);
			m_delay_14.write(add_6835);
			m_delay_13.write(sub_6837);
			m_delay_12.write(sub_6830);
			m_delay_11.write(sub_6809);
			m_history_10 = history_6851_next_6863;
			m_history_9 = history_6757_next_6864;
			m_history_8 = history_6763_next_6865;
			m_history_7 = history_6769_next_6866;
			m_history_6 = history_6656_next_6867;
			m_history_5 = history_6654_next_6868;
			m_history_4 = history_6658_next_6869;
			m_history_3 = history_6661_next_6870;
			m_y_2 = y0_next_6871;
			m_history_1 = history_6855_next_6872;
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
	inline void set_roomsize(double _value) {
		m_roomsize_24 = (_value < 0.1 ? 0.1 : (_value > 300 ? 300 : _value));
	};
	inline void set_ratio(double _value) {
		m_ratio_25 = (_value < 0.5 ? 0.5 : (_value > 2 ? 2 : _value));
	};
	inline void set_damping(double _value) {
		m_damping_26 = (_value < 0 ? 0 : (_value > 100 ? 100 : _value));
	};
	inline void set_decay(double _value) {
		m_decay_27 = (_value < 0.1 ? 0.1 : (_value > 100 ? 100 : _value));
	};
	inline void set_tone(double _value) {
		m_tone_28 = (_value < 500 ? 500 : (_value > 6000 ? 6000 : _value));
	};
	inline void set_mix(double _value) {
		m_mix_29 = (_value < 0 ? 0 : (_value > 100 ? 100 : _value));
	};
	inline void set_shimmer(double _value) {
		m_shimmer_30 = (_value < 0 ? 0 : (_value > 100 ? 100 : _value));
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
		case 0: self->set_roomsize(value); break;
		case 1: self->set_ratio(value); break;
		case 2: self->set_damping(value); break;
		case 3: self->set_decay(value); break;
		case 4: self->set_tone(value); break;
		case 5: self->set_mix(value); break;
		case 6: self->set_shimmer(value); break;
		
		default: break;
	}
}

/// Get the value of a parameter of a State object 

void getparameter(CommonState *cself, long index, double *value) {
	State *self = (State *)cself;
	switch (index) {
		case 0: *value = self->m_roomsize_24; break;
		case 1: *value = self->m_ratio_25; break;
		case 2: *value = self->m_damping_26; break;
		case 3: *value = self->m_decay_27; break;
		case 4: *value = self->m_tone_28; break;
		case 5: *value = self->m_mix_29; break;
		case 6: *value = self->m_shimmer_30; break;
		
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
	// initialize parameter 0 ("m_roomsize_24")
	pi = self->__commonstate.params + 0;
	pi->name = "roomsize";
	pi->paramtype = GENLIB_PARAMTYPE_FLOAT;
	pi->defaultvalue = self->m_roomsize_24;
	pi->defaultref = 0;
	pi->hasinputminmax = false;
	pi->inputmin = 0; 
	pi->inputmax = 1;
	pi->hasminmax = true;
	pi->outputmin = 0.1;
	pi->outputmax = 300;
	pi->exp = 0;
	pi->units = "";		// no units defined
	// initialize parameter 1 ("m_ratio_25")
	pi = self->__commonstate.params + 1;
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
	// initialize parameter 3 ("m_decay_27")
	pi = self->__commonstate.params + 3;
	pi->name = "decay";
	pi->paramtype = GENLIB_PARAMTYPE_FLOAT;
	pi->defaultvalue = self->m_decay_27;
	pi->defaultref = 0;
	pi->hasinputminmax = false;
	pi->inputmin = 0; 
	pi->inputmax = 1;
	pi->hasminmax = true;
	pi->outputmin = 0.1;
	pi->outputmax = 100;
	pi->exp = 0;
	pi->units = "";		// no units defined
	// initialize parameter 4 ("m_tone_28")
	pi = self->__commonstate.params + 4;
	pi->name = "tone";
	pi->paramtype = GENLIB_PARAMTYPE_FLOAT;
	pi->defaultvalue = self->m_tone_28;
	pi->defaultref = 0;
	pi->hasinputminmax = false;
	pi->inputmin = 0; 
	pi->inputmax = 1;
	pi->hasminmax = true;
	pi->outputmin = 500;
	pi->outputmax = 6000;
	pi->exp = 0;
	pi->units = "";		// no units defined
	// initialize parameter 5 ("m_mix_29")
	pi = self->__commonstate.params + 5;
	pi->name = "mix";
	pi->paramtype = GENLIB_PARAMTYPE_FLOAT;
	pi->defaultvalue = self->m_mix_29;
	pi->defaultref = 0;
	pi->hasinputminmax = false;
	pi->inputmin = 0; 
	pi->inputmax = 1;
	pi->hasminmax = true;
	pi->outputmin = 0;
	pi->outputmax = 100;
	pi->exp = 0;
	pi->units = "";		// no units defined
	// initialize parameter 6 ("m_shimmer_30")
	pi = self->__commonstate.params + 6;
	pi->name = "shimmer";
	pi->paramtype = GENLIB_PARAMTYPE_FLOAT;
	pi->defaultvalue = self->m_shimmer_30;
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
