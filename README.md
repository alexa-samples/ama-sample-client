# Alexa Mobile Accessory (AMA) Sample Client

This is a minimal implementation of an AMA sample client to help you get started with your own development. It is primarily written in Javascript for ease of understanding. A small part that deals with audio encoding is written in C++.

The client uses Bluetooth Low Energy (BLE) protocol to communicate with Alexa app on your mobile phone,

The client does not implement A2DP return path. If you ask a question to Alexa, the answer would be heard on your paired phone.

The client has been tested on Ubuntu 18.04 as well as on Raspbian OS (Raspberry Pi).

The app is dependent on several third-party libraries, environments, and/or other software packages that are not included in this download package but that you must download ("External Dependencies") from third-party sources.  There are terms and conditions that you need to agree to abide by if you choose to install the External Dependencies.  If you do not agree with every term and condition associated with the External Dependencies, please do not use the app.  Please look at Appendix A for a list of External Dependencies.  

## Step 1: Install the required software packages

```sh
$ sudo apt install bluetooth bluez 
$ sudo apt install git g++  npm  protobuf-compiler
$ sudo npm install npm@latest -g
$ sudo npm install -g node-gyp
$ sudo apt-get install -y libusb-dev libdbus-1-dev libglib2.0-dev libudev-dev libical-dev libreadline-dev libasound2-dev libopus-dev libbluetooth-dev
```

## Step 2: Download AMA client code and dependencies

Download the code into a folder, say, DevAMA.

```sh
$ cd DevAMA
$ npm install
$ node-gyp rebuild
```

## Step 3: Download AMA Kit from Amazon Developer Portal

AMA Kit can be found at
https://developer.amazon.com/alexa/console/avs/preview/resources/details/AMA%20Kit

Download and unzip AMA_ProtoFiles.zip.

```sh
$ cd ~/Downloads
$ unzip AMA_Protofiles.zip
$ mv AMA_Protofiles/* ~/DevAMA/AMA_Protofiles/
```

## Step 4: Generate JavaScript-based serialization code

```sh
$ cd ~/DevAMA/AMA_ProtoFiles
$ protoc --proto_path=. --js_out=import_style=commonjs,binary:. *.proto
$ mv *js ../amaprotos/
```

## Step 5: Change global settings for the client app

Your AMA device is identified by three pieces of information.

1. The name of your device
2. Product ID of the device
3. A serial number for the device

Create a new product at Amazon developer portal and note down the product ID.

Edit main.js and update the following lines:

```javascript
global.name = 'AMA Sample Client'; // Name of your device
global.deviceID = "xxxx"; // Device ID from devportal
global.deviceSerial = "yyyy"; // Serial num of this device
```



## Step 6:  Run the client app

Either stop or disable Bluetooth daemon. This is required by the BLE package being used, Bleno.
```sh
$ sudo systemctl stop bluetooth
```

Make sure that your Bluetooth connector is enabled:
```sh
$ sudo hciconfig hci0 up
```

Make sure that the Bluetooth connector can detect BLE devices in the viscinity:
```sh
$ sudo hcitool lescan
```

Run your app:
```sh
$ cd ~/DevAMA
$ sudo node main
```

## Step 7: Add your new device from Alexa app

Open the Alexa app on your phone and add a new device of "Headphones" type. The app will scan nearby devices and will display the name of your device. Select it. 

The device is now connected. You can see a few messages appearing in the client app.

## Step 8: Interact with the device
Anytime you wish to speak through the microphone, press 's' followed by ENTER.

Note that the speech returnted from Alexa will be played on your phone. Our client does not implement Bluetooth A2DP profile that is required to play audio in the client.

To quit the client app, press 'q' and enter keys.


# Appendix A - Dependencies

Provided below is a list of the External Dependencies and the applicable terms and conditions as indicated by the documentation associated with the External Dependencies as of Amazon's most recent review of such documentation.

THIS INFORMATION IS PROVIDED FOR CONVENIENCE ONLY. AMAZON DOES NOT PROMISE THAT THE LIST OR THE APPLICABLE TERMS AND CONDITIONS ARE COMPLETE, ACCURATE, OR UP-TO-DATE, AND AMAZON WILL HAVE NO LIABILITY FOR ANY INACCURACIES. YOU SHOULD CONSULT THE DOWNLOAD SITES FOR THE EXTERNAL DEPENDENCIES FOR THE MOST COMPLETE AND UP-TO-DATE LICENSING INFORMATION.

YOUR USE OF THE EXTERNAL DEPENDENCIES IS AT YOUR SOLE RISK. IN NO EVENT WILL AMAZON BE LIABLE FOR ANY DAMAGES, INCLUDING WITHOUT LIMITATION ANY DIRECT, INDIRECT, CONSEQUENTIAL, SPECIAL, INCIDENTAL, OR PUNITIVE DAMAGES (INCLUDING FOR ANY LOSS OF GOODWILL, BUSINESS INTERRUPTION, LOST PROFITS OR DATA, OR COMPUTER FAILURE OR MALFUNCTION) ARISING FROM OR RELATING TO THE EXTERNAL DEPENDENCIES, HOWEVER CAUSED AND REGARDLESS OF THE THEORY OF LIABILITY, EVEN IF AMAZON HAS BEEN ADVISED OF THE POSSIBILITY OF SUCH DAMAGES. THESE LIMITATIONS AND DISCLAIMERS APPLY EXCEPT TO THE EXTENT PROHIBITED BY APPLICABLE LAW.


### BlueZ
BlueZ is official Linux Bluetooth protocol stack. It is an Open Source project distributed under GNU General Public License (GPL). http://www.bluez.org/faq/common/

### Node.js
Node.js is an asynchronous event-driven JavaScript runtime. https://raw.githubusercontent.com/nodejs/node/master/LICENSE

### Opus
Opus is a totally open, royalty-free, highly versatile audio codec. http://opus-codec.org/license/

### AMA Kit
Documentation and Protocol Implementation for Alexa Mobile Accessory Kit. https://developer.amazon.com/support/legal/pml
