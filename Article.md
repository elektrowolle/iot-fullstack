Lora
====
IoT is about traffic lights talking to cars, a fridge which orders cheese on
it’s own and [example]. IoT means everything is connected to each other all the
time. Constantly devices exchange data and it’s properties will be analysed in
the cloud in real time. But how often is the constant connection between n:n
really needed? I will explain the properties of LoRa and IoT projects using an
example.

Keeping track of stocked items in an office is a huge burden. There is a large
number of individual items and often some of them won’t be used for weeks or
months. Others are used so regularly that after a glimpse all of them are gone.
Keeping track of those items saves money without doubt as every little search
for a pen is another step into delay and unproductive use of time.

One way to proceed is to lock your office items up and make everyone “order” it
from the office manager. Another way might be a list and everyone who takes
something and notice a soon lack of this item is requested to leave a note. But
as this article is about IoT and LoRa in specific, it won’t be a surprise to
use a Lora based sensor network to help stocking up items.

Most sensors placed in the environment are not delivering a series of
constantly changing data. And relevant properties of data aren’t pictures of
cheese or the tone of the yellow of the light indicating to be aware of a soon
change but an indicating id 3 for the category of milk products and a 55 for
for your favorite cheese. Most data in our daily life is mostly constant and
quantifiable into small amounts of states.

LoRa only allows to transmit very small data packages. Up to 51 bytes length
which is about a 3rd of a twitter message. Sending data encoded as text (like
json) is almost impossible that way. Data should be wrapped bitwise. Luckily
this is a common process for C developers.

The trivial finding of small amounts of really needed data matters when we
think about networks. Especially networks which are supposed to connect
everything with everything. Networks for the IoT. A good assumption of what an
IoT device really is is to compare it with our Smartphones. Smartphones are
connected with their near environment via Bluetooth and WiFi. For communication
just in a few centimeters NfC is used and to connect outside the owners own
perimeters it is essential to use a paid service from a provider offering you
to use their cell based network. All these networks serve a purpose for the
many ways we smartphones can be used.

Those networks used by smartphones are all great and provide us with an reality
unthinkable just a few years earlier. But these technologies also come with
major drawbacks which make them unusable for many IoT applications. WiFi and
Cell Networks use big amount of power. We notice that every time we find our
selves in a different part of the world where we can’t or don’t want to access
the internet. Suddenly our phones seem not to need a charge ever. WiFi is
pretty limited in reach. And everybody attending big events with a free “WiFi”
knows that those are hardly providing the stability and performance we expect.
The Cell-based networks come with immense costs.

Different to smartphones the IoT world can be viewed as many exploded
smartphones. For example: a connected peppermill might need to transmit exactly
one kind of data: how much pepper is left. The only two possible states of a
door would be either closed or open. A bike stand is either occupied or not.
This small amount of quantified data is a huge portion of everyday trackable
data. But why aren’t all these small devices connected yet? The reason why not
every single shoe lace talks back home to notify it’s owner or producer that it
needs to be replaced is reasoned by the fact that the networks we use at the
moment are not meant to be used by so many devices and they are power hungry.
Lora is a network technology designed to use a small amounts of power to
communicate short messages at a very low rate in a stable fashion. Together
with LoraWAN it forms a full cell-based end to end network stack. It Lora
utilizes free ISM bands and has a reach up to 15km. Gateways cost from 200€ and
node-modules for tinkering start at 30€. Nodes are not directly connected to
the internet similar to bluetooth applications. The gateways are connected to
individual “Network Servers” which forward messages further down to network
servers. Communication between node and network server can be encrypted and
doesn’t need to be unwrapped on gateways allowing a secure connection.
Software for Gateways and Networkservers are available open source and
libraries for the Lora-Founder Semtech own sx12 chipset series are avaible for
embedded plattforms. Applications can can access the data from the
Network-Servers via messaging protocols like MQTT.

For our example this means we have to set up a few devices and services before
we can process our data. This load of abstractions before our data is actually
coming into our own hands can be a bit annoying and time consuming. Though it
helps with handling the huge amount of devices which now can be connected to
your services and often ca be handled as if it would be one device.
Exploring a new technology is always embedded into a context of the explorer.
When we create a development platform it is mandatory to think about the
context in which tests will be made what the purpose of exploring the
technology is. The selection of interfaces and toolchain must be in harmony
with it’s later use. Will the device be tested outside in the field for long
term observation or is it used as a placeholder data source for a backend? Will
it need to work autonomous or will it be the interface to some computer vision
systems frontend?

The example provided serves the purpose to show how a full stack from hardware
to user interface is built up. It is not to be seen as best practice nor as an
example of good system-architecture. Though the example shows what is needed to
to build up a service with a fairly big amount of devices and users.

Hands on
--------

The whole project is available at https://github.com/elektrowolle/iot-fullstack/

The things we need are:
1.    A sensor to track the amount or at least the presence of items
2.    A device to transmit this data
3.    A Gateway or Service Provider forwarding this data to the internet
4.    A service to handle incoming data
5.    A tool to keep track of the items and visualize data
For a prototype I suggest the following implementation (numbers map):
1.    A flat load sensor attached to sheets of plastic
2.    Arduino Zero with the Dragino Lora shield
3.    Raspberry Pi with the Linklabs Lora Hat
4.    The things network
5.    A node-red Server as some software glue between the things-network and
the backend
6.    Feathersjs Backend + vuejs Frontend

To track the amount of items we will use force/load sensors. The load sensor is
connected to the Arduino. The Lora shield is attached on top. We use the the
Built-in realtime clock to make measurements only every 3 hours. The
measurements will be then transmitted to our gateway.

To be able to transmit our data it is needed to prepare a few things. Connect a
gateway to the things network and open a new the things network project. To use
the Gateway we have to install the semtech packetforwarder. After logging in to
the thingsnetwork a new gateway needs to be registered. The gateway then has to
be configured to transmit to one of the thethingsnetwork router. 

IBM provides a piece of sotware called lmic short for LoRaWan in C to to
connect our device to the gateway. To use it on an Arduino a few
implementations have to be made, but gladly there is already done by
things-nyc. It needs to expanded to read out the force sensors and transform
the data into something transmittable.

The thethings-application then has to be taught how to understand this data.
After that we can access it from a node-red flow and transmit it to our
application(s). Our application is a simple CRUD server based on feathersjs.
Feathersjs then provides an socketio interface to access the data from an
vue-js based interface.

Wrap Up
-------
LoRa is a big step into a real ‘everything is connected’ - world as it provides
the right set of properties.

IoT projects depend on a number of skills which sometimes make them hard to
prototype. They depend on knowledge about electronics embedded-software,
networking, real-time protocols, databases, data-analytics and front-end. It is
often possible to skip many steps in a prototyping scenario. If for example
only one device is needed as proof of concept the data usually can be send
directly from device to fron-end. But some technologies, LoRa is one of them,
doesn’t allow this kind of short-cut. And for holistic consulting about IoT the
whole stack has to be considered anyway.

As IXDS we help designing services utilizing IoT technologies. We set up test
environments and make prototypes for show case or field test scenarios. New
technologies need to be tested and understood prior to deployment. IXDS can
help developing flexible test system with the focus on service development
rather then pure technology demonstrators.
