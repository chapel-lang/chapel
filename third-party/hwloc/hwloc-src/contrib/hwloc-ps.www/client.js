/* Copyright © 2019 Inria.  All rights reserved. */

var express = require('express')
var path = require('path')
var app = express()
var net = require('net')
var HOST = 'localhost'
var HPPORT = 8888
var NPORT = 3000
var verbose = 0
var open = 1
var fs = require('fs')
var open = require("open")
var lastRequest = ''
var client = new net.Socket()
var svg
var json

function usage() {
    console.log(path.basename(process.argv[0]) + " " + path.basename(process.argv[1]) + " [options] <file.svg>")
    console.log("  SVG file <file.svg> must be generated with lstopo --of nativesvg <file.svg>")
    console.log("Options:")
    console.log("  --hp <port>   Change hwloc-ps JSON server port (default is "+HPPORT+")")
    console.log("  --np <port>   Change node port (default is "+NPORT+")")
    console.log("  -u            Display the webpage URL instead of opening it")
    console.log("  -v --verbose  Increase verbosity")
    console.log("  -h --help     Show this help")
}

var currentarg = 2
while(process.argv.length > currentarg){
    if(process.argv[currentarg] === '-h' || process.argv[currentarg] === '-help'){
	usage()
	return
    }else if (process.argv[currentarg] === '-v' || process.argv[currentarg] === '--verbose'){
	verbose++
	currentarg++
    }else if (process.argv[currentarg] === '-u'){
	open = 0
	currentarg++
    }else if (process.argv[currentarg] === '--hp'){
	if(currentarg+2 > process.argv.length){
	    console.err("Missing argument after --hp")
	    usage()
	    return
	}
	HPPORT = process.argv[currentarg + 1]
	currentarg += 2
    }else if (process.argv[currentarg] === '--np'){
	if(currentarg+2 > process.argv.length){
	    console.err("Missing argument after --np")
	    usage()
	    return
	}
	NPORT = process.argv[currentarg + 1]
	currentarg += 2
    }else{
	if(svg != undefined){
	    console.err("Unexpected parameter after SVG filename.")
	    usage()
	    return
	}
	svg = process.argv[currentarg]
	currentarg++
    }
}
if(svg === undefined){
    console.err("Missing SVG filename.")
    usage()
    return
}

client.on('data', function(data) {
    if (verbose > 0)
	console.log('##########################\n'
		    + 'Client received:\n'
		    + data
		    + '##########################\n')
    json += data
})

client.on('close', function() {
    console.log('Client closed')
})

client.on('error', function(err) {
    console.error(err)
})

app.get('/svg', function (req, res, next) {
    res.setHeader('Content-Type', 'image/svg+xml')
    res.sendFile(path.resolve(__dirname, svg))
})

app.get('/js', function (req, res, next) {
    res.setHeader('Content-Type', 'text/javascript')
    res.sendFile(path.join(__dirname, './assets/script.js'))
})

app.get('/json', function (req, res, next) {
    res.setHeader('Content-Type', 'application/json')
    res.send(json)
})

app.get('/stylecss', function (req, res, next) {
    res.setHeader('Content-Type', 'text/css')
    res.sendFile(path.join(__dirname, './assets/style.css'))
})

app.get('/maincss', function (req, res, next) {
    res.setHeader('Content-Type', 'text/css')
    res.sendFile(path.join(__dirname, './assets/main.css'))
})

app.get('/', function(req, res){
    res.sendFile(path.join(__dirname+'/assets/index.html'))
})

app.get('/bound', async (req, res) =>{
    json = ""
    lastRequest = 'bound'
    await client.write(lastRequest)
    setTimeout(function(){
	res.setHeader('Content-Type', 'application/json')
	res.send(JSON.parse(json))
    }, 500)
})

app.get('/all', async (req, res) =>{
    json = ""
    lastRequest = 'all'
    await client.write(lastRequest)
    setTimeout(function(){
	res.setHeader('Content-Type', 'application/json')
	res.send(json)
    }, 500)
})

app.get('/proc/:filter', async (req, res) =>{
    json = ""
    if (/^\d+$/.test(req.params.filter))
	lastRequest = 'threads pid=' + req.params.filter
    else
	lastRequest = 'name=' + req.params.filter
    await client.write(lastRequest)
    setTimeout(function(){
	res.setHeader('Content-Type', 'application/json')
	res.send(json)
    }, 500)
})

app.get('/threads', async (req, res) =>{
    json = ""
    if(!lastRequest.includes('threads'))
        lastRequest = 'threads ' + lastRequest
    else
        lastRequest = lastRequest.replace('threads ', '')
    await client.write(lastRequest)
    setTimeout(function(){
	res.setHeader('Content-Type', 'application/json')
	res.send(json)
    }, 500)
})

app.get('/lastcpulocation', async (req, res) =>{
    json = ""
    if(!lastRequest.includes('lastcpulocation'))
        lastRequest = 'lastcpulocation ' + lastRequest
    else
        lastRequest = lastRequest.replace('lastcpulocation ', '')
    await client.write(lastRequest)
    setTimeout(function(){
	res.setHeader('Content-Type', 'application/json')
	res.send(json)
    }, 500)
})

client.connect(HPPORT, HOST, function() {
    console.log('Client connected to hwloc-ps JSON server running on ' + HOST + ':' + HPPORT)

    let serv = app.listen(NPORT)

    if(open){
	open("http://localhost:" + NPORT)
    }else{
	console.log("Page is available from http://localhost:" + NPORT)
    }
})
