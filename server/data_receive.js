var http = require('http');
var dbconnect;
var myurl = require('url');
var Drinks = {
	"milk": "milk",
	"water": "water",
	"blacktea": "blacktea",
	"milktea":"milktea"
} 


http.createServer(function(req, res){
	var str = myurl.parse(req.url, true).query;
	var sys = require('sys')
	var exec = require('child_process').exec;
	var child;
	var sudo = require('sudo');
	var options = {
		cachePassword: true,	
		prompt: 'Password, yo? ',
		spawnOptions: { /* other options for spawn */ }
	};

//	res.writeHead(200, {'Content-Type': 'text/html'});
	if(str.distance && str.angle){
		console.log(str.distance);
		console.log(str.angle);
		child = sudo(['./../move_car', str.angle, str.distance], options);
		child.stdout.on('data',function(data){
			console.log(data.toString());
		});
	}

	if (Drinks[str.drink]){
		
		var txt = "you ordered : " + str.drink;
		res.end(txt);

		var num = '5';
		if(str.drink == 'water'){
			num = '0';
		}else if(str.drink == 'milk'){
			num = '1';
		}else if(str.drink == 'blacktea'){
			num = '2';
		}else if(str.drink == 'milktea'){
			num = '3';
		}else {
			console.log("wrong drink");
		}

		child = sudo(['./../drink_service', num], options);
		child.stdout.on('data', function (data){ 
	  		console.log(data.toString());
		});
	}
	else{
		var txt = "error";
		res.end(txt);
	}
//	console.log('here');
}).listen(54321);


console.log('server is running at port 54321');
