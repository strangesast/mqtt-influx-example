const http = require('http');
const express = require('express');
const WebSocket = require('ws');
const path = require('path');
const mqtt = require('mqtt')

const app = express();
const port = process.env.PORT || 3000;

app.get('/', (req, res) => {
  res.sendFile(path.join(__dirname, 'index.html'));
})

const server = http.createServer(app);

const wss = new WebSocket.Server({ server, path: '/ws' });

const mqttHost = process.env.MQTT_HOST || 'localhost';
const mqttPort = process.env.MQTT_PORT || 1883;
const mqttClient = mqtt.connect({servers: [{host: mqttHost, port: mqttPort}]});

mqttClient.on('connect', () => {
  wss.on('connection', (ws) => {
    console.log(`new connection!`);
    ws.on('message', (msg) => {
      mqttClient.publish('leds', msg);
    });
  });
});


server.on('close', () => mqttClient.end());

server.listen(port, () => {
  console.log(`listening at :${port}`)
})


const shutdown = () => {
  console.log('closing...');
  server.close(() => {
    console.log('closed');
  })
}

process.once('SIGTERM', shutdown);
process.once('SIGINT', shutdown);
