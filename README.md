# BMP Image Processor in C++

**This is an image-editor built with C++ for applying filters to bitmap files.**

5 filters are realised. The program supports 24-bit BMPs with RGB24 pixel format, no data compression or colour profiles, and with a `DIB header` of type `BITMAPINFOHEADER`. The image format corresponds to [this example](https://en.wikipedia.org/wiki/BMP_file_format#Example_1).

<br>

## Features

The program returns an image in the same 24-bit BMP format with one or more out of 5 available filters applied to it. If no filter argument is provided, the program returns the original image. If multiple filter arguments are given, the filters are applied consecutively.

Each pixel colour is represented by a 1x3 vector of values between 0 and 1, corresponding to its `(R, G, B)` markers. `(0, 0, 0)` represents black and `(1, 1, 1)` represents white. Some of the filters apply a matrix to each of the pixel's colours by using input from its surrounding pixels — thus, in the 'Sharpening' and 'Edge' filters, a pixel's colours are influenced by the 8 pixels surrounding it (directly above, on each side, and below). When a given pixel lies in a corner or at the border of the image, only non-empty / existing surrounding pixels are used to calculate its new values.


### Input Format

The program accepts input in the form:

```
{program name} {path to input.bmp} {path to output.bmp} [-{filter 1} [param 1] [param 2]...] [-{filter 2} [param 1] [param 2]...]...
```
Note the dash in front of the filter name. If no filter arguments are provided where they are needed or if the parameter format and number of parameters does not correspond to the filter name, the program returns an error message with instructions of what to include.


### Available Filters

**1. Crop** `-crop width height`

Crops the image to the given width and height. The upper-left corner of the image is used as the starting point. If the height or the width parameters exceed the original image size, the entire available original image is returned.


**2. Grayscale** `-gs`

Converts the image to greyscale, based on the formula:
```diff
- R' G' B' are new values, R, G, B are original values for each pixel -
R' = B' = G = 0.299 * R + 0.587 * G + 0.114 * B
```


**3. Negative** `-neg`

Inverts the image colours to a negative scale, based on the formula:
```diff
- R' G' B' are new values, R, G, B are original values for each pixel -
R' = 1 - R
G' = 1 - G
B' = 1 -B
```

**4. Sharpening** `-sharp`

Enhances image sharpness, the following matrix is applied:
```diff
- the value of each pixel is multiplied by the matrix: -
 0   —1   0
—1    5  —1
 0   —1   0

- thus, E is the new matrix and A is the original —
E[x][y] =
   0 * A[x-1][y-1]  +  (-1) * A[x][y-1]  +  0 * A[x+1][y-1] +
   (-1) * A[x-1][y]  +  5 * A[x][y]  +  (-1) * A[x+1][y] +
   0 * A[x-1][y+1]  +  (-1) * A[x][y+1]  +  0 * A[x+1][y+1]
```


**5. Edge Detection** `-edge threshold`

Selects and highlights edges. The image is converted to greyscale and the following matrix is applied:

```diff
- the value of each pixel is multiplied by the matrix: -
 0   —1   0
—1    4  —1
 0   —1   0

- thus, E is the new matrix and A is the original —
E[x][y] =
   0 * A[x-1][y-1]  +  (-1) * A[x][y-1]  +  0 * A[x+1][y-1] +
   (-1) * A[x-1][y]  +  4 * A[x][y]  +  (-1) * A[x+1][y] +
   0 * A[x-1][y+1]  +  (-1) * A[x][y+1]  +  0 * A[x+1][y+1]
```

Pixels whose values after this multiplication exceed the given `threshold` argument are coloured white `(1, 1, 1)`, the rest are coloured black `(0, 0, 0)`.



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
### Examples

- Sample images are available in the [input-bmp_files](https://github.com/an-sla/image_processor/tree/main/input-bmp_files) directory, expected results with the given parameters are provided in the [crop](https://github.com/an-sla/image_processor/tree/main/crop-various_results), [greyscale](https://github.com/an-sla/image_processor/tree/main/greyscale_results), [negative](https://github.com/an-sla/image_processor/tree/main/negative_results), [sharpening](https://github.com/an-sla/image_processor/tree/main/sharpening_results), and [edge](https://github.com/an-sla/image_processor/tree/main/edge0.01_results) directories.

<br>

## License
- The source images for this project listed in the [input-bmp_files](https://github.com/an-sla/image_processor/tree/main/input-bmp_files) directory have been taken from:
  - [FileFormat.Info](https://www.fileformat.info/format/bmp/sample/1d71eff930af4773a836a32229fde106/download) under the Creative Commons Attribution-Share Alike 3.0 United States License;
  - [The University of Southern Carolina BMP Files page](https://people.math.sc.edu/Burkardt/data/bmp/bmp.html) under the the GNU LGPL license;
  - [The HSE University GitLab Repository for this project](https://gitlab.com/levanovd/cpp-base-hse-2022), originally by Dwight Hooker, copyrighted, and distributed under the 'Fair Use' exception to US copyright law.

These have been modified with the program's filters for demonstration and educational purposes.

This project is licensed under the [GNU General Public License 3.0](https://www.gnu.org/licenses/gpl-3.0.en.html).

<br>

## Contributors

Anastasia / [an-sla](https://github.com/an-sla)

[![Linkedin](https://img.shields.io/badge/LinkedIn-0077B5?style=for-the-badge&logo=linkedin&logoColor=white)](https://www.linkedin.com/in/anastasia-slabucho-21b9b219b/)


