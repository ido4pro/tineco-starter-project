/**
 * Created by sguilly on 09/11/15.
 */

var express = require('express');
var multer  = require('multer');
var upload = multer({ dest: 'uploads/' });
var fs = require('fs');
var mv = require('mv');

var app = express();

var getClientIp = function(req) {
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

app.post('/ota/:deviceid', upload.single('rom'), function (req, res, next) {
  // req.file is the `avatar` file
  // req.body will hold the text fields, if there were any

    console.log(getClientIp(req));

    var deviceid = req.params.deviceid;

    console.log(deviceid,' -> receive file',req.file);

    var source = (req.file.destination+req.file.filename).toString();
    var destination = (req.file.destination+deviceid+'/'+req.file.originalname).toString();

    console.log(source,destination);

    mv(source,destination,{mkdirp: true}, function(err) {
        if ( err ) console.log('ERROR: ' + err);
    });

    res.sendStatus(200);
});

var server = app.listen(3003, function () {
    var host = server.address().address;
    var port = server.address().port;

    console.log('Example app listening at http://%s:%s', host, port);
});