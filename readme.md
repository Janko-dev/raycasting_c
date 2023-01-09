# Ray casting in C
This repo contains an example application for 2d ray casting written in the C language for both recreational and educational purposes. Ray casting is a technique where `k` rays are cast from a center of projection with each at a different angle. These rays are vectors defined by their polar coordinate representation, i.e., `{angle, length}`. To define the point where a ray has potentially hit a target, the [line-line segment intersection](https://en.wikipedia.org/wiki/Line%E2%80%93line_intersection) algorithm is used. The length of a given ray is set to the shortest target hit. This gives the effect of flashing light with a flashlight onto corners of walls.  

The application is linked with [`SDL2`](https://www.libsdl.org/) and build with `make`. For **Linux** users, building is very simple, given that `SDL2` is installed on the system. Refer [here](https://wiki.libsdl.org/SDL2/Installation) for more information. For **Windows** users, you need to adjust the `Makefile` provided in the repo with your own include (`-I`) path and lib (`-L`) path. 

## Quick start
Assuming `SDL2` is installed, the following will build and run the app.
```
$ git clone
$ cd raycasting
$ make
$ ./rays
```