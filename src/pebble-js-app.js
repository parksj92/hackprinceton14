function postStats(time, category) {
  console.log('Entered');
  //{"user":null,"date":"2014-11-16","activity_type":"Home","duration":1}
  var params = 'activity[date(1i)]=2014&activity[date(2i)]=November&activity[date(3i)]=16&activity[activity_type]=' + category + '&activity[duration]=' + time.toFixed(2);
  //var params = '{"activity": {"date":"2014-11-16", "activity_type":"' + category + '" , "duration":' + time.toFixed(2)+'}}' ;
  //console.log('JSON made');
  //player[name]=value
  //var params = JSON.stringify({user: null, date: "2014-11-16", activity_type: category, duration: time.toFixed(2)});
  //var params = JSON.parse(text);
  console.log('JSON parsed: ' + params);
  
  var http = new XMLHttpRequest();
  var url = 'https://polar-wave-9322.herokuapp.com/activities';
  //var params = JSON.stringify({ appoverGUID: approverGUID });
  http.open("POST", url, true);
  http.setRequestHeader("Content-type", "application/x-www-form-urlencoded");
  http.setRequestHeader("Content-length", params.length);
  http.setRequestHeader("Connection", "close");
  console.log('Headers set');
  
  http.onreadystatechange = function() {
    console.log('Inside');
    if(http.readyState == 4 && http.status == 200) {
        console.log(http.responseText);
    }
    else{
      console.log("State: " + http.readyState + "Status: " + http.status);
    }
  };
  http.send(params);

}

// Set callback for the app ready event
Pebble.addEventListener("ready", function(e) {
  console.log("connected! " + e.ready);
});

// Set callback for appmessage events
Pebble.addEventListener("appmessage", function(e) {
  console.log("message");
  console.log(e.payload);
  var time = 0, category = "";
  if(! e.payload.status){
    return;
  }
  if (e.payload.time) {
    time = e.payload.time;
  }
  console.log("Time: " + time);
  if (e.payload.category) {
     category = e.payload.category;
  }
  console.log("Cat:" + category);
  postStats(time, category);
});