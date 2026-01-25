// This file contains the implementation of Simulation of the R-C Amplifier Circuit
/*
Basic Thoery Background

Theory
1. Amplifier Circuit
2. Overview of a Transistor
3. Biasing of Transistor
4. CE Biasing
5. Voltage Divider Biasing
6. RC Coupled Amplifier

Methematical Formulation
DC Biasing Analysis
    1. Voltage Divider Biasing
    2. DC operating point equations
    3. Collector and Enitter Currents
    4. Node Voltages

AC (small signal) Analysis
    1. Thermal Voltage
    2. Dynamic Emitter Resistance
    3. Transconductance
    4. Input Impedance
    5. Output Impedance
    6. Voltage Gain
        a. Without Load Resistor
        b. With Load Resistor

Frequency Response Analysis
    1. Capacitor Impedances
    2. Effective Emitter Impedances
    3. Frequency Dependent Input Impedance
    4. Transfer Function Computation
    5. Magnitude and phase calculation

Time Domain Analysis

------------------------------

Procedure Outline

1. Define the amplifier component and parameters.
a. 𝛽
b. Base-Emitter Voltage
c. DC Biasing voltage (𝑉 𝑐𝑐)
d. Biasing Resistors (𝑅1, 𝑅2, 𝑅𝐶, 𝑅𝑒) and Load Resistor (𝑅𝑙)
e. Coupling and Bypass Capacitors (𝐶𝑖𝑛, 𝐶𝑜𝑢𝑡, 𝐶𝑒)
f. Input signal amplitude and frequency (test will be done using sine wave).

2. Perform DC Bias Analysis. Calculate:
a. Thevenin’s Voltage
b. Thevenin's Resistance
c. Base Current
d. Collector Current
e. Emitter Current
f. Collector-Emitter Voltage
g. Collector Voltage
h. Emitter Voltage
i. Base Voltage

3. Perform AC small signal Analysis. Calculate:
a. Thermal Voltage
b. Dynamic Emitter Resistance
c. Transconductance
d. Input Impedance
e. Output Impedance
f. Voltage Gain with no load
g. Voltage Gain with load resistance

4. Perform Frequency Response Analysis.
a. Define Frequency Range (1 Hz to 1MHz). Use ‘logspace’ function.
b. Compute angular frequency from the defined frequency.
c. Compute: Coupling Capacitor impedances (𝑍𝐶𝑖𝑛 , 𝑍𝐶𝑜𝑢𝑡 , 𝑍𝐶𝑒 )
d. Compute the effective emitter impedance.
e. Compute the frequency dependent impedances. (𝑍𝑖𝑛, 𝑍𝑒𝑒𝑓𝑓 )
f. Compute the different transfer function for different stages.
g. Compute the overall transfer function.
h. Compute the magnitude and phase at different frequencies.
i. Plot the magnitude and phase against frequencies.

5. Perform Time domain Analysis
a. Plot the input sine signal for 5 time periods with proper sampling.
b. Compute the gain and phase shift for input signal frequency
c. Compute the output signal.
d. Plot the output signal.

This is just rough outline. Find detailed report at: https://typst.app/project/rEPr6HSomYEpvAPEdQ1P2X
*/

#include <includes/Types.hpp>
#include <util/util.hpp>

#include <matplot/matplot.h>


// general 
using pType = double;
// for the electronics values percision..
using pvType = float;

// here for defining different components values structs (aggregates) are used.
// defining component and parameters.
struct TransistorProps
{
    pType beta { 100.0 };
};
const TransistorProps TP;

const pvType V_be { 0.7 };
const pvType V_cc { 12.0 };

struct BiasingRegisters
{
    pvType R_1 { 10e3 };
    pvType R_2 { 2.2e3 };
    pvType R_c { 2.2e3 };
    pvType R_e { 0.47e3 };
    pvType R_l { 10e3 };
};
const BiasingRegisters BR;

struct CapacitorProps
{
    pvType C_in { 10e-6 };
    pvType C_out { 10e-6 };
    pvType C_e { 100e-6 };
};
const CapacitorProps CP; 

using dComplex = Complex<double>;

const dComplex imgNum(0, 1);
const dComplex realNum(1, 0);

#define LINE_WIDTH 2

#define WIN_SIZE_X 1200
#define WIN_SIZE_Y 400


// after number 5 impl
int main(int argc, char* argv[])
{
    using namespace matplot;

    // DC bias analysis
    pvType V_th = V_cc * (BR.R_2 / (BR.R_1 + BR.R_2));
    pvType R_th = (BR.R_1 * BR.R_2) / (BR.R_1 + BR.R_2);
    pvType I_b = (V_th - V_be) / ((TP.beta + 1) * BR.R_e + R_th);
    pvType I_c = TP.beta * I_b;
    pvType I_e = (TP.beta + 1) * I_b;
    pvType V_e = I_e * BR.R_e;
    pvType V_c = V_cc - I_c * BR.R_c;
    pvType V_ce = V_c - V_e;
    pvType V_b = V_be + V_e;

    // AC small signal analysis
    const pType _k = 1.38e-23;
    const pType _T = 25 + 273;
    const pType _q = 1.6e-19;

    pvType V_t = _k * _T / _q;
    pvType r_e = V_t / I_e;
    pvType g_m = I_c / V_t;
    pvType Z_in = (TP.beta * r_e * BR.R_e) / (TP.beta * r_e + BR.R_e);
    pvType Z_out = BR.R_c;

    pvType Awol_v = -g_m * BR.R_c;
    pvType R_eq = (BR.R_c * BR.R_l) / (BR.R_c + BR.R_l);
    pvType Awl_v = -g_m * R_eq;

    // frequency response analysis
    const uint freq_min { 0} ; // eq. to 10e0
    const uint freq_max { 4 }; // in logspace 
    const uint samp_count { 1000 };
    auto freq_range = logspace(freq_min, freq_max, samp_count);

    auto ang_freq = VecScalMul(freq_range, 2*pi);
    PrintVec(ang_freq, "Angular Freq");

    auto Z_CinOp = [](const auto& v) {
        // this is solved form of 1/jwCin -> -j/wCin
        return dComplex(0, - 1.0 / (v * CP.C_in));
    };
    auto Z_Cin = VecOpCbComplex(ang_freq, Z_CinOp);
    
    auto Z_CoutOp = [](const auto& v) {
        // this is solved form of 1/jwCout -> -j/wCout
        return dComplex(0, - 1.0 / (v * CP.C_out));
    };
    auto Z_Cout = VecOpCbComplex(ang_freq, Z_CoutOp);
    
    auto Z_CeOp = [](const auto& v) {
        // this is solved form of 1/jwCe -> -j/wCe
        return dComplex(0, -1.0 / (v * CP.C_e));
    };
    auto Z_Ce = VecOpCbComplex(ang_freq, Z_CeOp);

    auto Z_eeffOp = [](const auto& c) {
        // equivalent to operation of R_e * Z_Ce / (R_e + Z_Ce)
        return (c * dComplex(BR.R_e, 0)) / (c + dComplex(BR.R_e, 0));
    };
    auto Z_eeff = VecOpCb(Z_Ce, Z_eeffOp);

    // this is Z_in(f) i.e for each freq.
    auto Z_in_fOp = [r_e, R_th](const auto& c) {
        // equivalent to Rth // (beta * (Z_eeff + r_e))
        // intermediate result 
        auto intRes = dComplex(TP.beta, 0) * (c + dComplex(r_e, 0));
        return dComplex(R_th, 0) * intRes / (dComplex(R_th, 0) + intRes);
    };
    auto Z_in_f = VecOpCb(Z_eeff, Z_in_fOp);


    // computation of different transfer function
    auto H_inOp = [](const auto& c1, const auto& c2) {
        // equivalent to Zin / (Zcin + Zin)
        // here first arg is expected as Zin
        return c1 / (c1 + c2);
    };
    auto H_in = DualVecOpCb(Z_in_f, Z_Cin, H_inOp);

    auto H_amp = -g_m * (R_eq / (1 + g_m * R_eq));

    auto H_outOp = [](const auto& c) {
        // equivalent of Rl / (Rl + Rc + ZCout)
        return dComplex(BR.R_l, 0) / (dComplex(BR.R_l + BR.R_c, 0) + c);
    };
    auto H_out = VecOpCb(Z_Cout, H_outOp);

    auto H_overall = DualVecOpCb(H_in, H_out, [H_amp](const auto& e1, const auto& e2) {
        return dComplex(H_amp, 0) * e1 * e2;
    });


    // now the plotting stuffs here..
    auto mag = GetComplexVecMag(H_overall);

    auto phase = GetComplexVecPhase(H_overall, true); // degree based..
    auto phase_rad = GetComplexVecPhase(H_overall); // radian based..

    #ifdef LOGGING
    std::cout << Z_Cin[0] << std::endl;
    std::cout << Z_Cout[0] << std::endl;
    std::cout << Z_Ce[0] << std::endl;
    std::cout << Z_eeff[0] << std::endl;
    std::cout << Z_in_f[0] << std::endl;
    std::cout << H_in[0] << std::endl;
    std::cout << H_amp << std::endl;
    std::cout << H_out[0] << std::endl;
    std::cout << H_overall[0] << std::endl;
    std::cout << mag[0] << std::endl;
    std::cout << phase[0] << std::endl;
    PrintVec(phase, "Phase values");
    #endif

    const pType A_in = 1.5;    
    const pType tp = 5;

    const pType freq_top = pow(10, freq_max);
    const pType freq_choice = freq_range[int(freq_range.size()/1.75)];
    const pType samp_freq = 16*freq_choice;
    // const pType 
    const pType fs_f = samp_freq / freq_choice;

    auto time = linspace(0, tp / freq_choice, fs_f*tp);
    // auto ang_freq = VecOpCb(time, [](const pType t) { return 2*pi*t; });
    auto V_in = VecOpCb(time, [tp, freq_choice](const pType t) { return sin(2*pi*freq_choice*t); });
    V_in = VecScalMul(V_in, A_in);
    
    auto choice_mag = mag[freq_choice];
    auto choice_phase = phase_rad[freq_choice];
    auto V_out = VecOpCb(time, [tp, freq_choice, choice_phase](const pType t) { return sin(2*pi*freq_choice*t + choice_phase); });
    V_out = VecScalMul(V_out, choice_mag);
    V_out = VecScalMul(V_out, A_in);
    
    auto f = figure();
    f->size(WIN_SIZE_X, WIN_SIZE_Y);
    // don't show any warning info..
    f->backend()->run_command("unset warnings");

    subplot(1, 1, 0)->line_width(LINE_WIDTH);
    plot(time, V_in);
    hold(true);
    plot(time, V_out);
    xlabel("time");
    ylabel("magnitude");
    title("Sine Plot: fs/f: " + std::to_string(fs_f) + " || freq choice: " + std::to_string(freq_choice));

    show();

    return 0;
}