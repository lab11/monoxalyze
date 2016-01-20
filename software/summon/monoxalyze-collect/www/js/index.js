/* JavaScript for Template Summon UI */

var deviceId = "C0:98:E5:30:03:14";                                                 // while testing, replace with address of a BLE peripheral
var deviceName = "BLE Device";                                                      // while testing, replace with desired name
var serviceId = "1800"; 
var startCollectId = "2A00"; 
var stopCollectId = "2A00"; 
var startReadId = "2A00"; 
var doneReadId = "2A00"; 

var app = {
    // Application Constructor
    initialize: function() {
        document.addEventListener("deviceready", app.onAppReady, false);
        document.addEventListener("resume", app.onAppReady, false);
        document.addEventListener("pause", app.onPause, false);
    },
    // App Ready Event Handler
    onAppReady: function() {
        if (window.gateway) { // if UI opened through Summon,
            deviceId = window.gateway.getDeviceId();// get device ID from Summon
            deviceName = window.gateway.getDeviceName();//e name from Summon
        }
        ble.isEnabled(app.onEnable);  // if BLE enabled, goto: onEnable
    },

    // App Paused Event Handler
    onPause: function() {    // if user leaves app, stop BLE
        ble.disconnect(deviceId);
        ble.stopScan();
    },

    // Bluetooth Enabled Callback
    onEnable: function() {
        app.onPause();         // halt any previously running BLE processes
        ble.startScan([], app.onDiscover, app.onAppReady);     
        app.log("Searching for " + deviceName + " (" + deviceId + ").");
    },
    // BLE Device Discovered Callback
    onDiscover: function(device) {
        if (device.id == deviceId) {
            app.log("Found " + deviceName + " (" + deviceId + ")! Connecting.");
            ble.connect(deviceId, app.onConnect, app.onAppReady);
        }
    },

    // BLE Device Connected Callback
    onConnect: function(device) {
        app.log("Connected to " + deviceName + " (" + deviceId + ")!");
		app.log(JSON.stringify(device));
		//change the dome name to say connected and ready to collect data
		document.getElementById("collect_button").innerHTML = "Start Collecting";
		document.getElementById("collect_button").disabled = false;

		//serviceId = device.services[2];
		
        //ble.read(deviceId, serviceUuid, characteristicUuid, app.onRead, app.onError);  
        //ble.write(deviceId, serviceUuid, characteristicUuid, app.stringToBytes(writeValue), app.onWrite, app.onError); 
    },
    onRead: function(data) {
        app.log("Characteristic Read: " + app.bytesToString(data));      
    },
    // BLE Characteristic Write Callback
    onWrite : function() {
        app.log("Characeristic Written: " + writeValue);      
    },

    onError: function() {              
        app.log("Read/Write Error.")
        ble.isEnabled(deviceId,function(){},app.onAppReady);
        ble.isConnected(deviceId,function(){},app.onAppReady);
    },
    // Function to Convert String to Bytes (to Write Characteristics)
    stringToBytes: function(string) {
        array = new Uint8Array(string.length);
        for (i = 0, l = string.length; i < l; i++) array[i] = string.charCodeAt(i);
        return array.buffer;
    },
    // Function to Convert Bytes to String (to Read Characteristics)
    bytesToString: function(buffer) {
        return String.fromCharCode.apply(null, new Uint8Array(buffer));
    },
	collect_button: function() {
		app.log("collect button pressed");
		ble.write(deviceId, service
	}
    // Function to Log Text to Screen
    log: function(string) {
        document.querySelector("#console").innerHTML += (new Date()).toLocaleTimeString() + " : " + string + "<br />"; 
    }
};

app.initialize();
