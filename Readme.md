## Simulation and Modeling Lab

> [!IMPORTANT]
> This project is setup in mac (apple silicon) & all the guide here are tuned to its specifics. Any usages in other platform might differ. Please change the build script in premake & other as per required.

This repo is setup with [matplotplusplus](https://github.com/alandefreitas/matplotplusplus) as a foundation. All the lab exercises are inside `/src` folder with respective lab number (eg: `lab1`, `lab2`, etc). 

**Dependencies**

for machine with [brew](https://brew.sh/) installed.
```bash
brew install matplotplusplus
```

> [!NOTE]
> Other dependencies are of matplotplusplus. like eg: opencv for images manipulation ...

**How to use**
- Clone the repo
```bash
git clone https://github.com/Thaparoshan143/simmod_lab # if specific -b <branch-name>
```

- Change the source code inside `\src` folder & execute the command
```bash
bash buru.sh # & any args if taken in script 
```

- Example code: (taken from original [matplotplusplus here](https://github.com/alandefreitas/matplotplusplus/blob/master/examples/line_plot/plot/plot_3.cpp))
```cpp
#include <cmath>
#include <matplot/matplot.h>

int main() {
    using namespace matplot;

    std::vector<double> x = linspace(0, 2 * pi);
    std::vector<double> y1 = transform(x, [](auto x) { return sin(x); });
    std::vector<double> y2 = transform(x, [](auto x) { return sin(x - 0.25); });
    std::vector<double> y3 = transform(x, [](auto x) { return sin(x - 0.5); });
    plot(x, y1, x, y2, "--", x, y3, ":");

    show();
    return 0;
}
```
Find other [matplotplusplus example here](https://github.com/alandefreitas/matplotplusplus/tree/master/examples)

- For cleanup
```bash 
bash cleanup.sh
```
