# gr-UTAT_HERON v2.0.0

A GNURadio out-of-tree module for the HERON Mk II ground station.

The [ground-station](https://github.com/HeronMkII/ground-station) repository has some notes and instructions, but we are in the process of moving everything pertaining to this block to this repository.

## Installation

First clone (devel branch) and build using cmake

```
git clone https://github.com/utat-ss/HERON-gr-utat -b devel
cd HERON-gr-utat
mkdir build
cd build
cmake .. -DCMAKE_BUILD_TYPE=Release
cmake --build .
```

Then, continue running the following in the `build` directory after cmake for your system

### ArchLinux

```
cpack -G TGZ
makepkg -si
```

### Debain Based

```
cpack -G DEB
sudo dpkg -i *.deb
```

### RPM Based

```
cpack -G RPM
sudo rpm -i *.rmp
```

## Blocks

### heron_rx_bb

Useful links:
1. [Endurosat Tranceiver onboard HERON Mk II](https://www.endurosat.com/cubesat-store/cubesat-communication-modules/uhf-transceiver-ii/#request-step-modal)
2. [Endurosat's ESTTC Protocol documentation for UHF Type II Tranceiver](https://drive.google.com/file/d/1QbZfTUcGsZVrNnLC-i74AeppRjXvG178/view?usp=sharing)
3. [GNURadio OutOfTree Module documentation](https://wiki.gnuradio.org/index.php/OutOfTreeModules)

Identifies data packets that are in the *UHF Type II RX/TX Packet Structure* used by the Endurosat Tranceiver. documentation on the packet structure can be found on page 13 of UHF Type II ESTTC Protocol documentation (link 2). 

This block will output the data in *Data Field 2* (refer to packet strucutre) **after suffixing it with an additional byte: 0x0D (carriage return)**. Thus, the output size (# of bytes) for each full packet received should be = (*Data Field 1*) + 1. See the testing code in python/qa_heron_rx_bb.py for more insight.

The input to this block should be unpacked data (every byte carries a single bit), but the output will be packed (normal 8-bit bytes). This way, the output of the [GFSK Demod block](https://wiki.gnuradio.org/index.php/GFSK_Demod) can be directly connected to the input of this block, with no packing required before or after.

## How this repository was created

```
gr_modtool newmod UTAT_HERON
cd gr-UTAT_HERON
gr_modtool add -t general -l cpp heron_rx_bb
    Please specify the copyright holder: University of Toronto Aerospace Team
    Enter valid argument list, including default arguments: # Leave empty, press enter

    Add Python QA code? [Y/n] y
    Add C++ QA code? [y/N] n
gr_modtool bind heron_rx_bb
```

The above provided a template code on which functionaility was added.
