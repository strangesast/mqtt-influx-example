var mqtt = require('mqtt')
const host = 'localhost'
const port = 1883
var client  = mqtt.connect({servers: [{host, port}]});

client.on('connect', function () {
    const fn = i => {
        const msg = `Hello mqtt (${i})`;
        console.log(`writing msg: ${msg}`);
        client.publish('leds', msg, {qos: 1}, () => setTimeout(() => fn(i + 1), 1000));
    };
    fn(0);
});

client.on('message', function (topic, message) {
    console.log(`topic: ${topic} message: ${message.toString()}`);
    client.end();
})
