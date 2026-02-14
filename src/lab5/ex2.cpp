// Simulation of National Econometric System

#include <iostream>
#include <matplot/matplot.h>
#include <includes/Types.hpp>
#include <util/util.hpp>

#define WIN_SIZE_X 1200
#define WIN_SIZE_Y 800

using pType = double;

#define G_INITIAL 10000 // initial value of expense in Rs
#define Y_INITIAL 15000 // initial value of revenue in Rs

// few params different only from ex1.cpp
int main(int argc, char* agrv[])
{
    const pType gCount { 10 }; // total growth year to cover
    const pType gIncrement { 5000 }; // incremental constant
    // auto gInc = [gIncrement](int x) { return gIncrement*x; }; // for now very simple function (st.line based), incremental callback function for passed with year value
    auto gInc = [gIncrement](int x) { return gIncrement*sqrt(x)/(x?x:1); }; // incremental callback function for passed with year value, little different
    Vector<pType> G; // expenses/spending amount holder
    G.push_back(G_INITIAL); 

    for (int i=1;i<gCount;++i)
    {
        G.push_back(G[i-1]+gInc(i)); // increment is added to previous year amount
    }

    PrintVec(G, "Government Expenses (G)");

    Vector<pType> Y; // year by year budget holder
    Y.push_back(Y_INITIAL); // first year 
    Y.resize(G.size()); // resizing to have size of G holder

    Vector<pType> I; // interest holder
    I.resize(G.size());
    Vector<pType> T; // (have to look)
    T.resize(G.size());
    Vector<pType> C; // consumptions holder
    C.resize(G.size());

    // this formula/mathematical eqn is based on our assumtptions
    for (int i=1;i<gCount;++i)
    {
        Y[i] = 45.45 + 2.27 * (I[i]+G[i]);
        I[i] = 2 + 0.1 * Y[i-1];
        T[i] = 0.2 * Y[i];
        C[i] = 20 + 0.7 * (Y[i] - T[i]);
    }

    PrintVec(Y, "Year to Year Revenue (Y)");
    PrintVec(I, "Investments (I)");
    PrintVec(T, "? (T)");
    PrintVec(C, "Consumptions (C)");

    using namespace matplot;
    
    auto timeT = matplot::linspace(0, gCount, gCount);

    auto f = figure();
    f->size(WIN_SIZE_X, WIN_SIZE_Y);
    // don't show any warning info..
    f->backend()->run_command("unset warnings");
    plot(timeT, G);
    hold(on);
    plot(timeT, Y);
    plot(timeT, I);
    plot(timeT, T);
    plot(timeT, C);
    legend({"G", "Y", "I", "T", "C"});
    title("National Econometric System");
    xlabel("Time (in years)");
    ylabel("Amount (Rs)");

    show();

    return 0;
}