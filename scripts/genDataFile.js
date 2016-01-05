var CTPParser = require('../build/Release/tp_node_addon').CTPParser;
var fs = require('fs');

var data = fs.readFileSync('../data/disconnect.json');

var addon = new CTPParser();
var mapObjects = new Map();
var values = new Map();
var previousKey = undefined;
var previousValue = undefined;
JSON.parse(String(data), function(k, v) {
  if (k.indexOf('http://') == 0 || k.indexOf('https://') == 0) {
    var existingValues = mapObjects.get(k);
    if (undefined == existingValues) {
      existingValues = values;
    }
    else {
      for (var key of values.keys()) {
        existingValues.set(key);
      }
    }
    mapObjects.set(k, existingValues);
    values = new Map();
    previousKey = undefined;
    previousValue = undefined;
  } else if (!isNaN(k)) {
    if (undefined != previousKey && k <= previousKey) {
      values.delete(previousValue);
    }
    values.set(v);
    previousKey = k;
    previousValue = v;
  } else {
    values = new Map();
    previousKey = undefined;
    previousValue = undefined;
  }
});

for (var [key, value] of mapObjects) {
  var keyValues = undefined;
  for (var key1 of value.keys()) {
    if (undefined != keyValues) {
      keyValues += ',';
      keyValues += key1;
    } else {
      keyValues = key1;
    }
    addon.addTracker(key1);
  }
  addon.addFirstPartyHosts(key, keyValues);
  console.log(key + " = " + keyValues);
}

var serializedObject = addon.serialize();
//console.log('serializedObject == ' + serializedObject);
//console.log('size == ' + serializedObject.length);

fs.writeFileSync('../data/TrackingProtection.dat', serializedObject);
