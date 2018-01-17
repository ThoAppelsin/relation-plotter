cThis project presents an application for plotting relations with two variables, onto bitmap image files.
The given relation need not be a function.

The application is written in C, in the October of 2014, in the middle of my third semester at the Boğaziçi University,
and my first semester at the Computer Engineering department.
It was an application developed out of self-interest, and in response to a recommendation of a friend.

## Overview

Given a relation of the form:

> <img src="http://www.sciweavers.org/tex2img.php?eq=r%28x%2C%20y%29%20%3D%200&bc=White&fc=Black&im=jpg&fs=18&ff=modern&edit=0" align="center" border="0" alt="r(x, y) = 0" width="117" height="29" />

such as a circle with radius of 4 units:

> <img src="http://www.sciweavers.org/tex2img.php?eq=x%5E2%20%2B%20y%5E2%20-%2016%20%3D%200&bc=White&fc=Black&im=jpg&fs=18&ff=modern&edit=0" align="center" border="0" alt="x^2 + y^2 - 16 = 0" width="179" height="29" />

this application plots the set of points satisfying the relation.
Inequalities, such as filled circles, may also be plotted.

## Usage

The application is unfortunately not an interactive one, which may be changed in an upcoming version.
As it is, it draws a list of relations represented by functions in C language.
As an example, the circle with radius 4 ventered at the origin would have a C function as follows:

```c
double rad4circle( double y, double x )
{
	return y * y + x * x - 16;
}
```

The output file is in the PGM format of the [Netpbm formats](https://en.wikipedia.org/wiki/Netpbm_format).
It may be viewed using the [IrfanView](http://www.irfanview.com/) image viewer application, as well as any viewer supporting the format.

## Shortcomings

- The curves drawn will be, depending on the way they were represented, be bolder at some parts, while finer (and sometimes even fainter)
at some others. While the phenomenon itself is a curious one, it is not a desired outcome, and needs to be fixed.
- Users should be able to specify the curves to be drawn without changing the code.
- Users should be able to specify the output file.
- More file formats, especially more common ones, should be supported.

