# BMP Image Processor in C++

**This is an image-editor built with C++ for applying filters to 24-bit bitmap files with RGB24 pixel format.**

## Features



<br>

## Getting Started
Instructions to install and test this project locally.


### Prerequisites
- C++ Compiler — must support at least the C++17 standard, i.e. MSVC, GCC, Clang


### Installing
- Clone this GitHub repository by running:
```
git clone https://github.com/an-sla/image_processor.git
```

### Building
- Use CMake settings or the following command-line arguments in the `image_processor` directory to build the project:


```diff
- shown for g++ and the C++17 standard -
g++ -std=c++17 -o image_processor image_processor.cpp controller.cpp file_work.cpp filters.cpp
```

- Then use the command line to input arguments and apply filters:


```diff
- shown for lena.bmp, negative filter -
./image_processor input-bmp_files/input-lena.bmp negative_results/output.bmp -neg
```

<br>

## License
- The source images for this project listed in the [input-bmp_files](https://github.com/an-sla/image_processor/tree/main/input-bmp_files) directory have been taken from:
  - [FileFormat.Info](https://www.fileformat.info/format/bmp/sample/1d71eff930af4773a836a32229fde106/download) under the Creative Commons Attribution-Share Alike 3.0 United States License;
  - [The University of Southern Carolina BMP Files page](https://people.math.sc.edu/Burkardt/data/bmp/bmp.html) under the the GNU LGPL license;
  - [The HSE University GitLab Repository for this project](https://gitlab.com/levanovd/cpp-base-hse-2022), originally by Dwight Hooker, copyrighted, and distributed under the 'Fair Use' exception to US copyright law.

This project is licensed under the [GNU General Public License 3.0](https://www.gnu.org/licenses/gpl-3.0.en.html).

<br>

## Contributors

Anastasia / [an-sla](https://github.com/an-sla)

[![Linkedin](https://img.shields.io/badge/LinkedIn-0077B5?style=for-the-badge&logo=linkedin&logoColor=white)](https://www.linkedin.com/in/anastasia-slabucho-21b9b219b/)


