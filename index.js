'use strict';

const config = 'Release';// 'Debug'

module.exports = require(`./build/${config}/idw`).idw;
