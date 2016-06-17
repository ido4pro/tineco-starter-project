var express = require('express');
var app = express();

var request = require('request');

var path = require('path');

var fs = require('fs')

var nameFile;

app.get('/*', function (req, res) {
    console.log('url=', req.url);
    console.log('h=', req.headers);
    console.log('b=', req.body);

      if (req.url.indexOf('rom0.bin') > -1) {
      console.log('rom0');
        res.sendFile(path.join(__dirname, '../out/firmware/rom0.bin'));
        //res.sendFile('/media/sguilly/storage/SRC/esp8266/nqd2/esp8266-dev/examples/mqtt/firmwares/1024_user1.bin');
    } else  if (req.url.indexOf('rom1.bin') > -1) {
    	console.log('rom1');
        res.sendFile(path.join(__dirname, '../out/firmware/rom0.bin'));
        //res.sendFile('/media/sguilly/storage/SRC/esp8266/nqd2/esp8266-dev/examples/mqtt/firmwares/1024_user2.bin');
    }
    
    else
    {
    	res.sendStatus(200);
    }


});

var server = app.listen(8887, function () {

    var host = server.address().address;
    var port = server.address().port;

    console.log('OTA SERVER listening at http://%s:%s', host, port);

});