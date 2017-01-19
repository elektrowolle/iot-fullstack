'use strict';

const hooks = require('./hooks');

class Service {
  constructor(drawer) {
    this.drawer = drawer || {};
  }

  find(params) {
    return Promise.resolve("Unsupported");
  }

  get(id, params) {
    return Promise.resolve("Unsupported");
  }

  create(data, params) {
    console.log("params", params);
    if(Array.isArray(data)) {
      return Promise.all(data.map(current => this.create(current)));
    }

    this.drawer.get(data.drawer_id)
      .then(drawer=>{
        if(!drawer.values) drawer.values = [];
        drawer.values.push({
          timestamp: (new Date()).valueOf(),
          values:data.values
        });
        this.drawer.patch(data.drawer_id, drawer);
      });

    return Promise.resolve(data);
  }

  update(id, data, params) {
    return Promise.resolve(data);
  }

  patch(id, data, params) {
    return Promise.resolve(data);
  }

  remove(id, params) {
    return Promise.resolve({ id });
  }
}

module.exports = function(){
  const app = this;
  const drawer = app.service('drawers');

  // Initialize our service with any options it requires
  app.use('/drawerData', new Service(drawer));

  // Get our initialize service to that we can bind hooks
  const drawerDataService = app.service('/drawerData');

  // Set up our before hooks
  drawerDataService.before(hooks.before);

  // Set up our after hooks
  drawerDataService.after(hooks.after);
};

module.exports.Service = Service;
