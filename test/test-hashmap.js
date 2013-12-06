'use strict';

var assert = require('assert');
var HashMap = require('../index');

var data = {
  'int32': [0, 1],
  'string': ['fee', 'fi']
};

[
  { keyType: 'int32', valueType: 'int32' },
  { keyType: 'int32', valueType: 'string' },
  { keyType: 'string', valueType: 'int32' },
  { keyType: 'string', valueType: 'string' }
].forEach(function (it) {
  var keyType = it.keyType;
  var valueType = it.valueType;

  suite('hashmap-' + keyType + '-' + valueType, function() {
    var map;

    setup(function() {
      map = new HashMap[keyType][valueType]();
    });

    test('put(), get(), contains()', function () {
      var key = data[keyType][0];
      var value = data[valueType][0];

      // put new element
      map.put(key, value);

      // validate that the new element was added
      assert(map.get(key) === value);
      assert(map.contains(key));

      // test missing element
      assert(!map.contains(data[keyType][1]));
    });

    test('remove()', function () {
      var key = data[keyType][0];
      var value = data[valueType][0];

      map.put(key, value);
      assert(map.contains(key));

      map.remove(key);
      assert(!map.contains(key));
    });

    test('size()', function () {
      var key1 = data[keyType][0];
      var value1 = data[valueType][0];

      var key2 = data[keyType][1];
      var value2 = data[valueType][1];

      assert(map.size() === 0);

      map.put(key1, value1);
      assert(map.size() === 1);

      map.put(key2, value2);
      assert(map.size() === 2);
    });

    test('clear()', function () {
      var key = data[keyType][0];
      var value = data[valueType][0];

      assert(map.size() === 0);

      map.put(key, value);
      assert(map.size() === 1);

      map.clear();
      assert(map.size() === 0);
    });

    test('empty()', function () {
      var key = data[keyType][0];
      var value = data[valueType][0];

      assert(map.empty());

      map.put(key, value);
      assert(!map.empty());
    });
  });
});
