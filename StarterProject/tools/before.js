var fs = require('fs');
var sscanf = require('scanf').sscanf;

var data = fs.readFileSync('./include/version.h');
//var data = fs.readFileSync('../include/version.h');

var lines = data.toString().split(/(?:\n|\r\n|\r)/g);

var version;

lines.forEach(function(line)
{
    if(line.lastIndexOf('#define VERSION', 0) === 0)
    {
        version = sscanf(line,'#define VERSION \"%d.%d.%d\"');
    }
});

var stream = fs.createWriteStream('./include/version.h');
stream.once('open', function(fd) {
    stream.write('#ifndef _VERSION_H_\n');
    stream.write('#define _VERSION_H_\n');
    stream.write('#define VERSION \"'+version[0]+'.'+version[1]+'.'+(version[2]+1)+'"\n');
    stream.write('#endif');

    stream.end();
});

console.log('VERSION = '+version[0]+'.'+version[1]+'.'+(version[2]+1));


