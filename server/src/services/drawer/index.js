'use strict';

const path = require('path');
const NeDB = require('nedb');
const service = require('feathers-nedb');
const hooks = require('./hooks');

module.exports = function(){
  const app = this;

  const db = new NeDB({
    filename: path.join(app.get('nedb'), 'drawers.db'),
    autoload: true
  });

  let options = {
    Model: db,
    paginate: {
      default: 5,
      max: 25
    }
  };

  // Initialize our service with any options it requires
  app.use('/drawers', service(options));

  // Get our initialize service to that we can bind hooks
  const drawerService = app.service('/drawers');

  // Set up our before hooks
  drawerService.before(hooks.before);

  // Set up our after hooks
  drawerService.after(hooks.after);
};
