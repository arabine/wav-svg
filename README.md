# WAV-SVG

WAV from/to SVG converter (back and forth)

<!-- Version and License Badges -->
![Version](https://img.shields.io/badge/version-0.1.0-green.svg?style=flat-square) 
![License](https://img.shields.io/badge/license-GPL-blue.svg?style=flat-square) 
![Language](https://img.shields.io/badge/language-C++-yellow.svg?style=flat-square) 


This project has two programs:
  * wav-svg command line utility
  * DrawSound GUI tool

Both of them allow you to convert a WAV file into SVG and an SVG file into a WAV file.

![Screenshot](drawsound.png)

The generated SVG file has one polyline element for each audio channel.

![SVG file in Inkscape](svg_export.png)


Acknowledgements
-------

Thanks to the Pyhon library wav2vec https://github.com/cristoper/wav2vec, my SVG file format is the same.

Third party libraries
-------

  * Qt (qt.io)
  * AudioFile is written and maintained by Adam Stark [http://www.adamstark.co.uk](http://www.adamstark.co.uk) Copyright (c) 2017 Adam Stark
  * Some functions borrowed from NanoSVG https://github.com/memononen/nanosvg (zlib license) Nano SVG Copyright (c) 2013-14 Mikko Mononen memon@inside.org

How to build
-------

Use Qt, each project has its own project (.pro) file. The command line program has its own Makefile project. Just type make to build it.

Versions
-------

##### 1.0.0 - 19th May 2018

- Version 1.0
- GUI
- Command line tool

##### 0.1.0-alpha - 8th May 2018

- Initial release (generation only, one channel)

TODO
-------

  * Support 32-bit audio generation
  * Add options to the command line program

License
-------

Copyright (c) 2018 Anthony Rabine

This program is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program.  If not, see <http://www.gnu.org/licenses/>.
