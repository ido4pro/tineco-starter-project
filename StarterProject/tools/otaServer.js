/**
 * Created by sguilly on 09/11/15.
 */
 
 
 var sscanf = require('scanf').sscanf;
 var fs = require('fs');
 var data = fs.readFileSync('/opt/workspace/src/include/define.h');

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

var express = require('express');
var app = express();

var bodyParser = require('body-parser');
app.use(bodyParser.json()); // support json encoded bodies
app.use(bodyParser.urlencoded({ extended: true })); // support encoded bodies

  const folderRom = 'romFiles/';

  var multer  = require('multer');
  var upload = multer({ dest: folderRom });
  
  var mv = require('mv');
  var path = require('path');


  var getClientIp = function(req) {
    
    console.log('req.headers=',req.headers)
    var ipAddress;

    var forwardedIpsStr = req.headers['x-forwarded-for'];
    //Allow getting cloudflare connecting client IP
    var cloudFlareConnectingIp=req.headers['cf-connecting-ip'];

    if (forwardedIpsStr) {
      var forwardedIps = forwardedIpsStr.split(',');
      ipAddress = forwardedIps[0];
    }

    if(cloudFlareConnectingIp!=undefined){
      ipAddress=cloudFlareConnectingIp;
    }

    if (!ipAddress) {
      ipAddress = req.connection.remoteAddress;
    }


    if(!ipAddress){
      return '';
    }

    if(ipAddress.indexOf(':') !== -1){
      ipAddress = ipAddress.split(':')[0];
    }

    return ipAddress;
  };

  function fileExists(filePath)
  {
    try
    {
      return fs.statSync(filePath).isFile();
    }
    catch (err)
    {
      return false;
    }
  }

  app.post('/ota/:deviceid', upload.single('rom'), function (req, res, next) {

    var deviceid = req.params.deviceid;

    console.log('Receive rom file for ',deviceid);

    var source = (req.file.destination+req.file.filename).toString();
    var destination = (req.file.destination+deviceid+'/'+req.file.originalname).toString();
    
    console.log('Source='+source)
    console.log('Destination='+destination)

    mv(source,destination,{mkdirp: true}, function(err) {
      if ( err ) console.log('ERROR: ' + err);
    });

    res.sendStatus(200);
  });

  app.get('/ota/:deviceid/*', function(req,res)
  //app.get('/ota/*', function(req,res)
  {
    
    var deviceid = req.params.deviceid;
    
    console.log('Ask rom file for ',deviceid);

    var pathToBin = folderRom+deviceid+'/rom0.bin';


    if(fileExists(pathToBin))
    {
      console.log('Send rom file for ',deviceid);

      res.sendFile(path.join(__dirname, pathToBin));

    }else
    {
      res.sendStatus(404);
    }

  });

  app.post('/start', function(req,res)
  {

    console.log('POST: ',req.body);

    var externalIp = getClientIp(req);

    console.log('START DEVICE - IP adress =',externalIp);

    res.writeHead(200, {'Content-Type': 'text/plain'});
    res.end(deviceid);

  });

var server = app.listen(3003, function () {
    var host = server.address().address;
    var port = server.address().port;

    console.log('Example app listening at http://%s:%s', host, port);
});