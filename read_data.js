var addon = require('./build/Release/tp_node_addon');

addon.addTracker('facebook.com');
console.log(addon.matchesTracker('facebook.com'));
console.log(addon.matchesTracker('facebook1.com'));
addon.addFirstPartyHosts('facebook.com', 'facebook.fr,facebook.de');
var thirdPartyHosts = addon.findFirstPartyHosts('facebook.com');
console.log(thirdPartyHosts);
if (undefined != thirdPartyHosts) {
  // Free the memory
}
thirdPartyHosts = addon.findFirstPartyHosts('facebook1.com');
console.log(addon.findFirstPartyHosts('facebook1.com'));
if (undefined != thirdPartyHosts) {
  // Free the memory
}
