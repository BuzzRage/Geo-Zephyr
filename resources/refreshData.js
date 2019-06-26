var req= new XMLHttpRequest();
setTimeout(process, 1000);

function process(){
  if(req.readyState==0 || req.readyState==4){
    req.open('GET','/sensors.json',true);
    req.onreadystatechange=handleServerResponse;
    req.send(null);
  }
  setTimeout(process,5000);
}

function handleServerResponse(){
  if(req.readyState==4 && req.status==200){
    ans = JSON.parse(req.response);
    document.getElementById('pressure').innerHTML = ans.pressure;
    document.getElementById('temperature').innerHTML = ans.temperature;
    document.getElementById('humidity').innerHTML = ans.humidity;
    document.getElementById('gas').innerHTML = ans.gas;
    document.getElementById('csventry').innerHTML = ans.csventry;
  }
}

function getCSV(){
  if(req.readyState==0 || req.readyState==4){
    req.open('GET','/data.csv',true);
    req.send(null);
  }
}
