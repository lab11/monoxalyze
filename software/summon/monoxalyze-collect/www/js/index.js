/* JavaScript for Template Summon UI */

var deviceId = "C0:98:E5:00:38:6D"; 
//var deviceId = "C0:98:E5:00:6E:2D"; 
var deviceName = "Monoxalyze";  
var serviceId = "1800"; 
var startCollectId = "11AB"; 
var stopCollectId = "11AC"; 
var startReadId = "11AD"; 
var doneReadId = "11AE"; 
var dataId = "11AF"; 
var dataFile;

var stateEnum = {
	WAITING: 0,
	COLLECTING: 1,
	WRITING_BACK: 2
}
var state;

var app = {
	
    // Application Constructor
    initialize: function() {
        document.addEventListener("deviceready", app.onAppReady, false);
        document.addEventListener("resume", app.onResume, false);
        document.addEventListener("pause", app.onPause, false);
    },
	onResume: function() {

	},
    // App Ready Event Handler
    onAppReady: function() {
		

		app.log("app started");
		state = stateEnum.WAITING;
		dataFile = new ArrayBuffer(20);

        if (window.gateway) { // if UI opened through Summon,
            deviceId = window.gateway.getDeviceId();// get device ID from Summon
            deviceName = window.gateway.getDeviceName();//e name from Summon
        }

        ble.isEnabled(app.onEnable,app.notEnabled);  // if BLE enabled, goto: onEnable
    },
	notEnabled: function() {
		app.log("BLE is not Enabled!!");
        ble.enable(app.onEnable,app.notEnabled);  
	},

    // App Paused Event Handler
    onPause: function() {    // if user leaves app, stop BLE
        ble.disconnect(deviceId);
        ble.stopScan();
    },

    // Bluetooth Enabled Callback
    onEnable: function() {
        app.onPause();         // halt any previously running BLE processes
        ble.startScan([], app.onDiscover, function(){app.log("scan error")});     
        app.log("Searching for " + deviceName + " (" + deviceId + ").");
    },
    // BLE Device Discovered Callback
    onDiscover: function(device) {
        if (device.id == deviceId) {
            app.log("Found " + deviceName + " (" + deviceId + ")! Connecting.");


			//connect
            ble.connect(deviceId, app.onConnect, function(){app.log("connect error")});
        }
    },
    onConnect: function(device) {

		//do some logging 
        app.log("Connected to " + deviceName + " (" + deviceId + ")!");
		app.log(JSON.stringify(device));

		//copy some stuff
		serviceId = device.services[2];
		startCollectId = device.characteristics[3].characteristic;
		stopCollectId = device.characteristics[4].characteristic;
		startReadId = device.characteristics[5].characteristic;
		doneReadId = device.characteristics[6].characteristic;
		dataId = device.characteristics[7].characteristic;

		
		app.log(JSON.stringify(startCollectId));
		app.log(JSON.stringify(stopCollectId));
		app.log(JSON.stringify(startReadId));
		app.log(JSON.stringify(doneReadId));
		app.log(JSON.stringify(dataId));


		//register for the notification from the data element to construct the
		//file
		ble.startNotification(deviceId,serviceId,dataId,app.onDataNotify,
		function() {
			app.log("notification start error");
		});
		ble.startNotification(deviceId,serviceId,doneReadId,app.onDoneNotify,
		function(){
			app.log("notification 2 start error");
		});

		//change the dome name to say connected and ready to collect data
		document.getElementById("collect_button").innerHTML = "Start Collecting";
		document.getElementById("collect_button").disabled = false;
    },
    onRead: function(data) {
        app.log("Characteristic Read: " + app.bytesToString(data));      
    },
    // BLE Characteristic Write Callback
    onWrite : function() {
        app.log("Characeristic Written: " + writeValue);      
    },
	onDataNotify: function(buffer) {
		app.log("got data notifation");
		var tmp = new Uint8Array(dataFile.byteLength + buffer.byteLength);
		app.log(dataFile.byteLength);
		tmp.set(new Uint8Array(dataFile), 0);
		tmp.set(new Uint8Array(buffer), dataFile.byteLength);
		dataFile = tmp.buffer;
	},
	onDoneNotify: function(buffer) {
		//do something to send to the cloud and save the file
		app.log("got done notifation");	
		intArray = new Uint32Array(dataFile, 0, dataFile.byteLength/4);
		app.log(intArray.join());
	},
    // BLE Device Connected Callback
    onError: function() {              
        app.log("Read/Write Error.")
        //ble.isEnabled(deviceId,function(){},app.onAppReady);
        //ble.isConnected(deviceId,function(){},app.onAppReady);
    },
	onError2: function() {
		app.log("error on writing collect start");
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
	nowCollecting: function() {
		app.log("successfully started collecting");
		state = stateEnum.COLLECTING;
		document.getElementById("collect_button").innerHTML = "Stop Collecting and Record Data";
	},
	doneCollecting: function() {
		app.log("successfully finished collecting - about to read back");
		var data = new Uint8Array(1);
		data[0] = 1;
		ble.write(deviceId,serviceId,startReadId,data.buffer,app.startReading,
		function() {
			app.log("start read error");
		});
	},
	startReading: function() {
		state = stateEnum.WRITING_BACK;
		document.getElementById("collect_button").innerHTML = "Recording Data...";
		document.getElementById("collect_button").disabled = true;
	},
	collect_button: function() {
		app.log("collect button pressed");
		if(state == stateEnum.WAITING) {
			var data = new Uint8Array(1);
			data[0] = 1;
			app.log("now writing");
			ble.writeWithoutResponse(deviceId,serviceId,startCollectId,data.buffer,app.nowCollecting,app.onError2);
		} else if (state == stateEnum.COLLECTING) {
			var data = new Uint8Array(1);
			data[0] = 1;
			ble.write(deviceId,serviceId,stopCollectId,data.buffer,app.doneCollecting,function() {app.log("error in stopping collection")});
		}
	},
    // Function to Log Text to Screen
    log: function(string) {
        //document.querySelector("#console").innerHTML += (new Date()).toLocaleTimeString() + " : " + string + "<br />"; 
		console.log(string);
    }
};

app.initialize();
