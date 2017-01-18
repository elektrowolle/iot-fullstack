'use strict';

const assert = require('assert');
const app = require('../../../src/app');

describe('drawer service', function() {
  it('registered the drawers service', () => {
    assert.ok(app.service('drawers'));
  });
});
