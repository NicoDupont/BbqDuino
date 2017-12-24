## Arduino BbqDuino

Author : Nicolas Dupont  
Last Update : 24/12/2017  
Current state : **Prototype finished**  

The goal is to read, store and display the temperature of a barbecue/grill to follow the cooking.  
I hope he would be a very good friend for cooking sessions on my barbecue.  

**To do List for final v1 ! :**  

level shifter for the lcd
1. Power supply the arduino from battery
2. Electric card for integration
3. Fritzing
4. Make integration into a box

**Features :**

- A WebApp to manage and monitor the cooking session.(the web app is designed to have just the necessary things)
- Monitor on the lcd the temperatures and the time (total time and "partial" time. they can be reset with buttons).
- A led indicate us if the food temperature is reached or if the barbecue temperature is not good (too low or too high).
- The 3 temperatures are stored in a database with a record every minutes.

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
	     | Docs    ( Schematic / Diagram / Useful docs for the project )  
	     readme.md


**Code :**

Arduino : [sketch](https://github.com/NicoDupont/BbqDuino/blob/master/Arduino/bbqduino)  
WebApp : [Scripts](https://github.com/NicoDupont/BbqDuino/blob/master/App)  

To save data and to use the web app, you need a database and a php/web server.    
For the project I used :  

 - Debian 9 with Mariadb/Mysql, php7 and apache2 in a virtualMachine on my local network.
 - Php/Jquery to manage all the data (backup, recovery and edit).
 - [Bootstrap](https://getbootstrap.com/) coupled with [ChartJs](http://www.chartjs.org/) to make the graph and the responsive design.

=> You need to adapt to your setting  

**Functional Diagram :**

![diagram bbqduino](https://github.com/NicoDupont/BbqDuino/blob/master/Docs/diagram bbqduino.png)

**Parts used :**

 - NodeMcu esp12 
 - 2x maverick sensors ET-73 ([smoker](http://www.maverickhousewares.com/parts/et-73-smoker-probe),[food](http://www.maverickhousewares.com/parts/et-7273-high-heat-6ft-food-probe)) + 2x 22k omhs resistors
 - 1x LM35 analog sensor for the ambient temperature
 - 1x ADS1015 ADC I2C to connect the 2 maverick sensors because the nodemcu have only 1 analog input.
 - 1x 5mm leds + 1x 100 ohms resistors
 - 2x buttons + 2x 10k ohms resistors
 - 1x Lcd 20x4
 - 1x level shifter 3.3v/5v for the lcd witch is powered with 5v and not 3.3v
 - 1x 6xAA Battery Holder + 6x AA Eneloop 1.2-1.5v
 - 1x 3.3v regulator (XL6019)
 - 4x jack 2.5mm mono connector (if I add a 2 more sensors)
 - 1x led 5mm support
 - 1x plastic box
 - 1x switch 2 positions to control the power suppley
 - 1x switch 3 positions to control the power supply of the nodemcu (usb or battery)

![parts1 bbqduino](https://github.com/NicoDupont/BbqDuino/blob/master/Img/breadboard_prototype.JPG)

**Web App :**

The web app is responsive :  

![web app iphone6 bbqduino](https://github.com/NicoDupont/BbqDuino/blob/master/Img/bbqduino_ipad.PNG)
![web app ipad bbqduino](https://github.com/NicoDupont/BbqDuino/blob/master/Img/bbqduino_iphone6.PNG)

**Schematic :**

![fritzing bbqduino](https://github.com/NicoDupont/BbqDuino/blob/master/Img/fritzing.png)
![elec bbqduino](https://github.com/NicoDupont/BbqDuino/blob/master/Img/elec_fritzing.png)
![board bbqduino](https://github.com/NicoDupont/BbqDuino/blob/master/Img/elec_board.png)

**Possible improvements :**

- Manage a fan for a charcoal grill
- Use a better battery
- Improve web app
- 4 probs
- Better screen 
