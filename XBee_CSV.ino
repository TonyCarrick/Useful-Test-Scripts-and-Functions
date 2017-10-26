
//------------------------------------------------------------------------------------------------//
 // Add these libraries if you havent already
 //------------------------------------------------------------------------------------------------//
#include <XBee.h>
#include <SoftwareSerial.h>
#include <SatellitePins.h>

//------------------------------------------------------------------------------------------------//
// All of the glorious xbee shit (i dont know why it just works)
//------------------------------------------------------------------------------------------------//

XBee xbee = XBee();
XBeeResponse response = XBeeResponse();
ZBRxResponse rx = ZBRxResponse();
ModemStatusResponse msr = ModemStatusResponse();
SoftwareSerial xBeeSerial(XBEE_RX, XBEE_TX); // RX, TX

//------------------------------------------------------------------------------------------------//
// Function prototypes
//------------------------------------------------------------------------------------------------//
void convertCSVtoInt(int *arr, int arrSize, String csvStr);
String getXbeeData();

//------------------------------------------------------------------------------------------------//
// Setting up serial ports
//------------------------------------------------------------------------------------------------//
void setup() {
  Serial.begin(9600);
  xBeeSerial.begin(9600);
  xbee.begin(xBeeSerial);
}

//------------------------------------------------------------------------------------------------//
// Demo of the functions in action
//------------------------------------------------------------------------------------------------//
void loop() {
    // Continuously read from the xbee
    String myString = getXbeeData();

    // If data from the xbee is longer than 'fuck all' then print it
    if (myString.length() > 0){
      Serial.println(myString);

      // Note, the size of the array must be the same as the second argument to the function 
      int myVals[4]; 
      // This function fills your array with the values from the string.
      convertCSVtoInt(myVals, 4, myString);

      // To test this we print each value
      for (int i = 0; i < 4; i++){
        Serial.println(myVals[i]);
      }
      
    }

}

//------------------------------------------------------------------------------------------------//
// Functions
//------------------------------------------------------------------------------------------------//
String getXbeeData() {
  // The xbee function does what it does and returns the string from the API packet
    String outputString = "";
    xbee.readPacket();
    
    if (xbee.getResponse().isAvailable()) {               // Data's ready!
      if (xbee.getResponse().getApiId() == ZB_RX_RESPONSE) {
        xbee.getResponse().getZBRxResponse(rx);           // I think this gets the data?
        
        // If the API packet details match, pile all of the data into a string
        int i;
        for (i = 0; i < rx.getDataLength(); i++){
          outputString += char(rx.getData(i));
        }    
      } else if (xbee.getResponse().getApiId() == MODEM_STATUS_RESPONSE) {
        xbee.getResponse().getModemStatusResponse(msr);
      }
    }
    return outputString;
}

void convertCSVtoInt(int *arr, int arrSize, String csvStr){
  // Takes a pointer to an array, the size of the array and a string containing the data
      String subStr = csvStr;

      // Pretty tricky right?
      arr[0] = subStr.toInt();
      for (int i = 1; i < arrSize; i++){
        subStr = subStr.substring(subStr.indexOf(',')+1);
        arr[i] = subStr.toInt();
      }
}

