#!/usr/bin/node
'use strict';

const config = 'Release';

var idw = require(`./build/${configName}/idw`);

console.log(Array.from({ length: 8 }, (v, k) => k / 2));

// console.log(idw.idw([{ value: 2, longitude: 1, latitude: 1 }, { value: 1, longitude: 2, latitude: 2 }], Array.from({ length: 8 }, (v, k) => k / 2), Array.from({ length: 8 }, (v, k) => k / 2)));
console.log(idw.idw([2, 1], [1, 2], [1, 2], Array.from({ length: 8 }, (v, k) => k / 2), Array.from({ length: 8 }, (v, k) => k / 2)))