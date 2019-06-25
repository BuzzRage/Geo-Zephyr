var req= new XMLHttpRequest();
setTimeout(process, 500);

function process(){
  if(req.readyState==0 || req.readyState==4){
    req.open('GET','/sensors.json',true);
    req.onreadystatechange=handleServerResponse;
    req.send(null);
  }
}

function handleServerResponse(){
  if(req.readyState==4 && req.status==200){
    ans = JSON.parse(req.response);
    document.getElementById('pressure').innerHTML = ans.pressure;
    document.getElementById('temperature').innerHTML = ans.temperature;
    document.getElementById('humidity').innerHTML = ans.humidity;
    document.getElementById('gas').innerHTML = ans.gas;
    setTimeout(process,1000);
  }
}
