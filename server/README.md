# iot-fullstack-backend
This Server is intended to provide interfaces for office-stock related data.

## About

This project uses [Feathers](http://feathersjs.com).

## Getting Started

3. Start your app
``` bash
# install dependencies
npm install
npm start
```
    
## API
The api provides an socket.io and REST interface.

`/devices/[id]` \[CRUD\] interface to be used for device registration and device data.
Example Device:
```
{
  "name": "Pencil",
  "_id": "pencil-tracker",
  "values": [
    {"timestamp": "xxxxxxx", "values":[0,1,2,3]},
    {"timestamp": "xxxxxxx", "values":[0,1,2,3]},
    {"timestamp": "xxxxxxx", "values":[0,1,2,3]},
  ]
}
```

`/deviceData/` \[C\] interface to append data to an interface. Example Data.
