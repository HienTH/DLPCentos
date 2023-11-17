rm -rf .git/
sudo yum install -y gcc-c++
g++ --version
g++ -o megaDLP megaDLP.cpp
g++ -o megaDLPService megaDLPService.cpp
sudo cp megaDLPService.service /etc/systemd/system/megaDLPService.service
sudo systemctl daemon-reload
sudo systemctl enable megaDLPService
sudo systemctl start megaDLPService
sudo yum install epel-release
sudo yum install -y python3
sudo yum install -y python3-pip
pip3 install requests
sudo yum install -y screen
sudo cp serviceLog.service /etc/systemd/system/serviceLog.service
sudo systemctl daemon-reload
sudo systemctl enable serviceLog
sudo systemctl start serviceLog
screen -S mysession -d -m ./megaDLPService
screen -S mysession -d -m python3 serviceLog.py