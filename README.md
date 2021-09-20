# IPE Generator
 
This library is an API for the IPE drawing software in C++ and is specifically design to work 
for the **[IBEX](http://www.ibex-lib.org/)** and **[CODAC](http://codac.io/)** library to draw figures for 
scientific articles

!!! There is two version of this API. The master branch work with IPE 7.2.13 (installed with apt) and the dev branch with 
the current version of IPE [7.2.24](https://ipe.otfried.org/) (if this not the case anymore do not hesitate to 
generate an issue, I will update it ^^)

## Installation guidelines (IPE apt version)

To use this API you first need to install **IBEX** and **CODAC** using the guidelines 
[here](http://codac.io/install/01-installation.html). 

Then using the terminal install the development version of libipe
```
sudo apt install libipe-dev
```

Then clone the repository and compile the sources

```
git clone https://github.com/JulienDamers/ipe_generator.git
cd ipe_generator
mkdir -p build && cd build
cmake ..
make
sudo make install
```



### For advanced users
If you decide to install **IBEX** and **CODAC** in locations other than the default folder 
(/usr ...) you will have to specify the installation folder in the CmakeLists




