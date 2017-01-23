# iot-fullstack
> An application intended to give a quick introduction into a complete IoT-Stack.

This project is seperated into 6 parts:
- The Hardware [Node](node)
- An application registered to [The things network](http://thethingsnetwork.org)
- A [payload decoder](payload-decoder.js) for thethingsnetwork
- A node-red flow `node-red`
- The feather-js backend to store and retrieve data [Server](server)
- A client application [client](client)

# Installation
1. Register an Appliaction at the thingsnetwork and add new device to the appliaction
2. Add [payload decoder](payload-decoder.js) to application
3. Change credentials in the [node](node) and program the device
4. Install [Node-red](http://nodered.org/)
5. Import [noder-red flow](node-red)
6. Install and start [Server](server)
7. Install and start [Client](client)
8. Connect to client
