// Basic implementation of Simulation of Continuous Systems (Chemical Equilibrium)

/*
# Necessary Theory

Evaluation of Chemical equilibrium reaction.
Assumption is that temp. press. & other factor are constant (factor affecting the reaction based on Le Chateliers' principle) 

Reversible reaction

A + B <-> C

Rate of reactants given by:

    𝑑𝐶1
    --- = 𝐾2𝐶3 − 𝐾1𝐶1𝐶2
    𝑑𝑡
    𝑑𝐶2
    --- = 𝐾2𝐶3 − 𝐾1𝐶1𝐶2
    𝑑𝑡
    𝑑𝐶3
    --- = 𝐾1𝐶1𝐶2− 𝐾2𝐶3
    𝑑𝑡

    Where K1 and K2 are the constants which determine how fast the reaction occurs in each
    direction.

Simplifying assumption with 1 mole of A and 1 mole of B react to product 1 mole of C.

Let C1(t), C2(t) and C3(t) be the concentration of reactants at ‘t’ instance of time. Then after
time Δ𝑡, the concentration of the reactants and products is given by:

    𝐶1(𝑡 + Δ𝑡) = 𝐶1(𝑡) + 𝑑𝐶1 ∗ Δ𝑡
                        𝑑𝑡

    𝐶2(𝑡 + Δ𝑡) = 𝐶2(𝑡) + 𝑑𝐶2 ∗ Δ𝑡
                        𝑑𝑡

    𝐶3(𝑡 + Δ𝑡) = 𝐶3(𝑡) + 𝑑𝐶3 ∗ Δ𝑡
                        𝑑𝑡
*/

#include <includes/Types.hpp>
#include <util/util.hpp>

#include <cmath>
#include <matplot/matplot.h>

#define ITERATION_COUNT 10000
#define INITIAL_REACTANT_CON 8
#define FORWARD_RXN_RATE 4
#define BACKWARD_RXN_RATE 7
#define DELTA_TIME 0.000025

#define REACTANT_CON_INJECT 4 // amount to inject
#define TIME_OF_INJECTION 0.5 // time at which con. is injected, range(0, 1), 0-start, 1-end

#define LINE_WIDTH 2
#define AXIS_Y_OFFSET 1
#define WIN_SIZE_X 1200
#define WIN_SIZE_Y 800

// #define LOGGING // if defined/used any gaurd block with log enabled under this will run

// precision datatype to use throughout
using pType = double;

// this is similar to ex-1, but we inject the small con. in reactant after certain time..
// warning: this implementation only contain single con. injection at given time, if multiple injection required use another version
int main() {
    using namespace matplot; 

    // rate of reaction (forward and backward)
    const pType K1 { FORWARD_RXN_RATE }, K2 { BACKWARD_RXN_RATE };

    pType deltaT { DELTA_TIME }; // this might defer so non-const
    uint iter { ITERATION_COUNT }; // total iteration count to perform.

    // concentration of the reactant and products
    pType C1 { INITIAL_REACTANT_CON }, C2 { INITIAL_REACTANT_CON }, C3 { 0 }, timeStep { 0 };

    // rate of change of reactant and products
    // differentiation form with respect to time, dCx/dt, used with dCx
    pType dC1 {}, dC2 {}, dC3 {};

    // hold the concentration of Cx with respect to tim
    std::vector<pType> C1t {};
    std::vector<pType> C2t {};
    std::vector<pType> C3t {};
    std::vector<pType> vtime {};

    // allocating enough space before hand to limit the reallocation
    C1t.reserve(iter);
    C2t.reserve(iter);
    C3t.reserve(iter);
    vtime.reserve(iter);
    
    // first state concentration is known, so starting from 1 with later index adjustment
    for (uint i=1;i<iter;++i)
    {
        C1t.push_back(C1);
        C2t.push_back(C2);
        C3t.push_back(C3);
        vtime.push_back(timeStep);

        // rate of change calculation
        dC1 = K2 * C3 - K1 * C1 * C2;
        dC2 = K2 * C3 - K1 * C1 * C2;
        dC3 = K1 * C1 * C2 - K2 * C3;

        // concentration calculation 
        C1 = C1t[i-1] + dC1 * deltaT;
        C2 = C2t[i-1] + dC2 * deltaT;
        C3 = C3t[i-1] + dC3 * deltaT;

        timeStep += deltaT;

        // check for conc. injection
        if (i == TIME_OF_INJECTION * iter)
        {
            // only reactant is injected.
            C1 += REACTANT_CON_INJECT;
            C2 += REACTANT_CON_INJECT;
        }
    }

    #ifdef LOGGING
    PrintVec(C1t, "Concentration of Reactant 1");
    PrintVec(C2t, "Concentration of Reactant 2");
    PrintVec(C3t, "Concentration of Product");
    // std::cout << "Max of C1: " << max(C1t) << std::endl;
    // std::cout << "Min of C1: " << min(C1t) << std::endl;
    #endif

    if (C1t.size() != vtime.size())
    {
        std::cout << "Size must be same of X and Y" << std::endl;
        return -2;
    }

    auto f = figure();
    f->size(WIN_SIZE_X, WIN_SIZE_Y);
    // don't show any warning info..
    f->backend()->run_command("unset warnings");

    subplot(2, 2, 0);
    plot(vtime, C1t, "-")->line_width(LINE_WIDTH);
    xlabel("time");
    ylabel("concentration");
    title("Reactant 1 (A)");
    ylim({0, INITIAL_REACTANT_CON + AXIS_Y_OFFSET});

    subplot(2, 2, 1);
    plot(vtime, C2t, "-")->line_width(LINE_WIDTH);
    xlabel("time");
    ylabel("concentration");
    title("Reactant 2 (B)");
    ylim({0, INITIAL_REACTANT_CON + AXIS_Y_OFFSET});
    
    subplot(2, 2, 2);
    plot(vtime, C3t, "-")->line_width(LINE_WIDTH);
    xlabel("time");
    ylabel("concentration");
    title("Product 1 (C)");

    show();

    return 0;
}
