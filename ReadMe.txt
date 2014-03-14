Points and Tangents

Captures relevant points and tangents from images, via Morlet wavelet filtering.
Should be cross platform, although I only tested it in OS X.
For a Mac-only version, see https://github.com/cicconet/PAT_Mac.


main.cpp
  Main file with examples for image IO, wavelet design,
  wavelet filtering, and capturing points and tangents.

PATImage
  Image IO, using the PNG library.

PATWavelet
  Morlet wavelet structure.

PATConvolution
  Handles convolutions. Convolution is implemented locally,
  so it’s not as fast as in your favorite convolution library.

PATCoefficients
  Main class. Captures relevant points and their tangents.

PAT.xcodeproj
  Xcode project file, in case you use Xcode.
  To compile on Terminal, type
  g++ *.cpp -o pat -I/usr/local/include -L/opt/local/lib -lpng
  or similar, depending on where you installed the PNG library.


Marcelo Cicconet
New York University
marceloc.net




