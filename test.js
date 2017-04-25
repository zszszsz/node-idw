#!/usr/bin/node
'use strict';
const idw = require('./index.js');

var points = [
    { value: 10, longitude: 11, latitude: 12 },
    { value: 1, longitude: 2, latitude: 3 },
    { value: 7, longitude: 8, latitude: 9 },
    { value: 4, longitude: 5, latitude: 6 }
];
var x = Array.from({ length: 40 }, (v, k) => k / 2);
var y = Array.from({ length: 40 }, (v, k) => k / 2);
var interpolated = idw(points, x, y, 2);
console.log(interpolated);

// var value = [10, 1, 7, 4];
// var x = [11, 2, 8, 5];
// var y = [12, 3, 9, 6];
// var X = Array.from({ length: 40 }, (v, k) => k / 2);
// var Y = Array.from({ length: 40 }, (v, k) => k / 2);
// var interpolated2 = idw(value, x, y, X, Y, 0.5)

// console.log(interpolated.every((col, y) => col.every((value, x) => value == interpolated2[y][x])));