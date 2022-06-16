# BMP Image Processor in C++

**This is an image-editor built with C++ for applying filters to bitmap files.**

5 filters are realised. The program supports 24-bit BMPs with RGB24 pixel format, no data compression or colour profiles, and with a `DIB header` of type `BITMAPINFOHEADER`. The image format corresponds to [this example](https://en.wikipedia.org/wiki/BMP_file_format#Example_1).

<br>

## Features

The program returns an image in the same 24-bit BMP format with one or more out of 5 available filters applied to it. If no filter argument is provided, the program returns the original image. If multiple filter arguments are given, the filters are applied consecutively.

**Argument Input Format**

The program accepts input in the form:

```
{program name} {path to input.bmp} {path to output.bmp} [-{filter 1} [param 1] [param 2]...] [-{filter 2} [param 1] [param 2]...]...
```
Note the dash in front of the filter name. If no filter arguments are provided where they are needed or if the parameter format and number of parameters does not correspond to the filter name, the program returns an error message with instructions of what to include.


**Available Filters**

1. Crop `-crop width height`
Обрезает изображение до заданных ширины и высоты. Используется верхняя левая часть изображения.

Если запрошенные ширина или высота превышают размеры исходного изображения, выдается доступная часть изображения.

2. Grayscale `-gs`
Преобразует изображение в оттенки серого по формуле

![encoding](https://latex.codecogs.com/svg.image?R'%20=%20G'%20=%20B'%20=0.299%20R%20&plus;%200%20.587%20G%20&plus;%200%20.%20114%20B)

3. Negative `-neg`
Преобразует изображение в негатив по формуле

![encoding](https://latex.codecogs.com/svg.image?R'%20=%201%20-%20R,%20G'%20=%201%20-%20G,%20B'%20=%201%20-%20B)

4. Sharpening `-sharp`
Повышение резкости. Достигается применением матрицы

![encoding](https://latex.codecogs.com/svg.image?%5Cbegin%7Bbmatrix%7D%20&%20-1%20&%20%20%5C%5C-1%20&%205%20&%20-1%20%5C%5C%20&%20-1%20&%20%20%5C%5C%5Cend%7Bbmatrix%7D)

5. Edge Detection `-edge threshold`

Selects and highlights edges. The image is converted to greyscale and the following matrix is applied:

<img src="https://latex.codecogs.com/svg.image?\begin{bmatrix}&space;&&space;&space;-1&&space;&space;\\&space;-1&&space;&space;4&&space;&space;-1\\&space;&&space;&space;-1&&space;&space;\\\end{bmatrix}">

Pixels whose values exceed the given `threshold` are coloured white, the rest are coloured black.



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

- Then use the command line to input arguments and apply filters as described above:


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


