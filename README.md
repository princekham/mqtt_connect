
Things to do
To make documents about the followings
1. Setting UP MQTT server.
2. Code for connecting to MQTT server
3. Codes for controlling a relay and read sensor information
4.Connecting to database.
5. Reporting with graphics.

Software required
1. Putty for logging in to the raspberry
2. WinSCP
3. raspberry Pi OS lite and flasher (balenaEtcher)

Setting fix IP address 
- sudo nano /etc/dhcpcd.conf
- comment out 
- interface eth0
- static ip_address-192.168.1.165/24
- static routers-192.168.1.1
- static domain_name_servers-192.168.1.1
- comments - setting up for fixed wireless address worked

Docker compose setup
Link: https://drive.google.com/file/d/1kYLdkN4jMBkiz3PL3uqxcd4nyZysgWJp/view?usp=sharing
Steps
- sudo apt update
- curl -fsSL https://get.docker.com -o get-docker.sh
- sudo sh get-docker.sh
- sudo usermod -aG docker $USER
- sudo apt install -y docker-compose
- sudo reboot

Instaling Portainer in Docker (docker cheat sheet at https://dockerlabs.collabnix.com/docker/cheatsheet/)
- has to run docker compose file from (https://drive.google.com/file/d/1ARX4v6H1X91B0etEmMHgvrnGBc8BcQFA/view?usp=sharing)
- copy the docker compose file to the raspberry pi (with WinSCP) and run (with Putty)
- Connecting with WinSCP
- ip address - 192.168.1.165
- Protocal - SCP
- username - pi ; password - raspberry
- Connecting with Putty
- can login to raspberry pi with the same username and password
- docker-compose up
- Portainer runs on port 9000
- username : admin ; password - 1q******
- Next install Mosquito MQTT broker
- copy the file to raspberry pi with WinSCP
- run docker compose up
- tools should explore : MQTT Explorer, MQTT Clinet, MYMQTT (phone apks), 
- (Comments: Raspberry pi wifi got disconnected, has to reconnect wifi and it worked)
- Setting up NodeRED
- copy the docker compose files and run docker compose up as before
- Notes: Time zones needs to be set up when setting up NodeRED; otherwise it gives wrong system time
- The docker compose file is as follow
- 
version: '2'

services:

  nodered:
      container_name: nodered
      build: ./setup/nodered/.
      restart: unless-stopped
      user: "0"
      privileged: true
      env_file: ./setup/nodered/nodered.env
      ports:
        - 1880:1880
      volumes:
        - ./volumes/nodered/data:/data
      environment:
        - TZ=Asia/Yangon
        
Tasks to do
- compile arduino codes for esp8266 12F for MQTT 
- one for relay driving (Finished - compiled codes at One_Relay)
- and one for sensor data reading

Installing InfluxDB
- Notes : influxdb:1.8 only supported; so influxdb:latest needs to be changed to influxdb:1.8 in docker-compose file

Creating database at InfluxDB
-Connect with InfluxDB client (bat file)
-show databases
- create database sensor_data
- use sensor_data
- show measurements
- select * from temp_logs

I have problems with ADC on esp8266 12F and I am trying to flash new firmware but cannot find the firmware.
I found that custom firmware can be build at https://nodemcu.readthedocs.io/en/release/build/ and https://nodemcu-build.com/
I think I can view from this video on how to flach the firmware - https://www.youtube.com/watch?v=MHrm7axsImI
I can use the following command to check the ID of esp8266
C:\Users\saoky\AppData\Local\Programs\Python\Python39\Scripts>esptool.py --port COM8 --baud 115200 flash_id
stepsin flashing
C:\Users\saoky\AppData\Local\Programs\Python\Python39\Scripts>esptool.py --port COM8 --baud 115200 erase_flash
C:\Users\saoky\AppData\Local\Programs\Python\Python39\Scripts>esptool.py --port COM8 --baud 115200 write_flash -fm dio -fs 32m 0x00000 blank4mb.bin


but to use this command I have to disconnect other tool like Esploer

More to look on InfluxDB - Time Series, DB Client,

Now we have to stream sensor data tothe influxDB - successful!

Grafana
port: 3000
enter own password
admin admin
change password to 1q******

How to install Zerotier
https://youtu.be/zADj0k0waFY

Node-RED function for turning timer on/off 

var val = parseFloat(msg.payload)
if(msg.payload>50)
msg.payload="stop"

return msg;

Node-RED function for InfluxDB input

var val = parseFloat(msg.payload)
msg.payload = {sensor:"moisture_sensor",moisture_val:val}
return msg;


changing passwd of Pi :4......Y

Use the following command to get into Docker Grafana container 
docker ps
pi@raspberrypi:~ $ docker exec -it 9a8f395114da /bin/bash

