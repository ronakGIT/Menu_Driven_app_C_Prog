#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stddef.h>

#ifdef _WIN32
#include <Windows.h>
#else
#include <unistd.h>
#endif

/***************************************************/
/*********** Preproccessor declaration *************/
/***************************************************/
typedef struct {
	char name[32];
	char designation[64];
	char id[32];
	int age;
}Info;

/***************************************************/
/*********** Preproccessor declaration *************/
/***************************************************/
#define SUCCESS		0
#define FAILURE		1

#define MAX_INVALID_ARG_COUNT	2
#define MAX_INFO_LIST		    2

#define MAX_ATTEMPT				3
#define MAX_PASS_LEN			16

/***************************************************/
/************** Type declaration *******************/
/***************************************************/
typedef enum {
	false = 0,
	true = 1
}_bool;

/***************************************************/
/************** Functions declaration **************/
/***************************************************/
void app_init(void);
void update_pass(void);
void create_password(void);
int is_authorised_person(void);
void display_info_by_name(void);
void display_info(void);
int add_entry(int entry_count);
void reset_info_list(void);
void print_menu(void);
void get_user_input(char *opt);
int menu(char choice);

/***************************************************/
/************** Functions declaration **************/
/***************************************************/
Info info_list[MAX_INFO_LIST];
char default_pass[MAX_PASS_LEN];
int entry_count = 0;

/***************************************************/
/************** Functions definition ***************/
/***************************************************/
int main(int argc, char *argv[]) {

	char choice = 0;
	int invalid_arg_count = 0;
	int ret = FAILURE;

	app_init();
	
	while(true) {
		get_user_input(&choice);
		ret = menu(choice);
		if (ret != SUCCESS) {
			printf("Invalid argument -> [%d]\n", invalid_arg_count + 1);
			printf("Maximum invalid argument count allowed is -> [%d]\n", MAX_INVALID_ARG_COUNT);
			print_menu();
			invalid_arg_count++;
			if (invalid_arg_count > MAX_INVALID_ARG_COUNT) {
				printf("Invalid arg count exceeds the range, hence terminating the application.\n");
				exit(FAILURE);
			}
		}
	}

	return (SUCCESS);
}

/***************************************************/
/***************************************************/
void app_init(void) {
	printf("\nDemonstration of menu driven application to access information.\n\n");
	reset_info_list();
	print_menu();
	create_password();	
}

/***************************************************/
/***************************************************/
void update_pass(void) {
	char pass[MAX_PASS_LEN];
	char confirm_pass[MAX_PASS_LEN];
	int invalid_count = 0;
	
	while (true) {
		memset(pass, 0, sizeof(pass));
		memset(confirm_pass, 0, sizeof(confirm_pass));
		
		printf("\tEnter current password: ");
		scanf("%s", pass);
		
		if (!strcmp(default_pass, pass)) {
			while (true) {
				memset(pass, 0, sizeof(pass));
				memset(confirm_pass, 0, sizeof(confirm_pass));
				memset(default_pass, 0, sizeof(default_pass));
				
				printf("\tEnter a new pass: ");
				scanf("%s", pass);
				printf("\tPlease confirm pass: ");
				scanf("%s", confirm_pass);
				
				if (!strcmp(pass, confirm_pass)) {
					printf("\tPassword updated successfully.\n");
					strcpy(default_pass, pass);
					return;
				}
				printf("\tPassword does not match, please try again\n");
			}
		}
		printf("\tPassword does not match, please try again\n");
		if (++invalid_count > MAX_ATTEMPT) {
			printf("\tPlease wait for a minute.\n");
#ifdef _WIN32
  			Sleep(1000);
#else
  			usleep(1000*1000*5);  /* sleep for 5 Seconds */
#endif
		}
	}
}

/***************************************************/
/***************************************************/
void create_password(void) {
	char pass[MAX_PASS_LEN];
	char confirm_pass[MAX_PASS_LEN];
	
	printf("\nCreate a new password before accessing menu.\n");
	
	while (true) {
		memset(pass, 0, sizeof(pass));
		memset(confirm_pass, 0, sizeof(confirm_pass));
		memset(default_pass, 0, sizeof(default_pass));
		
		printf("\tEnter a password: ");
		scanf("%s", pass);
		printf("\tConfirm a pssword: ");
		scanf("%s", confirm_pass);
		
		if (!strcmp(pass, confirm_pass)) {
			printf("\tPassword set successfully.\n");
			strcpy(default_pass, pass);
			break;
		}
		printf("\tPassword does not match, please try again.\n\n");
	}
}

/***************************************************/
/***************************************************/
int is_authorised_person(void) {
	char pass[MAX_PASS_LEN];
	memset(pass, 0, sizeof(pass));
	printf("Enter a password: ");
	scanf("%s", pass);
	
	if (!strcmp(default_pass, pass)) {
		printf("Access granted!\n");
		return SUCCESS;
	}
	
	printf("Invalid password, please try again\n");

	return FAILURE;
}

/***************************************************/
/***************************************************/
void display_info_by_name(void) {
	if(!entry_count) {
		printf("\tDatabase is empty.\n");
		return;	
	}
	
	int idx = 0;
	for (idx=0; idx<MAX_ATTEMPT; idx++) {
		if (is_authorised_person() == SUCCESS) {
			break;
		}	
	}
	
	if (idx == MAX_ATTEMPT) {
		printf("\nMax incorrect password limit exceeds, hence terminating the application.\n");
		exit(1);	
	}		
	
	char name[32];
	
	memset(name, 0, sizeof(name));
	printf("Enter a name to fetch an info from DB: ");
	scanf("%s", name);
	
	//TODO: Application got hung when entering invalid name
	for(idx=0; idx<entry_count; idx++) {
#if 0
		if (info_list[idx].name == NULL) {
			printf("Encounter an empty name.\n");
			break;	
		}
#endif
		if (!strcmp(name, info_list[idx].name)) {
			printf("Match found\n");
			printf("\tName: %s\n", info_list[idx].name);
			printf("\tDesignation: %s\n", info_list[idx].designation);
			printf("\tId: %s\n", info_list[idx].id);
			printf("\tAge: %d\n\n", info_list[idx].age);
			return;	
		}
	}
	printf("\"%s\" does not found in the database.\n");
}

/***************************************************/
/***************************************************/
void display_info(void) {
	int idx = 0;
	
	if (!entry_count) {
		printf("\tDatabase is empty.\n");
		return;
	}
	
	printf("Database contains [%d] entry\n", entry_count);
	
	for(idx=0; idx<entry_count; idx++) {
		printf("Entry -> [%d]\n", idx+1);
		printf("\tName: %s\n", info_list[idx].name);
		printf("\tDesignation: %s\n", info_list[idx].designation);
		printf("\tId: %s\n", info_list[idx].id);
		printf("\tAge: %d\n\n", info_list[idx].age);
	}
}

/***************************************************/
/***************************************************/
int add_entry(int count) {
	if (count > MAX_INFO_LIST) {
		return FAILURE;
	}

	printf("\tEnter a name: ");
	gets(info_list[count - 1].name);

	printf("\tEnter a designation: ");
	gets(info_list[count - 1].designation);

	printf("\tEnter a id: ");
	gets(info_list[count - 1].id);

	printf("\tEnter an age: ");
	scanf("%d", &info_list[count - 1].age);

	printf("\nSummary\n");
	printf("\tName: %s\n", info_list[count - 1].name);
	printf("\tDesignation: %s\n", info_list[count - 1].designation);
	printf("\tId: %s\n", info_list[count - 1].id);
	printf("\tAge: %d\n\n", info_list[count - 1].age);
	
	return SUCCESS;
}

/***************************************************/
/***************************************************/
void reset_info_list(void) {
	int idx = 0;
	for (idx=0; idx<MAX_INFO_LIST; idx++) {
		memset(info_list[idx].name, 0, sizeof(info_list[idx].name));
		memset(info_list[idx].designation, 0, sizeof(info_list[idx].designation));
		memset(info_list[idx].id, 0, sizeof(info_list[idx].id));
		info_list[idx].age = 0;
	}
	entry_count = 0;
}

/***************************************************/
/***************************************************/
void print_menu(void) {
	printf("\tPress 1 to add a new entry\n");
	printf("\tPress 2 to display information\n");
	printf("\tPress 3 to display information by his/her name\n");
	printf("\tPress p/P to change the password\n");
	printf("\tPress r/R to reset the database\n");
	printf("\tPress q/Q to quit the application\n");
	printf("\tPress h/H to get a help\n");
}

/***************************************************/
/***************************************************/
void get_user_input(char *opt) {
	char choice;
	printf("\nEnter you choice: ");
	scanf(" %c", &choice);
	*opt = choice;
	fflush(stdin);
}

/***************************************************/
/***************************************************/
int menu(char choice) {
	switch (choice) {
	case '1':
		printf("\nAdd new entry\n");
		if (add_entry(++entry_count) != SUCCESS) {
			entry_count = 0;
			printf("Database limit exceeds, hence terminating the application\n");
			exit(FAILURE);
		}
		break;

	case '2':
		printf("\nDisplay information\n");
		display_info();
		break;

	case '3':
		printf("\nDisplay information by his/her name\n");
		display_info_by_name();
		break;
		
	case 'p':
	case 'P':
		printf("\nUpdate a new password\n\n");
		update_pass();
		break;
		
	case 'r':
	case 'R':
		printf("\nReseting database...");
	    reset_info_list();
	    printf("DONE\n");
	    break;

	case 'h':
	case 'H':
		print_menu();
		break;

	case 'q':
	case 'Q':
		printf("Exiting from the application\n");
		printf("Thank You!");
		exit(SUCCESS);

	default:
		return (FAILURE);
	}

	return (SUCCESS);
}
