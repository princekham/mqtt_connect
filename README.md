
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


