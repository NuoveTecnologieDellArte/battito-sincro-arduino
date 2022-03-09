// Server
const PORT = 1883
const TOPIC = 'main'

var aedes = require('aedes')()
var server = require('net').createServer(aedes.handle)
var timesync = require('timesync');

var ts = timesync.create({
  server: 'http://battito.cuoredinapoli.net/timesync',
  interval: 3000
});

// helper function to log date+text to console:
const log = (text) => {
  console.log(`[${new Date().toLocaleString()}] ${text}`)
}

// client connection event:
aedes.on(
  'client',
  (client) => {
    let message = `Client ${client.id} just connected`
    log(message)
    aedes.publish({
      cmd: 'publish',
      qos: 2,
      topic: TOPIC,
      payload: message,
      retain: false
    })
  }
)

//client disconnection event:
aedes.on(
  'clientDisconnect',
  (client) => {
    message = `Client ${client.id} just DISconnected`
    log(message)
    aedes.publish({
      cmd: 'publish',
      qos: 2,
      topic: 'main',
      payload: message,
      retain: false
    })
  }
)

setInterval(function () {
  var now = new Date(ts.now());
  if((now.getSeconds()%3==1) && (counter == 0)){
    counter = 1;
    aedes.publish({     
      cmd: 'publish',
      qos: 0,
      topic: 'main',
      payload: 'test',
      retain: false
    });
  } else if (now.getSeconds()%3!=1){
    counter = 0;
  }
}, 1);

server.listen(PORT, function () {
  console.log(`server listening on port ${PORT}`)
})