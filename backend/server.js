const WebSocket = require('ws');
const mqtt = require('mqtt');

const wss = new WebSocket.Server({ port: 8080 });

const mqttServer = 'mqtt://usm.revolusi-it.com';
const mqttUser = 'usm';
const mqttPassword = 'usmjaya24';
const mqttTopic = 'test/test';

const mqttClient = mqtt.connect(mqttServer, {
    username: mqttUser,
    password: mqttPassword
});

mqttClient.on('connect', () => {
    console.log('Connected to MQTT server');
    mqttClient.subscribe(mqttTopic);
});

mqttClient.on('message', (topic, message) => {
    if (topic === mqttTopic) {
        const jsonMessage = message.toString();
        console.log('Received:', jsonMessage);
        // Broadcast message to all WebSocket clients
        wss.clients.forEach(client => {
            if (client.readyState === WebSocket.OPEN) {
                client.send(jsonMessage);
            }
        });
    }
});

wss.on('connection', ws => {
    console.log('New WebSocket connection');
    ws.on('message', message => {
        console.log('Received via WebSocket:', message);
        mqttClient.publish(mqttTopic, message);
    });
});


