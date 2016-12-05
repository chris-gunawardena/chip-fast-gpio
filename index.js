


var bindings = require('bindings');
var gpio = bindings('chip-fast-gpio');

gpio.export(132);
gpio.mode(132, "out");

// https://bbs.nextthing.co/t/solved-gpio-pin-numbers/2931/16
var start_time = new Date().getTime();
for(var i=0; i<1000; i++) {
	gpio.write(132, 1);
}
var end_time = new Date().getTime();
console.log(end_time - start_time);

gpio.unexport(132);
