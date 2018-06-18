const mqtt = require('mqtt');
const config = require('./config');

exports.handler = function (e, ctx, callback) {
    const client = mqtt.connect(config.mqttURL, {
        port: config.mqttPort,
        username: config.mqttUsername,
        password: config.mqttPassword
    });

    client.on('connect', () => {
        client.publish('royesp01/02/dev', JSON.stringify(e));
        client.end(callback(null, {
            status: 'OK',
            message: JSON.stringify(e)
        }));

    });

    client.on('error', (error) => {
        client.end(callback(null, {
            status: 'ERROR',
            message: error
        }));
    });

    /* client.on('message', (topic, message) => {
        // message is Buffer
        console.log(message.toString());
        client.end();
    }); */
};