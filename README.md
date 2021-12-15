# IPE Generator
 
This library is an API for the IPE drawing software in C++ and is specifically design to work 
for the **[IBEX](http://www.ibex-lib.org/)** and **[CODAC](http://codac.io/)** library to draw figures for 
scientific articles

**!!!** There are two versions of this API. The ubuntu branch work with IPE 7.2.13 (installed with 
apt) and the master branch  with  the current version of IPE [7.2.24](https://ipe.otfried.org/) 
(if this not the case anymore do not hesitate to generate an issue, I will update it ^^)


## Dependencies

To build *ipe-generator* you need to install some dependencies :

```bash
sudo apt install libpng-dev libjpeg-dev libspiro-dev
```


## Installation guidelines (most recent ipe version with master branch)

To use this API you first need to install **IBEX** and **CODAC** using the guidelines
[here](http://codac.io/install/01-installation.html).


Then clone the repository and compile the sources.
```
git clone https://github.com/JulienDamers/ipe_generator.git
cd ipe_generator
mkdir -p build && cd build
cmake ..
make
sudo make install
```


## Installation guidelines (IPE apt version with ubuntu_ipe branch)

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



## For advanced users
- If you decide to install **IBEX** and **CODAC** in locations other than the default folder 
(/usr/ ...) you will have to specify the installation folder in the CMakeLists
- If you want to install ipe_generator in a folder other than _/usr/local/ipegenerator_ you will 
  have to specify the path to the installation folder in _src/core/figure.h_ line 386

```cpp
std::string m_ref_document = "/path/you/want/ipegenerator/style/basic.ipe";
```
and then compile and install the source with: 

```
git clone https://github.com/JulienDamers/ipe_generator.git
cd ipe_generator
mkdir -p build && cd build
cmake -DCMAKE_INSTALL_PREFIX=path/you/want ..
make
sudo make install
```





