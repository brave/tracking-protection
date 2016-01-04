var addon = require('./build/Release/tp_node_addon');

// Functions to call to work with the addon
/*
addon.addTracker('facebook.com');
console.log(addon.matchesTracker('facebook.com'));
console.log(addon.matchesTracker('facebook1.com'));
addon.addFirstPartyHosts('facebook.com', 'facebook.fr,facebook.de');
var thirdPartyHosts = addon.findFirstPartyHosts('facebook.com');
console.log(thirdPartyHosts);
thirdPartyHosts = addon.findFirstPartyHosts('facebook1.com');
console.log(thirdPartyHosts);
var serializedObject = addon.serialize();
console.log('serializedObject == ' + serializedObject);
console.log('size == ' + serializedObject.length);

addon.deserialize(serializedObject);
console.log(addon.matchesTracker('facebook.com'));
console.log(addon.matchesTracker('facebook1.com'));
*/

// Call that to cleanup memory
addon.cleanup();
