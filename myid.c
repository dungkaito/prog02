#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <pwd.h>
#include <grp.h>

int main() {
	printf("Nhap username: ");

	// get username from input
	char name[32];
	fgets(name, 32, stdin);	// use fgets to eliminate buffer overflow

	// removing trailing newline character from fgets() input
	name[strcspn(name, "\n")] = 0;

	// get a record (line) from /etc/passwd where username = name
	struct passwd *pwd = getpwnam(name);
	if (pwd == NULL) {
		printf("Khong tim thay user tuong ung!\n");
		exit(EXIT_FAILURE);
	}

	// get list of groups (GID) to which the user *pwd belongs
	int ngroups = 0; // number of groups
	// this call is just to get the correct ngroups
	getgrouplist(pwd->pw_name, pwd->pw_gid, NULL, &ngroups);
	gid_t groups[ngroups];
	// here we actually get the groups
	getgrouplist(pwd->pw_name, pwd->pw_gid, groups, &ngroups);

	// OUTPUT
	printf("====Thong tin nguoi dung====\n");
	printf("ID: %u\n", pwd->pw_uid);
	printf("Username: %s\n", pwd->pw_name);
	printf("Thu muc home: %s\n", pwd->pw_dir);
	printf("Danh sach group: ");
	for (int i=0; i<ngroups; i++) {
		printf("%s ", getgrgid(groups[i])->gr_name);
	}
	printf("\n============================\n");

	exit(EXIT_SUCCESS);
}

