#include <Types.hpp>
#include <Utils.hpp>
#include <pserandgen/Random.hpp>

#define RANDOM_NUM_COUNT 10
#define RANDOM_MAX_VALUE 100

int main(int argc, char* argv[]) {

    auto randGenerator = LinearCongRNGen();
    randGenerator.Init(RANDOM_MAX_VALUE);

    auto randNums = randGenerator.NextFew(RANDOM_NUM_COUNT);
    VecPrint(randNums, "Random Numbers (seq1)");

    randNums = randGenerator.NextFew(RANDOM_NUM_COUNT);
    VecPrint(randNums, "Random Numbers (seq2)");
    
    randGenerator.Reset();
    randNums = randGenerator.NextFew(RANDOM_NUM_COUNT);
    VecPrint(randNums, "Random Numbers (after reset, same as seq1)");
    
    return 0;
}