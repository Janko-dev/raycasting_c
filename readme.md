# Ray casting in C
This repository contains an example application of 2d ray casting written in the C language for both recreational and educational purposes. Ray casting is a technique where `k` rays are cast from a center of projection with each at a different angle. These rays are vectors defined by their polar coordinate representation, i.e., `{angle, length}`. To define the point where a ray has potentially hit a target, the [line-line segment intersection](https://en.wikipedia.org/wiki/Line%E2%80%93line_intersection) algorithm is used. The length of a given ray is set to the shortest target hit. This gives the effect of flashing light with a flashlight onto corners of walls.  

The application is linked with [`SDL2`](https://www.libsdl.org/) and build with `make`. For **Linux** users, building is very simple, given that `SDL2` is installed on the system. Refer [here](https://wiki.libsdl.org/SDL2/Installation) for more information. For **Windows** users, you need to adjust the `Makefile` with your own include (`-I`) path and lib (`-L`) path. 

## Quick start
Assuming `SDL2` is installed, the following will build and run the app.
```
$ git clone
$ cd raycasting
$ make
$ ./rays
```

## Deep dive

### Line-line segment intersection algorithm

The Wikipedia page for [`line segment intersection`](https://en.wikipedia.org/wiki/Line%E2%80%93line_intersection) states multiple approaches for finding the correct intersection point, if any, between 2 line segments. The one used here, presented by [Antonio, F. (1992)](https://www.sciencedirect.com/science/article/pii/B9780080507552500452?via%3Dihub), is defined in terms of first degree Bézier curves, where the line segments are defined by their parametric equation, i.e., 
```math
    L = \{(x, y) \in \mathbb{R} \;|\; (x, y) = \vec{p_0} + t\vec{v},\; t \in \mathbb{R}\}
``` 
Given 2 points $P1, P2 \in \mathbb{R}^2$, interpolating between these points in a linear manner results in a line which can be formulated as $P = \alpha P1 + (1-\alpha)P2$, where $\alpha$ is in the interval $[0, 1]$. Expanding and simplifying this gives the parametric form: $P = P1 + \alpha(P2-P1)$. 

This brings us to the equations for locating the intersection point between 2 line segments. The following system of linear equations consists of 2 equations defined by their respective 2 points, $(P1,P2)$ and $(P3,P4)$, in the plane and linear interpolation variables, i.e., $\alpha, \beta$. 
```math
\displaylines{P^* = P1 + \alpha(P2-P1)\\ P^* = P3 + \beta(P4-P3)}
```
Solving this system for $\alpha$ and $\beta$ gives
```math
    \alpha = \frac{ByCx - BxCy}{AyBx - AxBy},\;\;\;\;\;\;\;\;\;\;\;\;\beta = \frac{AxCy - AyCx}{AyBx - AxBy}
```
where the intermediate values of $A, B, C$ are used for readability of the formula, and are defined as follows
```math
\displaylines{A = P2 - P1\\ B = P3 - P4\\ C = P1 - P3}
```
Notice that the denominators are equal, which means a single test `if denominator != 0` will suffice. Furthermore, there will be an intersection if $\alpha, \beta \in [0, 1]$. Finally, to compute the intersection point when all conditions are met, refer to either equation: $P^* = P1 + \alpha(P2-P1)$ or $P^* = P3 + \beta(P4-P3)$.

### Polar coordinates
The use of polar coordinates defined by `{angle, length}` makes working and updating vectors very simple. The basic mechanism to convert from polar coordinates to cartesian coordinates is defined by 
```math
    P_{cart} = (length \cdot \cos(angle),\;\; length \cdot\sin(angle))
```
To obtain the angle between 2 vectors, the inverse tangent function was used, i.e., `atan2(y, x)`. With these conversion schemes, updating vectors means updating their angle and length. Since multiple rays depend on a single angle variable, albeit offset by a multiple of a constant, computing vectors on the fly is computationally an efficient operation. 

### Translation to (fake) 3D
Computing intersections between rays means that their lengths are updated to the shortest intersection hit. This results in data about the length of the rays with respect to a max ray length. Plotting the ray lengths as rectangles can give the illusion of a 3 dimensional space. When running the application, one can use the `Q` button on their keyboard to change the viewpoint from 2D to 3D and vice versa. 

<img src="img\2d_view.PNG" alt="drawing" width="500"/>
<img src="img\3d_view.PNG" alt="drawing" width="500"/>
