# gr-utat

Out of tree modules for ground station.

Contents:

* EnduroSat Frame
* more to come

## Step 1: Install GNURadio

Make sure you install GR using `pybombs` by following the instructions in its readme: https://github.com/gnuradio/pybombs#pybombs

## Step 2: Install gr-utat

Mandatory dependencies:
* swig

To install, first clone the repository. Place it in a known location (I like to place it within where I've installed gnuradio, with all the other packages). Next, use the following commands:
* !! Make sure you are in a terminal with GNURadio set up! If you set up with pybombs, you make have to run `source gnuradio/default/setup_env.sh` before moving on !!
* `cd /path/to/gr-utat`
* `mkdir build`
* `cd build`
* `cmake ..`
* `make`
* `sudo make install`
* `sudo ldconfig`
