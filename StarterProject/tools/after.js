var fs = require('fs');
var sscanf = require('scanf').sscanf;

var FormData = require('form-data');


var data = fs.readFileSync('./include/define.h');
//var data = fs.readFileSync('../include/version.h');

var lines = data.toString().split(/(?:\n|\r\n|\r)/g);

var deviceid;
var serverUrl;

lines.forEach(function(line)
{
    if(line.lastIndexOf('#define DEVICE_ID', 0) === 0)
    {
        deviceid = sscanf(line,'#define DEVICE_ID \"%s\"');
    }
    else if(line.lastIndexOf('#define SERVER_URL', 0) === 0)
    {
        serverUrl = sscanf(line,'#define SERVER_URL \"%s\"');
    }




});

console.log('deviceId=',deviceid);

var form = new FormData();

form.append('rom', fs.createReadStream('./out/firmware/rom0.bin'));

console.log('serverUrl=',serverUrl);

form.submit(serverUrl +'/ota/'+deviceid, function(err, res) {
    
    console.log('upload done!')
   res.resume();

    var mqtt = require('mqtt')
    var clientMqtt = mqtt.connect({

        host:'mqtt.ido4pro.com',
        clientId: 'forTest'

    });

    clientMqtt.publish(deviceid+'/command', JSON.stringify({command: 'ota'}),{qos:0, retain: false}, function()
    {
        process.exit()
    });

    setTimeout(function(){process.exit()},5000);
});
