/* Copyright © 2019 Inria.  All rights reserved. */

function start(svgObject){
    const texts = svgObject.getElementsByTagName('text')
    for (let text of texts) {
	text.setAttribute("style", text.getAttribute("style") + "; pointer-events:none")
    }
}

async function createProcesses(svgdoc, req, json = null){
    await clear(svgdoc)
    clearTable()
    if(!json){
	response = await fetch("/" + req)
	json = await response.json()
    }
    json.forEach(proc => {
	displayProcess(svgdoc, proc, req)
    })
    createTooltip(svgdoc)
    if (/^proc\/\d+$/.test(req)) {
	//only show if single PID
	displayInfo(svgdoc, json)
	//threads auto-enabled for single PID
	document.getElementById('threads').checked = true
    }
}

async function displayProcess(svgdoc, proc, req = null){
    if(!proc.object)
	return
    //create table except when looking at a single process
    if(!/proc\/\d+$/.test(req))
	fillTable(svgdoc, proc)
    bullet(svgdoc, proc, "")
    text(svgdoc, proc, "")
    if(proc.threads){
	proc.threads.forEach(async thread =>{
	    if(thread.object){
		bullet(svgdoc, thread, "thread")
		text(svgdoc, thread, "thread")
	    }
	})
    }
}

function bullet(svgdoc, obj, objType){
    let bulletCircle = svgdoc.getElementById("bulletCircle" + objType + "_" + obj.object.replace(':','_'))
    //if a bullet doesn't already exist, if it does, just add the process id to the bullet attributes
    if(!bulletCircle){
	let id = obj.object.replace(':','_')+"_rect"
	let rect = svgdoc.getElementById(id)

	bulletCircle = svgdoc.createElementNS("http://www.w3.org/2000/svg","rect")

	//difference thread and process
	if(objType === "thread"){
	    bulletCircle.setAttribute("thread", true)
	    bulletCircle.setAttribute("height", "15")
	    bulletCircle.setAttribute("fill", "#7cacf9")
	    bulletCircle.setAttribute("y", rect.y.animVal.value + 12)
	}else{
	    bulletCircle.setAttribute("y", rect.y.animVal.value - 5)
	    bulletCircle.setAttribute("height", "15")
	    bulletCircle.setAttribute("fill", "#FFFFFF")
	}

	//set the bullet in the middle if the process is on machine_O
	if(id == "Machine_0_rect"){
	    const svg = svgdoc.getElementsByTagName('svg')[0]
	    bulletCircle.setAttribute("x", svg.width.baseVal.value / 2 - 10)
	    bulletCircle.setAttribute("y", 5)
	    if(objType === "thread"){
		bulletCircle.setAttribute("x", svg.width.baseVal.value / 2 + 45)
		bulletCircle.setAttribute("y", 5)
	    }
	}else{
	    bulletCircle.setAttribute("x", rect.x.animVal.value-5)
	}
	bulletCircle.setAttribute("rx", 10)
	bulletCircle.setAttribute("ry", 10)
	bulletCircle.setAttribute("circle", true)
	bulletCircle.setAttribute("stroke-width", "1")
	bulletCircle.setAttribute("stroke", "#000000")
	bulletCircle.classList.add("tooltip-trigger")
	rect.parentNode.appendChild(bulletCircle)
	bulletCircle.id = "bulletCircle" + objType + "_" + obj.object.replace(':','_')
	let secondId
	if(objType === "thread")
	    secondId = "thread id : " + obj.PID
	else
	    secondId = "process id : " + obj.PID
	bulletCircle.setAttribute('secondId', secondId)
    }else {
	let secondId = bulletCircle.getAttribute('secondId')
	if(objType === "thread")
	    secondId += ';' + "thread id : " + obj.PID
	else
	    secondId += ';' + "process id : " + obj.PID
	bulletCircle.setAttribute('secondId', secondId)
    }
}

function text(svgdoc, obj, objType){
    let id = obj.object.replace(':','_')+"_rect"
    let rect = svgdoc.getElementById(id)
    let bulletText = svgdoc.createElementNS("http://www.w3.org/2000/svg","text")
    let bulletCircle = svgdoc.getElementById("bulletCircle" + objType + "_" + obj.object.replace(':','_'))

    //if a bullet doesn't already exist, if it does, increment the text
    if(!svgdoc.getElementById("bulletText" + objType + "_" + obj.object.replace(':','_'))){

	bulletText.setAttribute("circle", true)

	//set the text in the middle if the process is on machine_O
	if(id == "Machine_0_rect"){
	    const svg = svgdoc.getElementsByTagName('svg')[0]
	    bulletText.setAttribute("x", svg.width.baseVal.value / 2 - 3)
	    bulletText.setAttribute("y", 15)
	    if(objType === "thread"){
		bulletText.setAttribute("x", svg.width.baseVal.value / 2 + 52)
		bulletText.setAttribute("y", 15)
	    }
	}else{
	    bulletText.setAttribute("x", rect.x.animVal.value+2)
	    bulletText.setAttribute("y", rect.y.animVal.value+6)
	}

	//difference thread and process
	if(objType === "thread" && id != "Machine_0_rect"){
	    bulletText.setAttribute("thread", true)
	    bulletText.setAttribute("y", rect.y.animVal.value+22)
	}

	bulletText.setAttribute("style", bulletText.getAttribute("style") + "; pointer-events:none")
	bulletText.setAttribute("font-size", "10px")
	updateBulletText(bulletText, obj.PID, bulletCircle)
	bulletText.id = "bulletText" + objType + "_" + obj.object.replace(':','_')
	rect.parentNode.appendChild(bulletText)
    }else{
	let text = svgdoc.getElementById("bulletText" + objType + "_" + obj.object.replace(':','_'))
	updateBulletText(text, findPid(text, '+'), bulletCircle)

	//set the text in the middle if the process is on machine_O
	if(id == "Machine_0_rect"){
	    if (text.textContent == 10)
		text.setAttribute("x", parseInt(text.getAttribute('x')) - 3)
	    else if(text.textContent == 100)
		text.setAttribute("x", parseInt(text.getAttribute('x')) - 4)
	    return
	}
    }
}

async function updateBulletText(text, pid, bulletCircle){
    text.textContent = await pid
    let length = text.getComputedTextLength()
    bulletCircle.setAttribute("width", length + 15)
}

function findPid(text, op){
    let nb
    if(!text.getAttribute('nb')){
	nb = "[2]"
	text.setAttribute('nb', nb)
    }else{
	nb = text.getAttribute('nb')
	nb = nb.replace('[','')
	nb = nb.replace(']','')
	if(op == '+')
	    nb = parseInt(nb) + 1
	else{
	    nb = parseInt(nb) - 1
	}
	nb = '[' + nb + ']'
	text.setAttribute('nb', nb)
    }
    return nb
}

function clear(svgdoc){
    document.getElementById('searchId').value = ''
    const info = document.getElementById('info')
    info.classList = 'hidden'
    const infoNotFound = document.getElementById('info-not-found')
    infoNotFound.classList = 'hidden'
    svgElements = svgdoc.getElementsByTagName('svg')[0].children
    let i = 0;
    for(element of svgElements){
	if(element.id.includes('bulletCircle'))
	    i += 2
    }
    if(i == 0)
	return
    for(i ; i >= 0 ; i--){
	svgElements[svgElements.length - 1].remove()
    }
}

function createTooltip(svgdoc){
    if(svgdoc.getElementById('tooltip'))
	svgdoc.getElementById('tooltip').remove()
    //Create tooltip to display process id
    const g = svgdoc.createElementNS("http://www.w3.org/2000/svg","g")
    const blackRect = svgdoc.createElementNS("http://www.w3.org/2000/svg",'rect')
    const whiteRect = svgdoc.createElementNS("http://www.w3.org/2000/svg",'rect')
    const tooltip = svgdoc.createElementNS("http://www.w3.org/2000/svg",'text')

    blackRect.setAttribute('fill','black')
    blackRect.setAttribute('opacity','0.4')
    blackRect.setAttribute('rx','2')
    blackRect.setAttribute('ry','2')
    blackRect.setAttribute('x','2')
    blackRect.setAttribute('y','-8')
    blackRect.setAttribute('height','11')
    blackRect.setAttribute('opacity','0.4')
    whiteRect.setAttribute('y','-10')
    whiteRect.setAttribute('fill','white')
    whiteRect.setAttribute('rx','2')
    whiteRect.setAttribute('ry','2')
    whiteRect.setAttribute('height','11')
    tooltip.setAttribute('x','4')
    tooltip.setAttribute('y','6')

    g.setAttribute('visibility', 'hidden')
    g.id = "tooltip"

    g.appendChild(blackRect)
    g.appendChild(whiteRect)
    g.appendChild(tooltip)
    svgdoc.getElementsByTagName('svg')[0].appendChild(g)

    let triggers = svgdoc.getElementsByClassName('tooltip-trigger')
    for (let i = 0; i < triggers.length; i++) {
	triggers[i].onmousemove = showTooltip
	triggers[i].onmouseout = hideTooltip
	triggers[i].onclick = async function(){
		//on click : display process info for the first process in the bullet
		procs = triggers[i].getAttribute('secondId').split(";")
		let data = new Array()
		response = await fetch("/json")
	json = await response.json()
	await procs.forEach(proc => {
		json.forEach(jsonProcess => {
			if(proc.substring(13) == jsonProcess.PID)
				data.push(jsonProcess)
		})
	})
	let req = data.length == 1 ? "proc/" + data[0].PID : null
	createProcesses(svgdoc, req, data)
	}
    }

    function showTooltip(evt) {
	const tooltip = svgdoc.getElementById('tooltip')
	let tooltipText = tooltip.getElementsByTagName('text')[0]
	let tooltipRects = tooltip.getElementsByTagName('rect')

	//clear tooltip
	while(tooltipText.firstChild){
	    tooltipText.removeChild(tooltipText.firstChild)
	    for (let i = 0; i < tooltipRects.length; i++) {
		tooltipRects[i].setAttribute("height", parseInt(tooltipRects[i].getAttribute("height")) - 15)
	    }
	}

	//display all processes id in the bullet
	let text = evt.target.getAttribute('secondId')
	let tspan
	text = text.split(';')
	let length = 0
	for (str of text ) {
	    //create the text element
	    tspan = svgdoc.createElementNS("http://www.w3.org/2000/svg",'tspan')
	    if(text.indexOf(str) !== 0)
		tspan.setAttribute('dy', '1em')
	    tspan.setAttribute('x', '0')
	    tspan.textContent = str
	    tooltipText.appendChild(tspan)
	    //find the length of the longest text
	    if(tspan.getComputedTextLength() > length)
		length = tspan.getComputedTextLength()
	    //ajust the height of the tooltip element ( and its shadow )
	    for (let i = 0; i < tooltipRects.length; i++) {
		tooltipRects[i].setAttribute("height", parseInt(tooltipRects[i].getAttribute("height")) + 15)
	    }
	    //display "..." if there is more than 5 processes
	    if(text.indexOf(str) === 5){
		tspan.textContent = '\u00A0\u00A0\u00A0\u00A0\u00A0\u00A0\u00A0\u00A0\u00A0\u00A0\u00A0\u00A0\u00A0...'
		tooltipText.appendChild(tspan)
		break
	    }
	}
	//set the tooltip ( and its shadow ) width according to the longest element
	for (let i = 0; i < tooltipRects.length; i++) {
	    tooltipRects[i].setAttributeNS(null, "width", length + 8)
	}
	//display the tooltip at the mouse coordonates
	let CTM = svgdoc.getElementsByTagName('svg')[0].getScreenCTM()
	let x = (evt.clientX - CTM.e + 6) / CTM.a;
	let y = (evt.clientY - CTM.f + 20) / CTM.d;
	tooltip.setAttributeNS(null, "transform", "translate(" + x + " " + y + ")");
	tooltip.setAttributeNS(null, "visibility", "visible")
    }

    function hideTooltip() {
	const tooltip = svgdoc.getElementById('tooltip')
	tooltip.setAttributeNS(null, "visibility", "hidden")
    }
}

async function displayInfo(svgdoc, data){
    //handle 2 div : one if the process is found, and one if it isn't
    const info = document.getElementById('info')
    const infoNotFound = document.getElementById('info-not-found')
    info.classList = 'hidden'
    infoNotFound.classList = 'hidden'

    if(!data[0].object){
	infoNotFound.classList.toggle('hidden')
	return
    }
    const infos = info.children[0].children
    infos[0].innerHTML = 'Process: ' + data[0].PID
    infos[1].innerHTML = 'Name: ' + data[0].name
    infos[2].innerHTML = 'Object: ' + data[0].object.replace(':',' L#')
    if(data[0].threads){
	infos[3].innerHTML = 'Threads: ' + (data[0].threads.length - 1)
    }else{
	infos[3].classList = 'hidden'
    }
    info.classList.toggle('hidden')
}

function fillTable(svgdoc, proc){
    document.getElementById('processTable').classList = "table100 ver2 m-b-110"
    const table = document.getElementById('table')
    const tr = document.createElement('tr')
    const td = document.createElement('td')
    const td2 = document.createElement('td')
    const td3 = document.createElement('td')
    const checkbox = document.getElementById('mainCheck')

    tr.classList.add("row100", "head")
    td.classList.add("cell100", "column1")
    td2.classList.add("cell100", "column1")
    td3.classList.add("cell100", "column1")

    tr.setAttribute("object", proc.object)
    td.innerHTML = proc.PID
    td2.innerHTML = proc.name
    td3.innerHTML = "<input class='subCheck' checked='true' type='checkbox'>"
    td3.children[0].addEventListener('change', displayTableElement)
    checkbox.checked = true
    checkbox.onchange = changeAll

    tr.appendChild(td)
    tr.appendChild(td2)
    tr.appendChild(td3)
    table.appendChild(tr)

    //toggle dispay - hide according to checkbox value
    function displayTableElement(event){
	const tr = event.target.parentElement.parentElement
	const tds = tr.children
	const proc = { PID : tds[0].innerHTML, name : tds[1].innerHTML, object : tr.getAttribute('object') }
	if(event.target.checked == false){
	    const element = svgdoc.getElementById("bulletCircle" + "_" + proc.object.replace(':','_'))
	    removeProc(element, proc)
	}else{
	    bullet(svgdoc, proc, "")
	    text(svgdoc, proc, "")
	    createTooltip(svgdoc)
	}
    }

    function removeProc(element, proc){
	let id = element.id
	let text = svgdoc.getElementById(id.replace('bulletCircle', 'bulletText'))
	let secondId = element.getAttribute('secondId').replace('process id : ' + proc.PID + ';', '')
	if(!text.getAttribute('nb') || text.getAttribute('nb') == '[1]'){
	    text.remove()
	    element.remove()
	}else{
	    updateBulletText(text, findPid(text, '-'), element)
	}
	element.setAttribute('secondId', secondId)
    }

    //handle click on main checkbox -> set all checkbox value to its value
    function changeAll(event){
	const checkboxes = document.getElementsByClassName('subCheck')
	size = checkboxes.length
	for(let i = 0 ; i < size ; i++ ){
	    if(checkboxes[i].checked != event.target.checked){
		checkboxes[i].checked = event.target.checked
		displayTableElement({target: checkboxes[i]})
	    }
	}
    }
}

function clearProc(){
    document.getElementById('searchId').value = ""
    const checkboxes = document.getElementsByClassName('checkbox')
    for( let i = 0 ; i < checkboxes.length ; i++ ){
        checkboxes[i].checked = false
    }

    let svgObject = document.getElementById('svg-object').contentDocument
    clear(svgObject)
}

function clearTable(){
    const table = document.getElementById("table");
    while (table.firstChild) {
        table.removeChild(table.firstChild);
    }
    document.getElementById("processTable").classList = "table100 ver2 m-b-110 hidden"
}

function handleProcButton(){
    let functionIsRunning = false
    //use function is running so that the function doesn't run twice if the user double click.
    async function addProc(req){
        if (!functionIsRunning) {
            functionIsRunning = true
            if(req == 'bound' || req == 'all'){
		clearProc()
		clearTable()
            }
            let svgObject = document.getElementById('svg-object').contentDocument
            await createProcesses(svgObject, req)
            functionIsRunning = false
        }
    }

    const procButtons = document.getElementsByClassName('procButton')
    for(let i = 0 ; i < procButtons.length ; i++){
        procButtons[i].addEventListener('click', function(){
            addProc(procButtons[i].type)
        })
    }
}

function handleCheckbox(){
    const checkboxes = document.getElementsByClassName('checkbox')
    for( let i = 0 ; i < checkboxes.length ; i++ ){
        checkboxes[i].checked = false
        checkboxes[i].addEventListener('change',function(e){
            clearTable()
            let svgObject = document.getElementById('svg-object').contentDocument
            createProcesses(svgObject, e.target.id)
        })
    }
}

function procByid(){
    let svgObject = document.getElementById('svg-object').contentDocument
    let id = document.getElementById('searchId').value
    clearProc()
    createProcesses(svgObject, 'proc/'+id)
}

//time out so that the svg has the time to load
setTimeout(function() {
    handleCheckbox()
    handleProcButton()
    const input = document.getElementById('searchId')
    input.value = ''
    input.addEventListener('keyup',function(event){
        if(event.keyCode === 13){
            event.preventDefault()
            procByid()
        }
    })

    let svgObject = document.getElementById('svg-object').contentDocument
    let svgElement = svgObject.getElementById('Machine_0_rect')

    if(svgObject.getElementById('Machine_0_rect')){
        createProcesses(svgObject, 'bound')
        start(svgObject)
    }else{
        const h1 = document.createElement('h1')
        h1.innerHTML = "Your svg file doesn't have the good format to load javascript"
        document.body.appendChild(h1)
    }
}, 200)
