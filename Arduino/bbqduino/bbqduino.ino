/*
* -----------------------------------------------------------------------------------
* Last update :   24/12/2017
* Project BbqDuino v0
* Author(s) : Nicolas Dupont (https://github.com/NicoDupont)
* Contributor(s) :
* Arduino : Nodemcu esp12 + lcd 20x4 I2C + 2x buttons+ 2x maverick prob ET-73 + 1x lm35 + 1x ads1015
* Arduino 1.8.4 (Ubuntu 16.04.3)
* -----------------------------------------------------------------------------------
*/

//Library for I2C
#include <Wire.h> 

//Library for ads1015
#include <Adafruit_ADS1015.h>
Adafruit_ADS1015 ads1015;  // Ads1015 @ (0x48)

// Library for wifi
#include <ESP8266WiFi.h>
const char* ssid     = "YourSSID";			// Your SSID
const char* password = "YourPASSWORD";	// Your Password
const char* host = "Web/Databse Server Ip";				// Web/Databse Server Ip
const int httpPort = 80;						// Web/Databse Server port
WiFiClient client;								// Initialize the wifi client mode

// Library for LCD
#include <LiquidCrystal_I2C.h>
// Setup LCD I2C if the first one don't work try the second one
LiquidCrystal_I2C lcd(0x3F,20,4);		//LiquidCrystal_I2C lcd(0x27,20,4); 

// Setup the led 
const int L1 = 16;  						// pin D0 digital on the nodemcu
// Setup the buttons
const int BU1 = 0; 						// pin D3 digital
const int BU2 = 2;						// pin D4 digital
// Setup the buzzer
// const char BUZ = 15;						// pin D8 digital
// Setup variables
float voltage = 3.3;    				// 3.3 for the nodemcu esp12
long time0, time1, time2, time3, duration, total;	// manage the time in the main loop
int hour,minute,thour,tminute;			// display the cooking time on the lcd
boolean lcdtimeinfo, buzzer;			// toggle the display and the buzzer
const long tbrd = 15000; 				// time in ms between each update for session informations
const long tled = 4000;  				// time in ms between led and lcd control 
const long tber = 3000;  				// time in ms between each reading
const long tbgr = 60000; 				// 3600000=(1hour) time in ms between each geting request
const int idambient = 1; 				// id for the ambient prob
const int idbbq = 2;     				// id for the barbecue prob
const int idfood = 3;	 				// id for the food prob
const int idbbq2 = 4;             // id for the barbecue prob 2
const int idfood2 = 5;         // id for the food prob 2
int bbqtarget = 0;  	 				// temperature for the target internal barbecue temperature
const int bbqdelta = 7;  				// delta for led and buzzer control
int foodtarget = 0; 	 				// temperature for the target fooding temperature
const int fooddelta = 7; 				// delta for led and buzzer control
int tambient = 0;		 				// set ambient temperature at 0
int tbbq = 0; 			 				// set barbecue temperature at 0
int tfood = 0;			 				// set food temperature at 0
int tbbq2 = 0;               // set barbecue temperature at 0
int tfood2 = 0;              // set food temperature at 0
int sessionid = 0;		 				// set 0 for the session id 
int sessionend = 1;						// set 1 for the end state of the current session
// new structure for the http response
struct retrieve { int i, b, f, e; };
struct retrieve response;

/*-------------------------------------------------------
 * ------------------------------------------------------
 *   SETUP  *
 *-------------------------------------------------------
 --------------------------------------------------------*/
void setup(){

	Serial.begin(9600);					// Initialize the serial at 9600bauds

	StartLcd();							// Initialize the lcd
	StartWifi();						// Initialize the wifi connection
	IpSerialWifi();					// print local ip on serial monitor
	Serial.println("Init...");
	delay(1000); 
	Serial.println("Ready !");
	delay(1000); 						// wait a little
	MessageLcd(1, 3, "Starting..");
	MessageLcd(1, 4, "Ip:");
	lcd.setCursor(3,3);
	IpLcdWifi();						// print ip on the lcd
	delay(4000);						// wait a little
	//lcd.clear();						// Clear the lcd

	// initialize time for the main loop
	time0 = millis();
	time1 = millis();
	time2 = millis();
	time3 = millis();
	duration = millis();
	total = millis();
	lcdtimeinfo=0;						// toggle the display
	buzzer=0;							// toggle the buzzer
  
	// initialize pin output for the  led monitoring status
	pinMode(L1, OUTPUT);
	digitalWrite(L1, HIGH);
	//initialize the digital pins to use the 2 buttons
	pinMode(BU1, INPUT);
	pinMode(BU2, INPUT);
  
	UpdateData();						// get from the server : the last session id, the last target bbq temp, the last target food temp and the end state. 
  /*Serial.print("Session n° : ");
  Serial.println(sessionid);
  Serial.print("Session status n° : ");
  Serial.println(sessionend);*/
  
  //Serial.println("Getting single-ended readings from AIN0..3");
  //Serial.println("ADC Range: +/- 6.144V (1 bit = 3mV)");
  ads1015.begin();  // start the ads1015

}

/*-------------------------------------------------------
 * ------------------------------------------------------
 *   MAIN LOOP  *
 *-------------------------------------------------------
 --------------------------------------------------------*/
void loop(){

	/*
	Serial.print("Button 1 : ");
	Serial.println(digitalRead(BU1));
	Serial.print("Button 2 : ");
	Serial.println(digitalRead(BU2));
	*/
	// It will be better to use the button library or interrupt function.. but it work for now (it is very basic actualy)..  
	if ((digitalRead(BU1) == 1) && (digitalRead(BU2) == 0)){
		// reset duration (cooking time)
		duration = millis();
		delay(170);
	}
		else if ((digitalRead(BU1) == 0) && (digitalRead(BU2) == 1)){
		// reset total (total cooking time)
		total = millis();
		delay(170);
	}
  
	//-----------------------------------------------------------------------
	//-----------------------------------------------------------------------
	// Update the session informations from the server
	if((millis() - time3) >= tbrd) {
    if (TestWifi()){
      Serial.println("Restart Wifi connection !");
      StartWifi();
    }
		UpdateData();		// see the UpdateData() function for details
		time3 = millis();	//reboot time
	}
  
	//-----------------------------------------------------------------------
	//-----------------------------------------------------------------------
	// Update the 3 temperatures every << tber >> ms
	if((millis() - time1) >= tber) {
		tambient = (int) (analogRead(0) * (voltage / 1023.0 * 100.0)); // analog 0 on the nodemcu
		tbbq = AverageTemp(1,10); // anaolog 1 on ADS1015
		tfood = AverageTemp(0,10);// anaolog 0 on ADS1015
    //tbbq2 = AverageTemp(3,30); // anaolog 3 on ADS1015
    //tfood2 = AverageTemp(2,30);// anaolog 2 on ADS1015
		time1 = millis(); //reboot time
	}

	//---------------------------------------------------
	//---------------------------------------------------
	// Control when data needs to be send to the database
	if((millis() - time2) >= tbgr) { 
		// send data only if the session id is not equal to 0 and the current session is active
		if ((sessionid > 0) && (sessionend == 0)) {
			SaveDataWifi(sessionid,idambient,tambient);
			SaveDataWifi(sessionid,idbbq,tbbq);
			SaveDataWifi(sessionid,idfood,tfood);
      //SaveDataWifi(sessionid,idbbq2,tbbq2);
      //SaveDataWifi(sessionid,idfood2,tfood2);
		}
		time2 = millis(); //reset time
	}

  //-----------------------------------------------------------------------
  //-----------------------------------------------------------------------
  // Control the 3 leds, the buzzer and the lcd 
  if((millis() - time0) >= tled) {

		//-------------------
		// Send the 3 temperatures to the serial monitor
		Serial.print("Air : ");
		PrintTemp(tambient);
		Serial.print("Bbq : ");
		PrintTemp(tbbq);
		Serial.print("Food : ");
		PrintTemp(tfood);

		//-------------------
		// Control the buzzer if necessary
    //Serial.print("State Alertemp : ");
    //Serial.println(AlertTemp(tbbq,tfood,tbbq2,tfood2));
		/*if (AlertTemp(tbbq,tfood,tbbq2,tfood2)){
			//Serial.print("buzzer : ");
			//Serial.println(buzzer);
			if(buzzer == 1){     
				tone(BUZ,500);
        delay(300);
        noTone(BUZ);
				delay(500);
				tone(BUZ,500);
        delay(300);
        noTone(BUZ);
			}
		}*/

		//-------------------
		// Update lcd and toggle CookingTime/totaltime
		lcd.clear();
		
		if (lcdtimeinfo == 0){
			//-------------------
			// update the cooking time
			hour=DurationHour(duration);
			minute=DurationMinute(duration);
			
			MessageLcd(1,1,"Cooking time : ");
			lcd.setCursor(15,0);
			if (hour < 10) {
				lcd.print("0");
				lcd.print(hour);
			}else{
				lcd.print(hour);
			}
			lcd.print(":");
			if (minute < 10){
				lcd.print("0");
				lcd.print(minute);
			}else{
				lcd.print(minute);
			}
		}else{
			//-------------------
			// update the total time
			thour=DurationHour(total);
			tminute=DurationMinute(total);

			MessageLcd(1,1,"Total time : ");
			lcd.setCursor(15,0);
			if (thour < 10) {
			lcd.print("0");
			lcd.print(thour);
			}else{
			lcd.print(thour);
			}
			lcd.print(":");
			if (tminute < 10) {
			lcd.print("0");
			lcd.print(tminute);
			}else{
			lcd.print(tminute);
			}
		}
    
		PrintTempLcd(tambient,"Air  :  ",1,2);
		MessageLcd(14,2,"Target:");
		PrintTempLcd(tbbq,"Bbq  : ",1,3);
		PrintTempLcd(tfood,"Food :  ",1,4);
		lcd.setCursor(15,2);
		lcd.print(bbqtarget);
		lcd.print(" C");
		lcd.setCursor(16,3);
		lcd.print(foodtarget);
		lcd.print(" C");

		//----------------------
		// Toggle the state of the led 
		if (AlertTemp(tbbq,tfood,tbbq2,tfood2)){   
			if (digitalRead(L1)){
				digitalWrite(L1, LOW);
			}
		}else{ 
			if (!digitalRead(L1)) {
				digitalWrite(L1, HIGH);
			}
		}
		time0 = millis();			//reset time
		lcdtimeinfo = !lcdtimeinfo;	// toggle 
		buzzer = !buzzer;			// toggle 
   
	}

} //end main loop


/*-------------------------------------------------------
 * ------------------------------------------------------
 *   FUNCTIONS   *
 *-------------------------------------------------------
 --------------------------------------------------------*/

void PrintTemp(float temp){
	Serial.print(temp);
	Serial.println(" : Degrés Celcius");
}

void PrintTempLcd(int temp, char text[], int posi, int line){
	lcd.setCursor(posi-1,line-1);
	lcd.print(text);
	lcd.print(temp);
	lcd.print(" C");  
}

void StartWifi(){
	//Serial.print("Connecting to ");
	//Serial.println(ssid);
	WiFi.begin(ssid, password); 
  int i =0;
	while (WiFi.status() != WL_CONNECTED){
		delay(500);
		Serial.print(".");
    i++;
    if (i >= 20){
      break;
      }
	}
	Serial.println("WiFi connected");  
	//Serial.println("IP address: ");
	//Serial.println(WiFi.localIP());	
}

// not tested
bool TestWifi(){
	if (WiFi.status() != WL_CONNECTED){
		return 0;
	}else{
		return 1;
	}
}

void IpSerialWifi(){
	Serial.print("My IP address: ");
	Serial.print(WiFi.localIP());
	Serial.print(".");
	Serial.println();
}

void SaveDataWifi(int s, int id, int temp){
	//Serial.print("connecting to ");
	//Serial.println(host);
	if (client.connect(host, httpPort)){
		// We now create a URI for the request
		String url = "/BBQDUINO/savedata.php";
		url += "?session=";
		url += (int) s;
		url += "&id=";
		url += (int) id;
		url += "&temp=";
		url += (int) temp;
		//Serial.println(url);
		client.print(String("GET ") + url + " HTTP/1.1\r\n" +
					 "Host: " + host + "\r\n" + 
					 "Connection: close\r\n\r\n");
		//delay(100);
		client.stop();
	}else{
		//StartWifi();
	}
}

  
void StartLcd(){  
	lcd.init();          
	lcd.backlight();
	lcd.setCursor(0,0);
	lcd.print("----- BBQDUINO -----");
	lcd.setCursor(0,1);
	lcd.print("v0 ------ 24/12/2017");
}

void MessageLcd(int posi, int line,char message[]){
	lcd.setCursor(posi-1,line-1);
	lcd.print(message); 
}


void IpLcdWifi() {
	lcd.print(WiFi.localIP()); 
	lcd.print(".");	
}

int DurationHour(long dur){
	long h;
	h= (millis() - dur) / 3600000;
	return (int) (h);
}

int DurationMinute(long dur){
	long m, h;
	h = DurationHour(dur);
	m= (millis() - dur - (h*3600000)) / 60000;
	return (int) (m);
}

int maverick_temp(int aval){
  double R, T;
  // Function from : http://smokingmeatforums.com/index.php?threads/the-how-to-for-making-an-arduino-controlled-smoker.90085/
  // These were calculated from the thermister data sheet
  //  A = 2.3067434E-4;
  //  B = 2.3696596E-4;
  //  C = 1.2636414E-7;
  // This is the value of the other half of the voltage divider
  R = log((1 / ((1023 / (double) aval) - 1)) * (double) 22500);
  // Compute degrees C
  T = (1 / ((2.3067434E-4) + (2.3696596E-4) * R + (1.2636414E-7) * R * R * R)) - 273.25;
  return (T);
}

int AverageTemp(int analogpin,int nb){
  int SumTemp = 0;
  int i = 0;
  while(i < nb){
    SumTemp += (int) (maverick_temp((ads1015.readADC_SingleEnded(analogpin) * 0.003 / voltage) * 1023));
    delay(10);
    //Serial.println(i);
    i++;
  }
  return (int) (SumTemp / nb);
}

bool AlertTemp(int tempbbq,int tempfood,int tempbbq2,int tempfood2){	
	if ((tempbbq == 0) || (tempfood ==0) || (bbqtarget == 0)  || (foodtarget == 0) /*|| (tempbbq2 == 0) || (tempfood2 == 0)*/){
		return 0;
	}else if ((tempbbq < (bbqtarget - bbqdelta)) || (tempbbq > (bbqtarget + bbqdelta)) || (tempfood > (foodtarget - fooddelta)) /*|| (tempbbq2 < (bbqtarget - bbqdelta)) || (tempbbq2 > (bbqtarget + bbqdelta)) || (tempfood2 > (foodtarget - fooddelta))*/){
		return 1;
	}else{
		return 0;
	}	
}


void UpdateData(){
	response = RetrieveData();
	while ((response.i == 0) || (response.i > 1000) || (response.b == 0) || (response.b > 1000) || (response.f == 0) || (response.f > 1000) || (response.e > 1)){ 
		Serial.println('Problem');
		response = RetrieveData();
	}
	sessionid = response.i;
	bbqtarget = response.b;
	foodtarget = response.f;
	sessionend = response.e; 
}

retrieve RetrieveData(){
	int d[] = {0, 0, 0, 0};
	if (client.connect(host, httpPort)){
		String url = "/BBQDUINO/retrievedata.php";
		client.print(String("GET ") + url + " HTTP/1.0\r\n" +
					"Host: " + host + "\r\n" + 
					"Connection: close\r\n\r\n");
		while (client.available() == 0){
			//wait client.available() == 0
		} 
		String text = "";    
		while(client.available()){
			char r = client.read();
			//Serial.println(r);
			text += r;
		}
		client.stop();
		//Serial.println(text);
		char chartext[text.length() + 1];
		text.toCharArray(chartext, text.length());
		char dlm[] = "|";
		char* val;
		val = strtok(chartext, dlm);
		for(int i = 0; i < 5; i++){
			d[i] = atoi(val);
			//Serial.println(d[i]);
			val = strtok(NULL, dlm);
		}
	}
	else{
		//StartWifi();
	}
	return { d[1], d[2], d[3], d[4] }; 
}

