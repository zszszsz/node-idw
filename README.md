# node-idw
inverse distance weighting interpolation for nodejs in C++

-----

````
var options = {
    nth: ?,
    same: ?,
    neighbor: ?,
    loLim: ?,
    upLim: ?
}
````
--------

**nth**: 

Nth power of distance. default value is **1**.

--------

**same**: 

Threshold of same point check : 

A **given point** will be considered as the **same point** of the **grid point**,

if the distance between them is **lower** than this threshold.

The **value** of the **grid point** will be set to the value of the given point,

And the interpolation process for this grid point will **stop immediately**.

Default value is **2 * DBL_MIN**.

--------


**neighbor**:

Threshold of neighborhood check.

A **given point** will **NOT** be considered a **neighbor** of the grid point,

if the distance between them is **larger** than this threshold.

Only neighbor points have effect on the grid point,

which means the given point will be **skiped** if it is not a neighbor.

Default value is **DBL_MAX**.

--------


**loLim**: 

Lower limit.

If the value of a given point is lower than the limit,
 
it will be **skip**, **unless** it is a **neighbor** of the grid point.

Default value is **-DBL_MAX**.

--------

**upLim**: 

upper limit. same.

Default value is **DBL_MAX**.

--------
````
const idw = require('idw')
...
points = [
    {value: 10, longitude: 11, latitude: 12},
    {value: 1, longitude: 2, latitude: 3},
    {value: 7, longitude: 8, latitude: 9},
    {value: 4, longitude: 5, latitude: 6},
    ...
]
x = Array.from({length: width}, (v,k)=> x(k))
y = Array.from({length: height}, (v,k)=> y(k))

var interpolated = idw(points, x, y, [options])
````

or


````
const idw = require('idw')
value = [10,1,7,4,...]
x = [11,2,8,5,...]
y=[12,3,9,6,...]
X = Array.from({length: width}, (v,k)=> X(k))
Y = Array.from({length: height}, (v,k)=> Y(k))
var interpolated = idw(value, x, y, X, Y, [options])
````

