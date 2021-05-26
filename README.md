
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

Instaling Portainer in Docker
- has to run docker compose file from (https://drive.google.com/file/d/1ARX4v6H1X91B0etEmMHgvrnGBc8BcQFA/view?usp=sharing)
- copy the docker compose file to the raspberry pi and run
- docker-compose up
- Portainer runs on port 9000
- 
