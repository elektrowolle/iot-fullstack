const feathers = require('feathers/client');
const socketio = require('feathers-socketio/client');
const hooks = require('feathers-hooks');
const io = require('socket.io-client');

import Vuex from "vuex"
import Vue from 'vue'
Vue.use(Vuex)

const socket = io('http://localhost:3030');

const app = feathers()
  .configure(hooks())
  .configure(socketio(socket));

const messageService = app.service('messages');

messageService.on('created', function(message) {
  console.log('Someone created a message', message);
});

// Use the messages service from the server
messageService.create({
  text: 'Message from client'
});

class Drawer{
  constructor({name, eui, occupied}) {
    this.name  = name;
    this.eui   = eui;
    this.occupied = occupied;
  }
}

export default new Vuex.Store({
    state: {
        drawers: [],
    },
    mutations:{
        addDrawer(state, payload){
            state.drawers.push(new Drawer(payload));
        },

        updateDrawer(state, payload){
            let drawer = state.drawers[payload.id];

            drawer.name     = payload.name     || drawer.name;
            drawer.eui      = payload.eui      || drawer.eui;
            drawer.occupied = payload.occupied || drawer.occupied;
        }
    },
    actions:{
        login(){

        }
    }
});