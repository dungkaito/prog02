#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <shadow.h>
#include <unistd.h>

int main() {
		/*-----AUTHENTICATE CURRENT PASSWORD-----*/
	// get the current logged in username
	char *username = getenv("USER");
	printf("Doi mat khau cho user %s.\n", username);

	// get a record (line) from /etc/shadow by username
	struct spwd *spwd = getspnam(username);

	// prompt the user for a password without echoing
	char *password = getpass("Nhap mat khau cu: ");

	// hash with same salt (in spwd->sp_pwdp) to authenticate 
	char *encrypted = crypt(password, spwd->sp_pwdp);
    if (strcmp(encrypted, spwd->sp_pwdp) != 0) {
        printf("Mat khau khong dung!\n");
        exit(EXIT_FAILURE);
	}

		/*-----HASH NEW PASSWORD, WRITE TO /etc/shadow-----*/
	password = getpass("Nhap mat khau moi: ");

	// we can use old salt for hashing new password
	encrypted = crypt(password, spwd->sp_pwdp);

	spwd->sp_pwdp = encrypted;

	// replace the line contain `username` with the new version (updated password field)
	FILE *file = fopen("/etc/shadow", "r");
	FILE *fileTemp = fopen("/tmp/replace.tmp", "w");
	if (!file || !fileTemp) {
    	printf("Mo file that bai!\n");
    	exit(EXIT_FAILURE);
    }

    // read line by line in /etc/shadow
    char * line;
    size_t len = 0;
	while (getline(&line, &len, file) != -1) {
		// if this line contain `username`
		if (strstr(line, username) != NULL) {
			// then write *spwd as the shadow password file format to /tmp/replace.tmp
			putspent(spwd, fileTemp);
		}
		else {
			// otherwise write the line
			fputs(line, fileTemp);
		}

    }
    // replace /etc/shadow with /tmp/replace.tmp
    remove("/etc/shadow");
    rename("/tmp/replace.tmp", "/etc/shadow");

    fclose(file);
    fclose(fileTemp);
	exit(EXIT_SUCCESS);
}
