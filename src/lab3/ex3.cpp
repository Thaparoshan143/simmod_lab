// Basic implementation of the random number generation...

#include <util/util.hpp>
#include <includes/Types.hpp>

#include <matplot/matplot.h>
// boost basic math library for chi-square standard values
#include <boost/math/distributions/chi_squared.hpp>

#include <lab3/Linearcong.hpp>

#define RANDOM_NUM_COUNT 1000
#define RANDOM_MAX_VALUE RAND_MAX

#define WIN_SIZE_X 1200
#define WIN_SIZE_Y 800

#define HISTOGRAM_BIN_SIZE 20

using pType = double;

// only few params are changed from ex1.cpp, (variant)
int main(int argc, char* argv[]) 
{
    using namespace matplot;

    auto randGenerator = LinearCongRNGen();
    // auto randGenerator = CombLinearCongRNGen();
    // using custom seed, a, c taken standard used value in rand()..
    randGenerator.Init(RANDOM_MAX_VALUE, 341, 1103515245, 12345); 

    auto randNums = randGenerator.NextFew(RANDOM_NUM_COUNT);
    auto randNumsNormalize = GetVectorNormalize<pType>(randNums, RANDOM_MAX_VALUE);
    #ifdef LOGGING
    PrintVec(randNums, "Random Numbers (seq1)");
    PrintVec(randNumsNormalize, "Random Numbers Normalized (seq1)");
    #endif 

    randNums = randGenerator.NextFew(RANDOM_NUM_COUNT);
    #ifdef LOGGING
    PrintVec(randNums, "Random Numbers (seq2)");
    #endif
    
    randGenerator.Reset();
    randNums = randGenerator.NextFew(RANDOM_NUM_COUNT);
    #ifdef LOGGING
    PrintVec(randNums, "Random Numbers (after reset, same as seq1)");
    #endif
    
    auto f = figure();
    f->size(WIN_SIZE_X, WIN_SIZE_Y);
    // don't show any warning info..
    f->backend()->run_command("unset warnings");

    subplot(1, 2, 0);
    // might give some error but works..
    hist(randNums, HISTOGRAM_BIN_SIZE);
    title("Normal");

    subplot(1, 2, 1);
    hist(randNumsNormalize, HISTOGRAM_BIN_SIZE);
    title("Normalized");

    show();

    // now chi-square test...
    auto binValues = GetVectorBinned<uint, pType, uint>(randNums, RANDOM_MAX_VALUE, HISTOGRAM_BIN_SIZE);
    // Map<uint, Vector<pType>> binValues = GetVectorBinned(randNums, HISTOGRAM_BIN_SIZE);

    uint expectedFr = RANDOM_NUM_COUNT / HISTOGRAM_BIN_SIZE;

    ::Vector<pType> obsFreq {};

    Seperator();
    std::cout << "Expected Freq: " << expectedFr << std::endl;

    for (const auto& k : binValues)
    {
        auto numCount = k.second.size();
        std::cout << "-> Num count for ind : " << k.first << " IS : " << numCount << std::endl;
        obsFreq.push_back(numCount);
    }

    float chiSqrVal = 0;

    for (const auto& obfr: obsFreq)
    {
        chiSqrVal += (pow(obfr - expectedFr, 2) / expectedFr);
    }

    const float alpha = 0.05;
    // this is sample dependent later subsitute with chi2inv function if applicable.. (currently using boost math lib as alternative..)
    boost::math::chi_squared_distribution<double> dist(HISTOGRAM_BIN_SIZE-1); // here degree of fredom is n-1, (n=bin size here..)
    auto chiSqrStandard = boost::math::quantile(dist, 1.0 - alpha);
    std::cout << "## CHI-Square: " << chiSqrVal << " || Standard: " << chiSqrStandard << std::endl;

    if (chiSqrVal < chiSqrStandard)
    {
        std::cout << "*** Hypothesis is accepted" << std::endl;
    }
    else 
    {
        std::cout << "*** Hypothesis is not accepted" << std::endl;
    }

    return 0;
}