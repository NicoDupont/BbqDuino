## Arduino BbqDuino

Author : Nicolas Dupont  
Last Update : 14/01/2019  
Current state : **Prototype finished**  

The goal is to read, store and display the temperature of a barbecue/grill to follow the cooking.  
I hope it will be a very good friend for cooking sessions on my gaz barbecue.  

**To do List for final v1 ! :**  

1. Power supply the arduino from power bank
2. Electric card for integration
3. Fritzing
4. Make integration into a box
5. improve the arduino code and web app code
6. Add ifttt alert

**Features :**

- A WebApp to manage and monitor the cooking session.(the web app is designed to have just the necessary things)
- Monitor on the lcd the temperatures and the time ("partial" time can be reset with a button).
- A led indicate us if the food temperature is reached or if the barbecue temperature is not good (too low or too high).(+ IFTTT Alert)
- The 4 temperatures are stored in a database with a record every minutes.

**Start :**

Before to start the arduino. You need to asure you that the last cooking session is finished.    
Go to the webapp and see if it is already finished. If not push the End button at the bottom of the page.    
A message shows you that the session is now finished.  
Now add a new session by clicking on the weel at the top.  
Select what do you want and click on the button "Add or Edit".  
You go back automaticaly to the previous page and you see the informations about the new session.  
You can start the arduino now.  
The arduino ask to the server the informations about the last cooking session and after is going to send data to the server.  
The web page is reloaded automaticaly every 3 minutes but you can click on the refresh button at the top when you want to obtain the last data.  


**Folder organisation :**

	| BbqDuino
	  => | App ( SqlDatabase and WebApp(php,html,css,js) )
	     | Arduino  ( Arduino sketch and library )
	     | Img  ( Pictures of the bbqduino)
	     | Docs    ( Schematic / Diagram / Fritzing parts / Useful docs for the project )  
	     readme.md


**Code :**

Arduino : [sketch](https://github.com/NicoDupont/BbqDuino/blob/master/Arduino/bbqduino)  
WebApp : [Scripts](https://github.com/NicoDupont/BbqDuino/blob/master/App)  

To save data and to use the web app, you need a database and a php/web server.    
For the project I used :  

 - Debian 9 with Mariadb/Mysql, php7 and apache2 on a raspberry pi on my local network.
 - A web app (html/php/jquery) to visualize and manage all the data (backup, recovery and edit).
 - [Bootstrap](https://getbootstrap.com/) coupled with [ChartJs](http://www.chartjs.org/) to make the graph and the responsive design.

=> You need to adapt to your setting  

**Functional Diagram :**

![diagram bbqduino](https://github.com/NicoDupont/BbqDuino/blob/master/Docs/diagram%20bbqduino.png)

**Parts used :**

 - NodeMcu esp12 
 - 4x maverick sensors ET-73 ([smoker](http://www.maverickhousewares.com/parts/et-73-smoker-probe),[food](http://www.maverickhousewares.com/parts/et-7273-high-heat-6ft-food-probe)) + 4x 22k omhs resistors
 - 1x DS18B20 digital sensor for the ambient temperature
 - 1x ADS1015 ADC I2C to connect the 4 maverick sensors because the nodemcu have only 1 analog input.
 - 1x 5mm leds + 1x 220 ohms resistors
 - 1x buttons + 1x 10k ohms resistors
 - 1x Lcd 16x2
 - 1x level shifter 3.3v/5v for the lcd witch is powered with 5v and not 3.3v [link](https://fr.aliexpress.com/item/Pratique-Simple-4-Canal-IIC-I2C-3-V-5-V-Logic-Level-Converter-Bidirektional-Shifter-Module/32839758085.html?spm=a2g0s.9042311.0.0.ONxQt6)
 - 1x 3.3v regulator
 - 4x jack 2.5mm mono connector
 - 1x led 5mm support
 - 1x plastic box
 - 1x switch 2 positions to control the power supply
 - 1x usb mini pcb

![parts1 bbqduino](https://github.com/NicoDupont/BbqDuino/blob/master/Img/breadboard_prototype.JPG)

**Web App :**

The web app is responsive :  

![web app iphone6 bbqduino](https://github.com/NicoDupont/BbqDuino/blob/master/Img/bbqduino_ipad.PNG)
![web app ipad bbqduino](https://github.com/NicoDupont/BbqDuino/blob/master/Img/bbqduino_iphone6.PNG)

**Schematic :**

![fritzing bbqduino](https://github.com/NicoDupont/BbqDuino/blob/master/Img/bbqduino_bb.png)
![elec bbqduino](https://github.com/NicoDupont/BbqDuino/blob/master/Img/bbqduino_schema.png)
![board bbqduino](https://github.com/NicoDupont/BbqDuino/blob/master/Img/elec_board.png)

**Possible improvements :**

- Oled 128*64
- json data
