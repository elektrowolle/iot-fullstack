'use strict';

const assert = require('assert');
const app = require('../../../src/app');

describe('drawerData service', function() {
  it('registered the drawerData service', () => {
    assert.ok(app.service('drawerData'));
  });
});
