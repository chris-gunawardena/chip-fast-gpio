


var bindings = require('bindings');
var gpio = bindings('chip-fast-gpio');

  var start_time = new Date().getTime();
  for(var i=0; i<1000; i++) {
    gpio.write(408, 1)
  }
  var end_time = new Date().getTime();
  console.log(end_time - start_time);

