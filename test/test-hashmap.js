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
  var keys = data[it.keyType];
  var values = data[it.valueType];

  suite('hashmap-' + it.keyType + '-' + it.valueType, function() {
    var map;

    setup(function() {
      map = new HashMap[it.keyType][it.valueType]();
    });

    test('put(), get(), contains()', function () {
      var key = keys[0];
      var value = values[0];

      // put new element
      map.put(key, value);

      // validate that the new element was added
      assert(map.get(key) === value);
      assert(map.contains(key));

      // test missing element
      assert(!map.contains(keys[1]));
    });

    test('remove()', function () {
      var key = keys[0];
      var value = values[0];

      map.put(key, value);
      assert(map.contains(key));

      map.remove(key);
      assert(!map.contains(key));
    });

    test('size()', function () {
      var key1 = keys[0];
      var value1 = values[0];

      var key2 = keys[1];
      var value2 = values[1];

      assert(map.size() === 0);

      map.put(key1, value1);
      assert(map.size() === 1);

      map.put(key2, value2);
      assert(map.size() === 2);
    });

    test('clear()', function () {
      var key = keys[0];
      var value = values[0];

      assert(map.size() === 0);

      map.put(key, value);
      assert(map.size() === 1);

      map.clear();
      assert(map.size() === 0);
    });

    test('empty()', function () {
      var key = keys[0];
      var value = values[0];

      assert(map.empty());

      map.put(key, value);
      assert(!map.empty());
    });

    test('iterator()', function () {
      var tuple1 = [keys[0], values[0]];
      var tuple2 = [keys[1], values[1]];
      var entry;

      map.put(tuple1[0], tuple1[1]);
      map.put(tuple2[0], tuple2[1]);

      var it = map.iterator();

      assert(it.hasNext());
      entry = it.next();
      assert(keys.indexOf(entry[0]) !== -1);
      assert(values.indexOf(entry[1]) !== -1);

      assert(it.hasNext());
      entry = it.next();
      assert(keys.indexOf(entry[0]) !== -1);
      assert(values.indexOf(entry[1]) !== -1);

      assert(!it.hasNext());
    });
  });
});
