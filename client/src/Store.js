const feathers = require('feathers/client');
const socketio = require('feathers-socketio/client');
const hooks = require('feathers-hooks');
const io = require('socket.io-client');

import Vuex from "vuex"
import Vue  from 'vue'
Vue.use(Vuex);

const socket = io('http://localhost:3030');

const app = feathers()
    .configure(hooks())
    .configure(socketio(socket));

const messageService = app.service('drawers');

class Drawer{
  constructor({name, eui, occupied}) {
    this._id      = eui;
    this.name     = name;
    this.eui      = eui;
    this.occupied = occupied;
    this.values   = [];
    this.load     = 0;
  }
}

function findDrawer(id, drawers){
    return drawers.find( drawer => drawer.eui == id )
}

const store = new Vuex.Store({
    state: {
        drawers: [],
        lastUpdate: new Date(),
    },
    mutations:{
        addDrawer(state, payload){
            state.drawers.push(payload);
        },

        updateDrawer(state, payload){
            let id      = payload._id;
            let drawers = state.drawers;

            let drawer = findDrawer(id, state.drawers)
                || state.drawers[state.drawers.push({}) - 1];

            Object.assign(drawer, payload);

            let lastValues = drawer.values ? drawer.values[drawer.values.length - 1] : undefined;
            drawer.load = lastValues ?
                lastValues.values[0] +
                lastValues.values[1] +
                lastValues.values[2] +
                lastValues.values[3] : 0;

            state.drawers = Array.from(state.drawers);
            state.lastUpdate = new Date();

            console.log("data for", id, drawers);
        },
    },

    actions:{
        fetchDrawers(context){
            messageService.find().then(res=>{
                console.log("fetch result:", res);
                res.data.map(
                    drawer => context.commit('updateDrawer', drawer)
                );
            });
        },

        updateDrawer(context, payload){
            console.log(payload);


            let drawer = findDrawer(payload.eui, context.state.drawers);
            let newDrawer = {};
            Object.assign(newDrawer, drawer);
            Object.assign(newDrawer, payload);

            console.log("update", newDrawer);
            messageService.patch(payload._id, payload).then(
                        res => context.commit('updateDrawer', res)
            );
        },

        addDrawer(context, payload){
            console.log("create new");
            messageService.create(new Drawer(payload)).then(
                        res => context.commit('updateDrawer', res)
            );
        }
    },

});

messageService.on('created', function(drawer) {
    console.log('received drawer', drawer);
    store.commit('updateDrawer', drawer);
});

messageService.on('patched', function(drawer) {
    console.log('received drawer', drawer);
    store.commit('updateDrawer', drawer);
});

export default store;
