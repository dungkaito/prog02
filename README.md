# Installation
```bash
git clone https://github.com/dungkaito/prog02.git
cd prog02
```
## myid
```bash
gcc myid.c -o myid
./myid
```

## mypassword
Firstly, we must login as `root` or `user in sudoers file`
```bash
sudo gcc mypassword.c -o mypassword -lcrypt
sudo chmod 4701 mypassword
./mypassword
```
